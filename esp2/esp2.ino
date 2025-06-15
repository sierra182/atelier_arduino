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

unsigned long lastAttemptTime = 0;
const long retryInterval = 5000; // 5 secondes
bool wifiConnected = false;

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  lastAttemptTime = millis();
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        
        if (!wifiConnected && millis() - lastAttemptTime >= retryInterval) {
            Serial.println("🔁 Tentative de connexion...");
            WiFi.begin(ssid, password);
            lastAttemptTime = millis();
        }

        if (WiFi.status() == WL_CONNECTED && !wifiConnected) {
            wifiConnected = true;
            Serial.println("✅ Connecté !");
            // Ici tu peux lancer le reste de ton programme (TCP client, etc.)
        }

        // Optionnel : détecter une déconnexion
        if (wifiConnected && WiFi.status() != WL_CONNECTED) {
            Serial.println("⚠️ Perte de connexion !");
            wifiConnected = false;
        }
        
        // 1. Lire les capteurs
        valuePot = analogRead(potPin);
        valueTher = analogRead(therPin);
        valueLdr = analogRead(ldrPin);

        // 2. Créer le message
        String message = "{\"pot\":" + String(valuePot) +
                        ",\"ther\":" + String(valueTher) +
                        ",\"ldr\":" + String(valueLdr) + "}";

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
