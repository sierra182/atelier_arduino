import serial
import json
import time

# Ouvre le port série (remplace par ton port et baudrate)
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

# Exemple de données à envoyer
data = {
    "ther": 2500,
    "pot": 2600,
    "ldr": 2700
}

# Convertit en JSON
json_data = json.dumps(data)

try:
    while True:
        # Envoie la chaîne JSON suivie d'un saut de ligne (pour faciliter la lecture côté récepteur)
        ser.write((json_data + '\n').encode('utf-8'))
        print("Données envoyées :", json_data)
        time.sleep(1)  # envoie toutes les secondes
except KeyboardInterrupt:
    print("Arrêt du programme")

ser.close()

