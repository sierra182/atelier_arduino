import serial
import time

# Connexion série
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
time.sleep(2)  # Attente que l'Arduino redémarre

# Envoie un message une fois
ser.write(b'Hello Arduino\n')

# Boucle de lecture continue
try:
    while True:
        if ser.in_waiting > 0:  # Vérifie s’il y a des données à lire
            line = ser.readline().decode('utf-8').strip()
            print(f"Arduino dit : {line}")
except KeyboardInterrupt:
    print("Fin de lecture.")
    ser.close()

