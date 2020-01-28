#pragma once
#include "Include.h"
#include "Platform.h"
#include "Player.h"

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
	vector<ObjectInfo*> GetObjects();
};

