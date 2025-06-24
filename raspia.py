from dotenv import load_dotenv
from crewai import Agent, Task, Crew
from langchain_openai import ChatOpenAI
from crewai.tools import tool

# Charger les variables d'environnement (clé OpenAI)
load_dotenv()

# 🛠️ Tool défini avec le décorateur officiel CrewAI
@tool("dire_bonjour")
def dire_bonjour(name: str) -> str:
    """Souhaite la bienvenue à une personne"""
    return f"Bonjour truc bidule {name} ! 😊"

# 🤖 Agent qui utilise le tool
agent = Agent(
    role="Agent d'accueil",
    goal="Saluer les gens gentiment avec leur prénom",
    backstory="Tu es un assistant chaleureux chargé d'accueillir tout le monde avec un sourire virtuel.",
    tools=[dire_bonjour],  # <- ton outil ici
    llm=ChatOpenAI(model="gpt-4o"),
    verbose=True
)

# 🧠 Tâche à accomplir
task = Task(
    description="Dis bonjour à John en utilisant le tool.",
    agent=agent
)

# 🚀 Lancement de l'équipe
crew = Crew(
    agents=[agent],
    tasks=[task],
    verbose=2
)

# 🎬 Exécution
result = crew.kickoff()
print(f"\n✅ Résultat final : {result}")
