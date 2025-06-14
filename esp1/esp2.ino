#include <WiFi.h>

const char* ssid = "ESP32_Server";
const char* password = "12345678";
const char* host = "192.168.4.1";  // IP du SoftAP

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    Serial.print("Connexion");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\n✅ Connecté !");
    delay(1000);
}

void loop() {
    WiFiClient client;
    if (client.connect(host, 1234)) {
        client.println("Bonjour serveur 👋");

        // 🔄 Attente de la réponse
        String response = client.readStringUntil('\n');
        Serial.print("📬 Réponse du serveur : ");
        Serial.println(response);

        client.stop();
    } else {
        Serial.println("❌ Connexion échouée");
    }

    delay(5000);  // Envoi toutes les 5 secondes
}
