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
	vector<ObjectInfo*> m_temp;
public:
	ObjectHandler();
	~ObjectHandler();

	void Update(float dt);
	void AddPlayer(int controllerID, int modelId);
	void RemovePlayer(int controllerID);
	void AddPlatform(int modelId);
	void RemovePlatform();
	void AddPowerUp();
	void RemovePowerUp();
	vector<ObjectInfo*> GetObjects();
};

