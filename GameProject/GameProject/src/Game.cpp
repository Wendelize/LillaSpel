#include "Header Files/Game.h"

Game::Game()
{
	m_objectHandler = new ObjectHandler();
	m_scene = nullptr;
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