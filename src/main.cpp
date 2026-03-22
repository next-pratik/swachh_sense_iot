#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Pixel_2957";
const char* password = "pratik123";

const char* mqtt_server = "autometra.cloud";

WiFiClient espClient;
PubSubClient client(espClient);

#define MQ137_PIN 34

void setup_wifi() {

  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {

  while (!client.connected()) {

    if (client.connect("ESP32_MQ137")) {
      Serial.println("MQTT connected");
    } 
    else {
      delay(5000);
    }
  }
}

void setup() {

  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  int sensorValue = analogRead(MQ137_PIN);

  char msg[50];
  sprintf(msg, "%d", sensorValue);

  client.publish("sensor/mq137", msg);

  Serial.print("MQ137 Value: ");
  Serial.println(sensorValue);

  delay(5000);
}