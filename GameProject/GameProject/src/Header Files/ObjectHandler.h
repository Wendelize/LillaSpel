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
	vector<ObjectInfo*> m_structs;
public:
	ObjectHandler();
	~ObjectHandler();

	void Update(float dt);
	void AddPlayer(vec3 pos, int controllerID, int modelId, vec3 color);
	void SetScale(int id, vec3 scale);
	void RemovePlayer(int index);
	void AddPlatform(int modelId);
	void RemovePlatform();
	void AddPowerUp();
	void RemovePowerUp();
	int GetNumPlayers();
	vec3 GetPlayerDirection(int index);
	int GetPlayerControllerID(int index);
	void SetPlayerControllerID(int index, int id);
	int GetPlayerModelID(int index);
	void SetPlayerModelID(int index, int id);
	vec3 GetPlayerColor(int index);
	void SetPlayerColor(int index, vec3 color);
	vec3 GetPlayerScale(int index);
	void SetPlayerScale(int index, vec3 scale);
	vector<ObjectInfo*> GetObjects();
};

