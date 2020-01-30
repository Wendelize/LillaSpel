#include "Header Files/Include.h"
#include "Header Files/ObjectHandler.h"

ObjectHandler::ObjectHandler()
{

}

ObjectHandler::~ObjectHandler()
{

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
	m_players.push_back(new Player);
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

vector<ObjectInfo*> ObjectHandler::GetObjects()
{
	vector<ObjectInfo*> temp;

	for (uint i = 0; i < m_players.size(); i++)
	{
		temp.push_back(m_players[i]->GetObjectInfo());
	}

	for (uint i = 0; i < m_platforms.size(); i++)
	{
		//temp.push_back(m_platforms[i]->GetObject());
	}

	//for (int i = 0; i < m_powerUps.size; i++)
	//{
	//	  temp.push_back(m_powerUps[i]->GetObject());
	//}

	return temp;
}