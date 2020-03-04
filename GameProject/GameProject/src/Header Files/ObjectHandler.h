#pragma once
#include "Include.h"
#include "Platform.h"
#include "Player.h"
#include "PowerUp.h"
#include "Ghost.h"
#include "MarchingCubes.h"
#include "Object.h"

class ObjectHandler
{
private:
	bool m_usedSpawns[20];
	bool m_collision;
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
	vector<Ghost*> m_ghosts;
	vector<Object*> m_objects;
	vector<ObjectInfo*> m_structs;
	vector<Light*> m_carLights;
	vector<vec3> m_bombZone;
	vector<ParticleSystem*> m_particles;
	// used to track collisions for statsMenu
	bool m_collisionHappened = false;
	bool m_death = false;
	bool m_spawnBall = false;
	int m_aCollisionPlayerId = -1;
	int m_bCollisionPlayerId = -1;
	int m_dead = -1;
	int m_activeBombs = 0;
	float m_collidTimer = 0;
	float m_scaleTimer = 0;
	// uses controller id
	vector <int> m_deathOrder;
	
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

	bool m_lightsOut;

public:
	ObjectHandler();
	~ObjectHandler();

	void Update(float dt);
	void AddPlayer(vec3 pos, int controllerID, int modelId, vec3 color, Model* model);
	void AddObject(vec3 pos, int modelId, Model* model);
	void RemoveObject(int index);
	void AddPlatform(int modelId, Model* model);
	void AddGhost(int index);
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
	int GetACollisionId();
	int GetBCollisionId();
	bool GetCollisionHappened();
	void SetCollisionHappened(bool setFalse = false);
	bool GetDeath();
	void SetDeath(bool setFalse = false);
	bool GetSpawnBall();
	void SetSpawnBall(bool spawn);
	// also sets dead to -1;
	int GetDeadId();
	// also clears the vector
	vector<int> GetDeathOrder();
	// TEMP!! TODO: REMOVE!!!!
	int GetPlayerControllerIDBloo(int index, int bloo);

	bool GetExplosion();
	void SetExplosion(bool b);
	vec3 GetExplosionPosition();

	void StopAllSound();
	vector<ObjectInfo*> GetObjects();
	vector<Light*> GetLights();
	vector<ParticleSystem*> particles();
	btDiscreteDynamicsWorld* GetWorld();
	DebugDrawer* GetDebugDrawer();
	int GetWinnerID();
	void AddDynamicPlatformMesh(MarchingCubes* cube);
	void RemoveDynamicPlatformMesh(MarchingCubes* cube);
	vector<vec3> GetBomb();
	void ClearBombs();
	int GetWinnerIndex();
	void UpdateVibration(float dt);
	void ClearHoles();
	MarchingCubes* GetCube();
	void CheckPowerUpCollision();
	bool CheckCollisionCars(float dt);
	bool GetLightsOut();
	void SetLightsOut(bool state);
	void RenderParticles();
	void RemoveAllObjects();
};
