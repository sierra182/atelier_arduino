#include <WiFi.h>

const char* ssid = "ESP32_Server";
const char* password = "12345678";
const char* host = "192.168.4.1";  // IP du SoftAP

int potPin = 34; // GPIO où est branché le signal du potentiomètre
int therPin = 35; // GPIO où est branché le signal du potentiomètre
int ldrPin = 32; // GPIO où est branché le signal du potentiomètre
int valuePot = 0;
int valueTher = 0;
int valueLdr = 0;


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
    // 1. Lire les capteurs
    int valuePot = analogRead(potPin);
    int valueTher = analogRead(therPin);
    int valueLdr = analogRead(ldrPin);

    // 2. Créer le message
    String message = "{pot:" + String(valuePot) +
                     ",ther:" + String(valueTher) +
                     ",ldr:" + String(valueLdr) + "}";

    // 3. Envoyer au serveur
    WiFiClient client;
    if (client.connect(host, 1234)) {
        Serial.println("📡 Connexion au serveur réussie !");
        client.println(message);
        Serial.println("📤 Envoyé : " + message);

        // 4. Lire la réponse du serveur
        String response = client.readStringUntil('\n');
        Serial.println("📬 Réponse : " + response);

        client.stop();
    } else {
        Serial.println("❌ Connexion échouée");
    }

    delay(1000);
}




