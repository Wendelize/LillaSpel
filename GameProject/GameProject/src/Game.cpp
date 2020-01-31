#include "Header Files/Game.h"

Game::Game()
{
	m_objectHandler = new ObjectHandler();
	m_scene = new Scene();
	m_scene->Init();
	m_time = 0;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_objectHandler->AddPlayer(vec3(i, 0, j), 0, 0, vec3(i, j, j));
		}
	}

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

