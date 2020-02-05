#include "Header Files/Game.h"

Game::Game()
{
	m_objectHandler = new ObjectHandler();
	m_scene = new Scene();
	m_scene->Init();
	m_time = 0;

	m_objectHandler->AddPlatform(0);

	m_objectHandler->AddPlayer(vec3(0, 6, 2), 0, 0, vec3(0, 0, 1));
	m_objectHandler->AddPlayer(vec3(0, 6, -2), 1, 0, vec3(0, 1, 0));
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

