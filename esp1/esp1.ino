#include <WiFi.h>
#include <ArduinoJson.h>
#include "interface.h"
#include <WiFiUdp.h>

WiFiUDP udp;
const unsigned int udpPort = 1235;  // un port UDP pour le broadcast

WiFiServer server(1234);  // Port TCP
const char* ssid = "ESP32_Server";
const char* password = "12345678";

StaticJsonDocument<200> doc;

float values[3] = {0.0, 0.0, 0.0};
float diffValues[3] = {0.0, 0.0, 0.0};

static unsigned long previousMillis = 0;
const long interval = 500;

void parseJson(StaticJsonDocument<200> &doc, String& json) {
 
  DeserializationError error = deserializeJson(doc, json.c_str());
  if (error) {
    Serial.print("Parsing error: ");
    Serial.println(error.c_str());
    return;
  }
  Serial.println("result: ");
  values[0] = doc["pot"];
  values[1] = doc["ther"];
  values[2] = doc["ldr"];

  Serial.println(values[0]);
  Serial.println(values[1]);
  Serial.println(values[2]);
}

void setup() {
  Serial.begin(115200);
  delay (3000);

  interface_setup();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println("AP créé !");
  Serial.print("IP : ");
  Serial.println(WiFi.softAPIP());

  server.begin();
  Serial.println("TCP Server listening.");

  udp.begin(udpPort);
  Serial.println("UDP started");
}

void loop() {
  interface_loop(values, diffValues);
  // WiFiClient client = server.available();
  static WiFiClient client;

  if (!client || !client.connected()) {
    client = server.available();
    if (client) {
      Serial.println("📡 Client connected");
    }
  }

  if (client && client.connected() && client.available()) {
    String msg = client.readStringUntil('\n');
    parseJson(doc, msg);
    client.println("ACK: message received !");
    Serial.println("✅ Response sent.");
  }
  String broadCastMsg = "{\"pot\":" + String(diffValues[0], 2) +
                      ",\"ther\":" + String(diffValues[1], 2) +
                      ",\"ldr\":" + String(diffValues[2], 2) + "}";

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // Serial.println(broadCastMsg);
    udp.beginPacket("192.168.4.255", udpPort);  // adresse de broadcast sur le réseau AP
    udp.write((const uint8_t*)broadCastMsg.c_str(), broadCastMsg.length());
    udp.endPacket();
  }
  
  if (client && !client.connected()) {
    Serial.println("❌ Client disconnected");
    client.stop();
  }
}
