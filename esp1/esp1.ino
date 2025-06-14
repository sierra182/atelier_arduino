#include <WiFi.h>
#include <ArduinoJson.h>
#include "interface.h"

WiFiServer server(1234);  // Port TCP
const char* ssid = "ESP32_Server";
const char* password = "12345678";

StaticJsonDocument<200> doc;

float values[3] = {0.0, 0.0, 0.0};
float diffValues[3] = {0.0, 0.0, 0.0};

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

}

void loop() {
  interface_loop(values, diffValues);
  WiFiClient client = server.available();
  if (client) {
    Serial.println("📡 Client connected");

    while (client.connected()) {
      if (client.available()) {

        String msg = client.readStringUntil('\n');
        Serial.print("📨 Received : ");
        Serial.println(msg);
        parseJson(doc, msg);
        // ✅ Envoi de l'accusé de réception
        client.println("ACK: message received !");
        Serial.println("✅ Response sent.");
      }
    }
    client.stop();
    Serial.println("❌ Client disconnected");
  }
  delay(100);
}
