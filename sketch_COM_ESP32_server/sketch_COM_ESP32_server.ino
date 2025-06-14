#include <WiFi.h>
#include <WiFiUdp.h>
#include <WebServer.h>

const char* ssid     = "galaxy";
const char* password = "wifipass";

WiFiUDP udp;
const unsigned int localPort = 1234;

WebServer server(80);

String lastMessage = "No message received";

void handleRoot() {
  String page = "<!DOCTYPE html><html><head><title>ESP32 Broadcast Receiver</title>";
  page += "<meta http-equiv=\"refresh\" content=\"2\">";
  page += "</head><body>";
  page += "<h1>Last UDP message received :</h1>";
  page += "<p>" + lastMessage + "</p>";
  page += "<h2>ESP32 IP Address :</h2>";
  page += "<p>" + WiFi.localIP().toString() + "</p>";
  page += "</body></html>";
  server.send(200, "text/html", page);
}

void setup() {
  Serial.begin(115200);
  while (!Serial){
    ;
  }
  Serial.println("Connectiong to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected !");
  Serial.print("IP Adress : ");
  Serial.println(WiFi.localIP());

  udp.begin(localPort);

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Webserver started");
}

void loop() {
  // Reception UDP
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incomingPacket[255];
    int len = udp.read(incomingPacket, 254);
    if (len > 0) {
      incomingPacket[len] = 0; // Terminer la chaîne
      lastMessage = String(incomingPacket);
      Serial.println("Message received : " + lastMessage);
    }
  }

  server.handleClient();
}
