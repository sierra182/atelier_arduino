import speech_recognition as sr
from pydub import AudioSegment
from pydub.playback import play
from io import BytesIO

def voice_to_text() -> str:
	r = sr.Recognizer()
	r.pause_threshold = 2.0
	r.energy_threshold = 200
	with sr.Microphone() as source:
		print(source.list_microphone_names())
		print("🔧 Calibrage en cours...")
		r.adjust_for_ambient_noise(source, duration=1)
		print("🔊 Parle, je t'écoute...")
		audio = r.listen(source, phrase_time_limit=5)
		wav_buffer = BytesIO(audio.get_wav_data())
		son = AudioSegment.from_file(wav_buffer, format="wav")
		print("🔊 Lecture en cours...")
		play(son)
	try:        
		text = r.recognize_google(audio, language="fr-FR")        
		return text
	except sr.UnknownValueError as err:		
		return  f"❌ Erreur : {err.__class__.__name__} – {err}"

if __name__ == "__main__":
	print(voice_to_text())