#include "Header Files/Game.h"

Game::Game()
{
	m_objectHandler = new ObjectHandler();
	m_scene = new Scene();
	m_scene->Init();
}

Game::~Game()
{
	delete m_objectHandler;
	delete m_scene;
}

void Game::Update()
{
}

void Game::Input()
{

}

void Game::Render()
{
	m_scene->Render(m_objectHandler->GetObjects());
}

GLFWwindow* Game::GetWindow()
{
	return m_scene->GetWindow();
}

