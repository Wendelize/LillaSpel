#pragma once
#include "Include.h"
#include "Platform.h"
#include "Player.h"
#include "PowerUp.h"
#include "MarchingCubes.h"

class ObjectHandler
{
private:
	bool m_usedSpawns[20];
	btVector3 m_spawnpoints[20] = 
	{
	btVector3(2,3,2), btVector3(5,3,5),btVector3(-5,3,5), btVector3(9,3,9), btVector3(-9,3,9),
	btVector3(2,3,-2), btVector3(5,3,-5), btVector3(-5,3,-5), btVector3(9,3,-9), btVector3(-9,3,-9),
	btVector3(-2,3,2), btVector3(2,3,0), btVector3(-2,3,0), btVector3(5,3,0), btVector3(-5,3,0),
	btVector3(-2,3,-2), btVector3(0,3,2), btVector3(0,3,5), btVector3(0,3,-2), btVector3(0,3,0)
	};
	vector<Player*> m_players;
	vector<Platform*> m_platforms;
	vector<PowerUp*> m_powerUps;
	vector<ObjectInfo*> m_structs;
	vector<vec3> m_bombZone;
	ISoundEngine* m_soundEngine;

	const char* m_soundFiles[NRDEATHSOUNDS];

	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btBroadphaseInterface* m_broadphase;
	btCollisionDispatcher* m_dispatcher;
	btSequentialImpulseConstraintSolver* m_solver;
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	DebugDrawer* m_debugDrawer;
	btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
	btGhostPairCallback* m_ghostCallback;

	MarchingCubes* m_cube;

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
	vec3 GetPlayerPos(int index);
	mat4 GetPlayerMatrix(int index);
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
	void StopAllSound();
	vector<ObjectInfo*> GetObjects();
	btDiscreteDynamicsWorld* GetWorld();
	DebugDrawer* GetDebugDrawer();
	void AddDynamicPlatformMesh(MarchingCubes* cube);
	void RemoveDynamicPlatformMesh(MarchingCubes* cube);
	vector<vec3> GetBomb();
	void ClearBombs();
	int GetWinnerIndex();
	void UpdateVibration(float dt);
	void ClearHoles();
	MarchingCubes* GetCube();
	void CheckPowerUpCollision();
};
