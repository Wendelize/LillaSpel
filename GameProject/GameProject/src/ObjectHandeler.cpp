#include "Header Files/Include.h"
#include "Header Files/ObjectHandler.h"

ObjectHandler::ObjectHandler()
{

}

ObjectHandler::~ObjectHandler()
{
	for (int i = 0; i < m_temp.size(); i++)
	{
		delete m_temp.at(i);
	}
	m_temp.clear();

	for (int i = 0; i < m_players.size(); i++)
	{
		delete m_players.at(i);
	}
	m_players.clear();

	for (int i = 0; i < m_platforms.size(); i++)
	{
		delete m_platforms.at(i);
	}
	m_platforms.clear();
}

void ObjectHandler::Update(float dt)
{
	for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->Update(dt);
	}
}

void ObjectHandler::AddPlayer(int controllerID, int modelId)
{
	m_players.push_back(new Player());
	m_players.back()->SetControllerID(controllerID);
	m_players.back()->SetModelId(modelId);
}

void ObjectHandler::RemovePlayer(int controllerID)
{
	for (uint i = 0; i < m_players.size(); i++)
	{
		if (m_players[i]->GetControllerID() == controllerID)
		{
			m_players.erase(m_players.begin(), m_players.begin() + i);
		}
	}
}

void ObjectHandler::AddPlatform(int modelId)
{
	m_platforms.push_back(new Platform);
	m_platforms.back()->SetModelId(modelId);
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

vector<ObjectInfo*> ObjectHandler::GetObjects()
{
	for (int i = 0; i < m_temp.size(); i++)
	{
		delete m_temp.at(i);
	}
	m_temp.clear();

	for (uint i = 0; i < m_players.size(); i++)
	{
		m_temp.push_back(m_players[i]->GetObjectInfo());
	}

	for (uint i = 0; i < m_platforms.size(); i++)
	{
		m_temp.push_back(m_platforms[i]->GetObjectInfo());
	}

	//for (int i = 0; i < m_powerUps.size; i++)
	//{
	//	  temp.push_back(m_powerUps[i]->GetObject());
	//}

	return m_temp;
}