#pragma once
#include "Include.h"
#include "Player.h"
#include "Platform.h"

class ObjectHandler
{
private:
	vector<Player*> m_players;
	vector<Platform*> m_platforms;
	//vector<PowerUp*> m_powerUps;
public:
	ObjectHandler();
	~ObjectHandler();

	void Update(float dt);
	void AddPlayer(int controllerID);
	void RemovePlayer(int controllerID);
	void AddPlatform();
	void RemovePlatform();
	void AddPowerUp();
	void RemovePowerUp();
	vector<Object*> GetObjects();
};

ObjectHandler::ObjectHandler()
{

}

ObjectHandler::~ObjectHandler()
{

}

void ObjectHandler::Update(float dt)
{
	//Ta emot controller data (fr�n Game eller h�r?) och skicka vidare till r�tt Player.
}

void ObjectHandler::AddPlayer(int controllerID)
{
	m_players.push_back(new Player);
	m_players.back()->SetControllerID(controllerID);
}

void ObjectHandler::RemovePlayer(int controllerID)
{
	for (int i = 0; i < m_players.size; i++)
	{
		if (m_players[i]->GetControllerID() == controllerID)
		{
			m_players.erase(m_players.begin(), m_players.begin() + i);
		}
	}
}

void ObjectHandler::AddPlatform()
{
	m_platforms.push_back(new Platform);
}

void ObjectHandler::RemovePlatform()
{
	// Kommer aldrig anropas?
}

void ObjectHandler::AddPowerUp()
{
	// to be continued
}

void ObjectHandler::RemovePowerUp()
{
	// to be continued
}

vector<Object*> ObjectHandler::GetObjects()
{
	vector<Object*> temp;

	for (int i = 0; i < m_players.size; i++)
	{
		temp.push_back(m_players[i]->GetObject());
	}

	for (int i = 0; i < m_platforms.size; i++)
	{
		//temp.push_back(m_platforms[i]->GetObject());
	}

	//for (int i = 0; i < m_powerUps.size; i++)
	//{
	//	  temp.push_back(m_powerUps[i]->GetObject());
	//}

	return temp;
}