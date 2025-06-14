#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid     = "galaxy";
const char* password = "wifipass";

WiFiUDP udp;
const unsigned int serverPort = 1234;
const IPAddress serverIP(192, 168, 159, 46); // Remplace par l’IP de ton ESP32 serveur

unsigned long lastSendTime = 0;
int counter = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Connexion au Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
  udp.begin(1234);
}

void loop() {
  if (millis() - lastSendTime > 2000) { // toutes les 2 secondes
    String message = "Data compteur: " + String(counter++);
    udp.beginPacket(serverIP, serverPort);
    udp.print(message);
    udp.endPacket();
    Serial.println("Message envoyé : " + message);
    lastSendTime = millis();
  }
}
