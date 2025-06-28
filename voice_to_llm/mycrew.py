from dotenv import load_dotenv
from typing import Type
from pydantic import BaseModel, Field
from crewai import Agent, Task, Crew
from langchain_openai import ChatOpenAI
from crewai.tools import BaseTool

load_dotenv()

class FirstInput(BaseModel):
	pass
	
class FirstTool(BaseTool):
	name : str = "first_tool"
	description : str = "execute une fonction de test"
	
	def _run(self):
		print("la fonction est execute")

agent = Agent(
	role="testeur",
	goal="executer le tool ou pas selon si luser te dis crapaux ds son prompt tu utilise le tool sinon tu dis juste non!",
	backstory="tu es la pour un test fait ton but",
	tools=[FirstTool()],
	llm=ChatOpenAI(model="gpt-4o"),
	verbose=True 
)

task = Task(
	description="j'aime les crapaux",
	expected_output="une phrase tres courte ou execute juste le tool",
	agent=agent
)

import voice as v
class VoiceTool(BaseTool):
	name : str = "voice_tool"
	description : str = "get the voice from microphone and tranlate to text"

	def _run(self):
		return v.voice_to_text()

input_agent = Agent(
	role="get new task",
	goal="use tool to get the text and send back",
	backstory="you are un lucky agent who just have to use the tool ans return the text",
	tools=[VoiceTool()],
	llm=ChatOpenAI(model="gpt-4o"),
	verbose=True
)

voice_task = Task(
	description="get user input",
	expected_output="",
	agent=input_agent
)

agent_1 = Agent(
	role="membre d'une conversation random",
	goal="simply discuss with the other agent of anything",
	backstory="you are un agent who just have to discuss with other agent, you are a chatbot",	
	llm=ChatOpenAI(model="gpt-4o"),
	verbose=True
)

agent_2 = Agent(
	role="membre d'une conversation random",
	goal="simply discuss with the other agent of birds",
	backstory="you are un agent who just have to discuss with other agent, you are a chatbot",	
	llm=ChatOpenAI(model="gpt-4o"),
	verbose=True
)

task_1 = Task(
	description="start and continue a conversation in french with an other agent, max ten messages in all the conv",
	expected_output="",
	agent=agent_1
)

agent_manager = Agent(
	role="linker",
	goal="handle a conversation by passing messages",
	backstory="you are un agent who handle a conversation between agent_1 and agent_2, you have to link their reponse to each other. its must be a real conversation between the agent like a pong with  ten max message in all the conversation",	
	llm=ChatOpenAI(model="gpt-4o"),	
	verbose=True
)

mycrew = Crew(
	agents=[input_agent],
	tasks=[voice_task],
	verbose=True,
)

result = mycrew.kickoff()
print(result)