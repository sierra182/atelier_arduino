/*int pinLed1 = 27;
int pinLed2 = 26;
int pinLed3 = 25;
int pinLed4 = 32;
int pinLed5 = 22;
int pinLed6 = 23;

void setup() {
  Serial.begin(115200);
  pinMode(pinLed1, OUTPUT); // Définir le GPIO comme sortie
  pinMode(pinLed2, OUTPUT);
  pinMode(pinLed3, OUTPUT);
  pinMode(pinLed4, OUTPUT);
  pinMode(pinLed5, OUTPUT);
  pinMode(pinLed6, OUTPUT);
  while (!Serial)
  {
    ;
  }
  Serial.println("ESP32 OK !");
}

void loop() {
  digitalWrite(pinLed1, HIGH); // LED ON
  digitalWrite(pinLed2, HIGH);
  digitalWrite(pinLed3, HIGH);
  digitalWrite(pinLed4, HIGH);
  digitalWrite(pinLed5, HIGH);
  digitalWrite(pinLed6, HIGH);
  Serial.println("LED ON");
  Serial.println("Je suis vivant !/n/n");

  delay(1000);

  digitalWrite(pinLed1, LOW);  // LED OFF
  digitalWrite(pinLed2, LOW);
  digitalWrite(pinLed3, LOW);
  digitalWrite(pinLed4, LOW);
  digitalWrite(pinLed5, LOW);
  digitalWrite(pinLed6, LOW);
  Serial.println("LED OFF");
  Serial.println("Vie de Merde !");
  delay(1000);                // 1 seconde éteinte
}*/

#include <WiFi.h>

int pins[] = {27, 26, 25, 32, 22, 23}; // tableau des GPIO
const int numLeds = 6;
int current = 0;
const char* ssid = "ESP32_Server";
const char* password = "12345678";
const char* host = "192.168.4.1";  // IP du SoftAP

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < numLeds; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }
  Serial.println("ESP32 - Vague LED");
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
  // Allumer LED courante
  digitalWrite(pins[current], HIGH);
  Serial.print("LED ON GPIO ");
  Serial.println(pins[current]);

  // Éteindre la LED précédente
  int previous = (current == 0) ? numLeds - 1 : current - 1;
  digitalWrite(pins[previous], LOW);

  current = (current + 1) % numLeds; // passer à la suivante
  delay(300); // durée entre chaque LED
}