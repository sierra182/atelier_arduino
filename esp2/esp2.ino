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

unsigned long previousMillis = 0;
const long interval = 200; // en ms (5 fois par seconde)

void setup() {

    Serial.begin(115200);
    Serial.println("Connection");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\n✅ Connected !");
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        
        // 0. check connection - reconnect if need be
        if (WiFi.status() != WL_CONNECTED) {
            WiFi.begin(ssid, password);
            delay(500);
            return;
        }
        
        // 1. Lire les capteurs
        valuePot = analogRead(potPin);
        valueTher = analogRead(therPin);
        valueLdr = analogRead(ldrPin);

        // 2. Créer le message
        String message = "{pot:" + String(valuePot) +
                        ",ther:" + String(valueTher) +
                        ",ldr:" + String(valueLdr) + "}";

        // 3. Envoyer au serveur
        WiFiClient client;
        if (client.connect(host, 1234))
        {
            Serial.println("📡 Connected to the server !");
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
    }
}
