#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define MUX_ADDR 0x70

#define BOUTON_HAUT 32
#define BOUTON_BAS 33
#define BOUTON_VALIDER 26
#define BOUTON_ANNULER 25

Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Menu
const char* menuItems[] = {"Potentiometer", "Thermistor", "Photoresistor"};
const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);
int selectedIndex = 0;

bool lastHaut = HIGH;
bool lastBas = HIGH;
bool lastValider = HIGH;
bool lastAnnuler = HIGH;

unsigned long lastBlinkTime = 0;
bool blinkState = false;
const unsigned long blinkInterval = 200;

float thresholdValues[] = {2048.00, 2048.00, 2048.00};

uint8_t lastMuxChannel = 255; // Valeur impossible → forcera la première sélection

String generateBar(int thresholdValue, int max = 10) {
  String bar = "";
  for (int i = 0; i < max; i++) {
    if (i < thresholdValue) {
      bar += (char)219; // caractère plein : █
    } else {
      bar += (char)32; // caractère vide : espace
    }
  }
  return bar;
}

void selectMuxChannel(uint8_t channel) {
  if (channel > 7 || channel == lastMuxChannel) return; // Évite les appels inutiles
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
  lastMuxChannel = channel; // Mise à jour
}

int getGraphThresholdValue(int selectedIndex)
{
  return (int)thresholdValues[selectedIndex] / 409.6;
}

void afficherDetail(const char* texte, int selectedIndex, float value, bool showBar = true) {
  selectMuxChannel(2); // écran droit
  display2.clearDisplay();
  display2.setTextSize(1);
  display2.setTextColor(SSD1306_WHITE);
  display2.setCursor(0, 0);
  display2.setTextSize(1);
  display2.println(texte);
  display2.println("");
  display2.print("value: ");
  display2.println(value);
  display2.setTextSize(2);
  if (showBar) {
    display2.println(generateBar(getGraphThresholdValue(selectedIndex)));
  } else {
    display2.println("          "); // espace vide (même largeur que la barre)
  }
  display2.setTextSize(1);
  display2.println("");
  display2.print("Cap:   ");
  display2.println(thresholdValues[selectedIndex]);
  display2.display();
}



void afficherMenu() {
  selectMuxChannel(5); // écran gauche
  display1.clearDisplay();
  display1.setTextSize(1);
  display1.setTextColor(SSD1306_WHITE);
  display1.setCursor(0, 0);

  for (int i = 0; i < menuLength; i++) {
    if (i == selectedIndex) {
      display1.print("> ");
    } else {
      display1.print("  ");
    }
    display1.println(menuItems[i]);
    display1.println("");
  }

  display1.display();
}

void interface_setup() {
  pinMode(BOUTON_HAUT, INPUT_PULLUP);
  pinMode(BOUTON_BAS, INPUT_PULLUP);
  pinMode(BOUTON_VALIDER, INPUT_PULLUP);
  pinMode(BOUTON_ANNULER, INPUT_PULLUP);



  Wire.begin(23, 22);

  // Écran gauche
  selectMuxChannel(5);
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display1.clearDisplay();
  display1.display();

  // Écran droit (inutile ici pour l’instant)
  selectMuxChannel(2);
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display2.clearDisplay();
  display2.display();

  afficherMenu();
}
bool stateOptionDisplayed = false;
bool editMode = false;



void interface_loop(float values[], float diffValues[]) {
  bool stateHaut = digitalRead(BOUTON_HAUT);
  bool stateBas = digitalRead(BOUTON_BAS);
  bool stateValider = digitalRead(BOUTON_VALIDER);
  bool stateAnnuler = digitalRead(BOUTON_ANNULER);
  int potVal = 0;

  if (!editMode)
    afficherDetail(menuItems[selectedIndex], selectedIndex, values[selectedIndex]);

  if (lastAnnuler == HIGH && stateAnnuler == LOW) {
    selectMuxChannel(2);
    display2.clearDisplay();
    display2.display();
    stateOptionDisplayed = false;
    editMode = false;
  }

  if (lastValider == HIGH && stateValider == LOW && !stateOptionDisplayed) {
    afficherDetail(menuItems[selectedIndex], selectedIndex, values[selectedIndex]);
    stateOptionDisplayed = true;
    editMode = false;
  } else if (lastValider == HIGH && stateValider == LOW && stateOptionDisplayed && !editMode) {
    editMode = true;
    Serial.println("Mode édition activé");
  }

  if (editMode) {
    unsigned long currentMillis = millis();
  if (currentMillis - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentMillis;
      blinkState = !blinkState; // inverse le bool à chaque intervalle
    }
    potVal = analogRead(35);
    thresholdValues[selectedIndex] = 4095 - potVal;
    afficherDetail(menuItems[selectedIndex], selectedIndex, values[selectedIndex], blinkState);
  }
  

  if (lastHaut == HIGH && stateHaut == LOW) {
    selectedIndex = (selectedIndex - 1 + menuLength) % menuLength;
    editMode = false;
    afficherMenu();
    afficherDetail(menuItems[selectedIndex], selectedIndex, values[selectedIndex]);
  }

  if (lastBas == HIGH && stateBas == LOW) {
    selectedIndex = (selectedIndex + 1) % menuLength;
    editMode = false;
    afficherMenu();
    afficherDetail(menuItems[selectedIndex], selectedIndex, values[selectedIndex]);
  }

  // update diffValues
  diffValues[0] = thresholdValues[0] - values[0];
  diffValues[1] = thresholdValues[1] - values[1];
  diffValues[2] = thresholdValues[2] - values[2];

  lastAnnuler = stateAnnuler;
  lastValider = stateValider;
  lastHaut = stateHaut;
  lastBas = stateBas;
}
