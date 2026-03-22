#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Pixel_2957";
const char* password = "pratik123";

// MQTT Broker
const char* mqtt_server = "autometra.cloud";

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// Sensor Pins
#define MQ137_PIN 34
#define MQ135_PIN 35
#define MQ136_PIN 32

// ---------------- WIFI ----------------
void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
}

// ---------------- MQTT RECONNECT ----------------
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");

    if (client.connect("ESP32_GAS_SENSOR")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
}

// ---------------- LOOP ----------------
void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // Read sensors
  int mq137Value = analogRead(MQ137_PIN);
  int mq135Value = analogRead(MQ135_PIN);
  int mq136Value = analogRead(MQ136_PIN);

  // Convert to string
  char msg1[10];
  char msg2[10];
  char msg3[10];

  sprintf(msg1, "%d", mq137Value);
  sprintf(msg2, "%d", mq135Value);
  sprintf(msg3, "%d", mq136Value);

  // Publish to MQTT topics
  client.publish("sensor/mq137", msg1);
  client.publish("sensor/mq135", msg2);
  client.publish("sensor/mq136", msg3);

  // Debug output
  Serial.print("MQ137: ");
  Serial.print(mq137Value);
  Serial.print(" | MQ135: ");
  Serial.print(mq135Value);
  Serial.print(" | MQ136: ");
  Serial.println(mq136Value);

  delay(5000);
}