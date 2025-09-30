#include <WiFi.h>
#include <PubSubClient.h>

// กำหนดค่า WiFi และ MQTT
const char* ssid = "PHU";
const char* password = "12341234";
const char* mqtt_server = "172.20.10.13"; // IP ของเครื่องที่รัน Mosquitto

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
const long interval = 3000; // ส่งข้อมูลทุก 3 วินาที

void setup_wifi() {
  delay(100);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void sendRandomValues() {
  int value1 = random(10, 100);
  int value2 = random(50, 200);

  char payload[100];
  snprintf(payload, sizeof(payload), "{\"value1\": %d, \"value2\": %d}", value1, value2);

  client.publish("esp32/sensor", payload);
  Serial.print("Published: ");
  Serial.println(payload);
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

  unsigned long now = millis();
  if (now - lastMsg > interval) {
    lastMsg = now;
    sendRandomValues();
  }
}