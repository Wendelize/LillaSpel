#include "Header Files/Game.h"

Game::Game()
{
	m_objectHandler = new ObjectHandler();
	m_scene = new Scene();
	m_scene->Init();

	m_objectHandler->AddPlayer(0, 0); // (Controller ID, model ID)
	m_objectHandler->AddPlayer(1, 0);

	m_objectHandler->AddPlatform(0);
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
	m_scene->Render(m_objectHandler->GetObjects());
}

GLFWwindow* Game::GetWindow()
{
	return m_scene->GetWindow();
}

