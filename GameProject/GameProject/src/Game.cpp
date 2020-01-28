#include "Header Files/Game.h"

Game::Game()
{
	m_objectHandler = new ObjectHandler();
	// m_scene = new Scene();
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

}