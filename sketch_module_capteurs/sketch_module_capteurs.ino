int potPin = 26; // GPIO où est branché le signal du potentiomètre
int therPin = 27; // GPIO où est branché le signal du potentiomètre
int ldrPin = 32; // GPIO où est branché le signal du potentiomètre
int valuePot = 0;
int valueTher = 0;
int valueLdr = 0;

void setup() {
  Serial.begin(115200); // plus rapide que 9600, et l'ESP32 gère bien
}

void loop() {
  valuePot = analogRead(potPin); // lecture analogique (0 à 4095 sur ESP32)
  valueTher = analogRead(therPin); // lecture analogique (0 à 4095 sur ESP32)
  valueLdr = analogRead(ldrPin); // lecture analogique (0 à 4095 sur ESP32)
  Serial.print("Potentiomètre : ");
  Serial.println(valuePot);
  Serial.print("Thermistance : ");
  Serial.println(valueTher);
  Serial.print("LDR : ");
  Serial.println(valueLdr);
  delay(500); // pour ne pas spammer
}