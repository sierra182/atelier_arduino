#include <WiFi.h>

// *******************************************************************
// This module reads values from three captors,
// formats them as a JSON string,
// and send them on the Network "ESP32_Server"
// *******************************************************************

const char* ssid = "ESP32_Server";
const char* password = "12345678";
const char* host = "192.168.4.1";

int potPin = 35;
int therPin = 34;
int ldrPin = 32;
int valuePot = 0;
int valueTher = 0;
int valueLdr = 0;


void setup() {
    WiFi.begin(ssid, password);

    Serial.begin(115200);
    delay (3000);
    Serial.print("Connection");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\n✅ Connected !");
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
    if (client.connect(host, 1234))
    {
        Serial.println("📡 Connexion to the server successful !");
        client.println(message);
        Serial.println("📤 sent : " + message);

        // 4. Lire la réponse du serveur
        String response = client.readStringUntil('\n');
        Serial.println("📬 Response : " + response);
        client.stop();
    }
    else
    {
        Serial.println("❌ Connexion failed");
    }
    delay(1000);
}
