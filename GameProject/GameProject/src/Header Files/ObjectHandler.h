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

	//Variables 4 Fysik
	btAlignedObjectArray<btCollisionShape*> m_collisionShapes; //bilar && platform [i framtiden powerups] Möjligen överflödig
	btBroadphaseInterface* m_broadphase;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher;
	btSequentialImpulseConstraintSolver* m_solver;
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	DebugDrawer* m_debugDrawer;

public:
	ObjectHandler();
	~ObjectHandler();

	void Update(float dt);
	void AddPlayer(vec3 pos, int controllerID, int modelId, vec3 color, Model* model);
	void RemovePlayer(int controllerID);
	void AddPlatform(int modelId, Model* model);
	void RemovePlatform();
	void AddPowerUp();
	void RemovePowerUp();
	vector<ObjectInfo*> GetObjects();
	btDiscreteDynamicsWorld* GetWorld();
	DebugDrawer* GetDebugDrawer();
};
