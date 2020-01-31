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
	void AddPlayer(vec3 pos, int controllerID, int modelId, vec3 color);
	void RemovePlayer(int controllerID);
	void AddPlatform(int modelId);
	void RemovePlatform();
	void AddPowerUp();
	void RemovePowerUp();
	vector<ObjectInfo*> GetObjects();
};

