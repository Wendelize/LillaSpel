#include "Header Files/Game.h"

Game::Game()
{
	m_objectHandler = new ObjectHandler();
	m_scene = new Scene();
	m_scene->Init(); // H�r skapas modellerna
	m_time = 0;

	vector<Model*> platforms = m_scene->GetModels(0);
	vector<Model*> cars = m_scene->GetModels(1); 

	m_objectHandler->AddPlatform(0, platforms[0]); // Passa modell

	m_objectHandler->AddPlayer(vec3(0, 7, 2), 0, 0, vec3(0, 0, 1), cars[0]); // Passa modell
	m_objectHandler->AddPlayer(vec3(0, 7, -2), 1, 0, vec3(0, 1, 0), cars[0]); // Passa modell
}

Game::~Game()
{
	delete m_objectHandler;
	delete m_scene;
}

void Game::Update(float dt)
{
	m_time += dt;

	m_objectHandler->Update(dt);

	Render();
}

void Game::Render()
{
	m_scene->Render(m_objectHandler->GetObjects(), m_objectHandler->GetWorld());
}

GLFWwindow* Game::GetWindow()
{
	return m_scene->GetWindow();
}

