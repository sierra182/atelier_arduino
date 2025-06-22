import openai
import json
import os
from dotenv import load_dotenv
from openai import OpenAI

load_dotenv()
client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))
# 🛠️ Fonction que le LLM pourra appeler
def dire_bonjour(name):
    return f"Bonjour truc bidule {name} ! 😊"

def send_value(sensor: str, value: float):
    print(f"je vais envoyer ces infos a l'arduino: {sensor} : {value}")
    import serial
    import time

    # 🧭 Ouvre la connexion série
    ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
    time.sleep(2)  # ⏳ Attend que la connexion soit prête

    # # ✉️ Envoie un message à l'Arduino
    # ser.write(b'sensor-value')
    import json

    data = {
        "sensor": sensor,
        "value": value
    }

    # 🔄 Conversion en chaîne JSON
    json_str = json.dumps(data) + "\n"  # le \n permet à l'Arduino de savoir quand le message se termine

    # 📤 Envoi via le port série
    ser.write(json_str.encode('utf-8'))
        # 📥 Lit une réponse (si l'Arduino en envoie)
    # line = ser.readline().decode('utf-8').strip()
    # print(f"Reçu depuis Arduino : {line}")

    # 🔚 Ferme la connexion
    ser.close()

# 📦 Déclaration du tool (function)
function_tool = [
    {
        "name": "send_value",
        "description": "send the sensor type and the value to the arduino device",
        "parameters": {
            "type": "object",
            "properties": {
                "sensor": {
                    "type": "string",
                    "description": "type of the sensor"
                },
                 "value": {
                    "type": "number",
                    "description": "value of the sensor limit"
                }
            },
            "required": ["sensor", "value"]
        }
    }
]

# 🧠 Envoi d'un message à GPT avec tool disponible
def infer(prompt: str):
    response = client.chat.completions.create(
        model="gpt-4o",  
        messages = [
        {
            "role": "system",
            "content": (
            "Tu es un assistant qui agit uniquement quand l'utilisateur fournit deux informations :\n"
            "1️⃣ le **type de capteur** ne peut etre que soit 'temperature' ou 'humidity', etc.)\n"
            "2️⃣ la **valeur seuil** (ex: 42)\n\n"
            "Ces deux éléments peuvent être **explicites ou implicites dans la phrase**. "
            "Tu dois être capable de les **interpréter même s’ils sont sous-entendus** ».\n\n"
            "➡️ Si tu détectes les deux, appelle la fonction correspondante.\n"
            "l'argument sensor pour la fonction, peux uniquement etre 'temp' ou 'hum'"
            "❌ Sinon, explique à l'utilisateur que tu attends un type de capteur et une valeur seuil, sans essayer d'inventer ou de dévier."
        )
        },
        {
            "role": "user",
            "content": prompt
        }
    ],
        functions=function_tool,
        function_call="auto"
    )

    # 🔍 Si GPT décide d'appeler une fonction...
    message = response.choices[0].message
    if message.function_call is not None:
        func_name = message.function_call.name
        args = json.loads(message.function_call.arguments)
        
        if func_name == "send_value":
            result = send_value(**args)
            # print("✅ Appel réel de la fonction :", result)
    else:
        print("🤖 Réponse directe du LLM :", message.content)

def main():
    print("start")
    while True:
        try:
            prompt = input(">>> ")  # ⌨️ Lecture depuis l'entrée standard
            infer(prompt)
        except BaseException:
            break

if __name__ == "__main__":
    main()