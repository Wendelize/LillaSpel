#pragma once
#include "Include.h"
#include "Platform.h"
#include "Player.h"
#include "PowerUp.h"

class ObjectHandler
{
private:
	bool m_usedSpawns[20];
	btVector3 m_spawnpoints[20] = 
	{
	btVector3(2,1,2), btVector3(5,1,5),btVector3(-5,1,5), btVector3(9,1,9), btVector3(-9,1,9),
	btVector3(2,1,-2), btVector3(5,1,-5), btVector3(-5,1,-5), btVector3(9,1,-9), btVector3(-9,1,-9),
	btVector3(-2,1,2), btVector3(2,1,0), btVector3(-2,1,0), btVector3(5,1,0), btVector3(-5,1,0),
	btVector3(-2,1,-2), btVector3(0,1,2), btVector3(0,1,5), btVector3(0,1,-2), btVector3(0,1,0)
	};
	vector<Player*> m_players;
	vector<Platform*> m_platforms;
	vector<PowerUp*> m_powerUps;
	vector<ObjectInfo*> m_structs;
	// used to track collisions for statsMenu
	bool m_collisionHappened = false;
	bool m_death = false;
	int m_aCollisionPlayerId = -1;
	int m_bCollisionPlayerId = -1;
	int m_dead = -1;
	float m_collidTimer = 0;
	// uses controller id
	vector <int> m_deathOrder;

	ISoundEngine* m_soundEngine;
	vector<ISoundSource*> m_crashes;

	const char* m_soundFiles[NRDEATHSOUNDS];

	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btBroadphaseInterface* m_broadphase;
	btCollisionDispatcher* m_dispatcher;
	btSequentialImpulseConstraintSolver* m_solver;
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	DebugDrawer* m_debugDrawer;
	btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
	btGhostPairCallback* m_ghostCallback;
public:
	ObjectHandler();
	~ObjectHandler();

	void Update(float dt);
	void AddPlayer(vec3 pos, int controllerID, int modelId, vec3 color, Model* model);
	void AddPlatform(int modelId, Model* model);
	void SetScale(int id, vec3 scale);
	void RemovePlayer(int index);
	void RemovePlatform();
	void AddPowerUp();
	int GetNumPowerUps();
	void RemovePowerUp(int index);
	int GetNumPlayers();
	int GetPlayerLives(int index);
	void SetNumberOfLives(int num);
	vec3 GetPlayerDirection(int index);
	int GetPlayerControllerID(int index);
	void SetPlayerControllerID(int index, int id);
	int GetPlayerModelID(int index);
	void SetPlayerModelID(int index, int id);
	vec3 GetPlayerColor(int index);
	void SetPlayerColor(int index, vec3 color);
	vec3 GetPlayerScale(int index);
	void SetPlayerScale(int index, vec3 scale);
	int GetACollisionId();
	int GetBCollisionId();
	bool GetCollisionHappened();
	void SetCollisionHappened(bool setFalse = false);
	bool GetDeath();
	void SetDeath(bool setFalse = false);
	// also sets dead to -1;
	int GetDeadId();
	// also clears the vector
	vector<int> GetDeathOrder();
	// TEMP!! TODO: REMOVE!!!!
	int GetPlayerControllerIDBloo(int index, int bloo);

	void StopAllSound();
	vector<ObjectInfo*> GetObjects();
	btDiscreteDynamicsWorld* GetWorld();
	DebugDrawer* GetDebugDrawer();
	int GetWinnerID();
	void UpdateVibration(float dt);

};
