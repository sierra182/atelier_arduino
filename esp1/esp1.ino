#include <WiFi.h>
#include "interface.h"

// WiFiServer server(1234);  // Port TCP
// const char* ssid = "ESP32_Server";
// const char* password = "12345678";

void setup() {

  interface_setup();
  // WiFi.mode(WIFI_AP);
  // WiFi.softAP(ssid, password);

  // Serial.println("AP créé !");
  // Serial.print("IP : ");
  // Serial.println(WiFi.softAPIP());

  // server.begin();
  // Serial.println("Serveur TCP en écoute.");
 
}

void loop() {

  interface_loop();
  // WiFiClient client = server.available();
  // if (client) {
  //   Serial.println("📡 Client connecté");

  //   while (client.connected()) {
  //     if (client.available()) {

  //       String msg = client.readStringUntil('\n');
  //       Serial.print("📨 Reçu : ");
  //       Serial.println(msg);

  //       // ✅ Envoi de l'accusé de réception
  //       client.println("ACK: message reçu !");
  //       Serial.println("✅ Réponse envoyée.");
  //     }
  //   }
  //   client.stop();
  //   Serial.println("❌ Client déconnecté");
  // }
  delay(100);
}
