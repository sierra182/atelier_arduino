import re
import json
import pyaudio
from vosk import Model, KaldiRecognizer
import dateparser
from datetime import datetime

# ✅ Table de conversion des nombres en lettres → chiffres
TRADUCTION_NOMBRES = {
    "deux mille cinquante": "2050",
    "deux mille vingt-six": "2026",
    "deux mille vingt-cinq": "2025",
    "deux mille trente": "2030",
    "deux mille quarante": "2040",
    "trente-et-un": "31", "trente": "30", "vingt-neuf": "29", "vingt-huit": "28",
    "vingt-sept": "27", "vingt-six": "26", "vingt-cinq": "25", "vingt-quatre": "24",
    "vingt-trois": "23", "vingt-deux": "22", "vingt-et-un": "21", "vingt": "20",
    "dix-neuf": "19", "dix-huit": "18", "dix-sept": "17", "seize": "16", "quinze": "15",
    "quatorze": "14", "treize": "13", "douze": "12", "onze": "11", "dix": "10",
    "neuf": "9", "huit": "8", "sept": "7", "six": "6", "cinq": "5", "quatre": "4",
    "trois": "3", "deux": "2", "un": "1", "une": "1", "zéro": "0"
}

def convertir_phrase(phrase):
    phrase = phrase.lower()
    phrase = phrase.replace("heures", "h").replace("heure", "h")

    # Remplacement ordonné du plus long au plus court
    mots_tries = sorted(TRADUCTION_NOMBRES.items(), key=lambda x: -len(x[0]))
    for mot, val in mots_tries:
        pattern = r'\b' + re.escape(mot) + r'\b'
        phrase = re.sub(pattern, val, phrase)
    return phrase

def extraire_datetime_depuis_phrase(phrase):
    phrase_convertie = convertir_phrase(phrase)
    print(f"🔎 Analyse convertie : « {phrase_convertie} »")

    # Extraire une séquence complète du type "le 25 juillet 2025 à 18 h 30"
    match = re.search(
        r"(?:le\s)?\d{1,2}(?:er)?\s\w+(?:\s\d{4})?\s?à\s\d{1,2}(?:\s?h(?:\s?\d{1,2})?)?",
        phrase_convertie
    )

    if match:
        combinaison = match.group(0).strip()
        # Convertir "18 h 30" en "18h30"
        combinaison = re.sub(r"(\d{1,2})\s?h\s?(\d{1,2})", r"\1h\2", combinaison)
        # Convertir "18 h" en "18h00"
        combinaison = re.sub(r"(\d{1,2})\s?h\b", r"\1h00", combinaison)
    else:
        combinaison = ""

    print(f"🧪 Phrase analysée : « {combinaison} »")
    dt = dateparser.parse(combinaison, languages=['fr'])

    # Ajouter l’année si absente
    if dt and not re.search(r"\d{4}", combinaison):
        now = datetime.now()
        dt = dt.replace(year=now.year)
        if dt < now:
            dt = dt.replace(year=now.year + 1)

    return dt

# 🔊 Initialisation du modèle VOSK
model = Model("vosk-model-small-fr-0.22")
recognizer = KaldiRecognizer(model, 16000)

p = pyaudio.PyAudio()
stream = p.open(format=pyaudio.paInt16, channels=1, rate=16000,
                input=True, frames_per_buffer=8000)
stream.start_stream()

print("🗣️ Parlez... (Dites « vosk fin de session » pour quitter)")

rendez_vous_log = []

try:
    while True:
        data = stream.read(4000, exception_on_overflow=False)
        if recognizer.AcceptWaveform(data):
            result = json.loads(recognizer.Result())
            text = result.get("text", "").strip().lower()

            if text:
                print("💬", text)

                if "vosk fin de session" in text:
                    print("🛑 Fin détectée par la voix.")
                    break

                if "rendez-vous" in text:
                    dt = extraire_datetime_depuis_phrase(text)
                    if dt:
                        rendez_vous_log.append((dt.strftime("%Y-%m-%d %H:%M"), text))
                        print(f"📌 Rendez-vous : {dt.strftime('%A %d %B %Y à %H:%M')} — « {text} »")
                    else:
                        print("❓ Rendez-vous détecté, mais date/heure non reconnue.")
except KeyboardInterrupt:
    print("⏹️ Fin de l'écoute (Ctrl+C).")
finally:
    stream.stop_stream()
    stream.close()
    p.terminate()

    if rendez_vous_log:
        print("\n📒 Récapitulatif :")
        for dt, phrase in rendez_vous_log:
            print(f"- [{dt}] → {phrase}")
    else:
        print("Aucun rendez-vous reconnu avec date et heure.")
