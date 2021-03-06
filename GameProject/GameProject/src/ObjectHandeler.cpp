#include "Header Files/Include.h"
#include "Header Files/ObjectHandler.h"

inline void sleepSomeTime() { Sleep(100); }

ObjectHandler::ObjectHandler()
{
	m_broadphase = new btDbvtBroadphase();
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_solver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0, -10.f, 0));
	m_debugDrawer = new DebugDrawer;
	m_debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_dynamicsWorld->setDebugDrawer(m_debugDrawer);
	m_ghostCallback = new btGhostPairCallback();
	m_dynamicsWorld->getPairCache()->setInternalGhostPairCallback(m_ghostCallback);
	m_lightsOut = false;

	m_soundEngine = createIrrKlangDevice();
	if (m_soundEngine)
	{
		m_soundEngine->setSoundVolume(1.3f);

		m_soundEngine->setListenerPosition(vec3df(0, 18, 33), vec3df(0, -4, 3)); // Listener position, view direction
		m_soundEngine->setDefault3DSoundMinDistance(70.0f);
	}

	m_soundFiles[0] = "src/Audio/Player - Dying 1.mp3";
	m_soundFiles[1] = "src/Audio/Player - Dying 2.mp3";
	m_soundFiles[2] = "src/Audio/Player - Dying 3.mp3";
	m_soundFiles[3] = "src/Audio/Player - Dying 4.mp3";
	m_soundFiles[4] = "src/Audio/Player - Dying 5.mp3";
	m_soundFiles[5] = "src/Audio/Player - Dying 6.mp3";
	m_soundFiles[6] = "src/Audio/Player - Dying 7.mp3";
	m_soundFiles[7] = "src/Audio/Player - Dying 8.mp3";
	m_soundFiles[8] = "src/Audio/Player - Dying 9.mp3";
	m_soundFiles[9] = "src/Audio/Player - Dying 10.mp3";
	m_soundFiles[10] = "src/Audio/Player - Dying 11.mp3";
	m_soundFiles[11] = "src/Audio/Player - Dying 12.mp3";
	m_soundFiles[12] = "src/Audio/Player - Dying 13.mp3";
	m_soundFiles[13] = "src/Audio/Player - Dying 14.mp3";
	m_soundFiles[14] = "src/Audio/Player - Dying 15.mp3";
	m_soundFiles[15] = "src/Audio/Player - Dying 16.mp3";
	m_soundFiles[16] = "src/Audio/Player - Dying 17.mp3";
	m_soundFiles[17] = "src/Audio/Player - Dying 18.mp3";
	m_soundFiles[18] = "src/Audio/Player - Dying 19.mp3";
	m_soundFiles[19] = "src/Audio/Player - Dying 20.mp3";
	m_soundFiles[20] = "src/Audio/Player - Dying 21.mp3";
	m_soundFiles[21] = "src/Audio/Player - Dying 22.mp3";
	m_soundFiles[22] = "src/Audio/Player - Dying 23.mp3";
	m_soundFiles[23] = "src/Audio/Player - Dying 24.mp3";
	m_soundFiles[24] = "src/Audio/Player - Dying 25.mp3";
	m_soundFiles[25] = "src/Audio/Player - Dying 26.mp3";
	m_soundFiles[26] = "src/Audio/Player - Dying 27.mp3";
	m_soundFiles[27] = "src/Audio/Player - Dying 28.mp3";
	m_soundFiles[28] = "src/Audio/Player - Dying 29.mp3";
	m_soundFiles[29] = "src/Audio/Player - Dying 30.mp3";
	m_terrain = true;
}

ObjectHandler::~ObjectHandler()
{
	for (uint i = 0; i < m_players.size(); i++)
	{
		delete m_players.at(i);
	}
	m_players.clear();

	for (uint i = 0; i < m_platforms.size(); i++)
	{
		delete m_platforms.at(i);
	}
	for (uint i = 0; i < m_objects.size(); i++)
	{
		delete m_objects.at(i);
	}
	for (uint i = 0; i < m_oribtObjects.size(); i++)
	{
		delete m_oribtObjects.at(i);
	}
	int temp = m_powerUps.size();
	for (uint i = 0; i < temp; i++)
	{
		RemovePowerUp(0);
	}
	m_powerUps.clear();
	m_platforms.clear();

	for (uint i = 0; i < m_ghosts.size(); i++)
	{
		delete m_ghosts.at(i);
	}
	m_ghosts.clear();

	for (size_t i = 0; i < m_structs.size(); i++)
	{
		delete m_structs.at(i);
	}
	m_structs.clear();

	for (size_t i = 0; i < m_carLights.size(); i++)
	{
		delete m_carLights.at(i);
	}
	m_carLights.clear();

	for (size_t i = 0; i < m_objLights.size(); i++)
	{
		delete m_objLights.at(i);
	}
	m_objLights.clear();

	for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];

		btRigidBody* body = btRigidBody::upcast(obj);


		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	delete m_ghostCallback;
	//delete dynamics world
	delete m_dynamicsWorld;

	//delete solver
	delete m_solver;

	//delete broadphase
	delete m_broadphase;

	//delete dispatcher
	delete m_dispatcher;

	delete m_collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	m_collisionShapes.clear();

	delete m_debugDrawer;
}

void ObjectHandler::Update(float dt)
{
	const char* filename;
	int randomNumber = rand() % NRDEATHSOUNDS;

	m_dynamicsWorld->stepSimulation(dt, 10);

	m_scaleTimer += dt;
	if (m_cube->GetCurrentLevel() == 5)
	{
		if (m_objects.size() != 0)
		{
			if (m_objects.at(0)->GetPos().z < -21.0f && m_objects.at(0)->GetWay())
			{
				m_objects.at(0)->SetWay(false);
			}
			else if (m_objects.at(0)->GetPos().z > 21.0f && !m_objects.at(0)->GetWay())
			{
				m_objects.at(0)->SetWay(true);
			}
			if (m_objects.at(0)->GetWay())
			{
				m_objects.at(0)->Move(vec3(0, 0, -5));
			}
			else
			{
				m_objects.at(0)->Move(vec3(0, 0, 5));
			}
		}
	}
	CheckPowerUpCollision();
	CheckCollisionCars(dt);
	UpdateVibration(dt);

	UpdateHook(dt);
	UpdateLastPos();

	//LightsOut power-up update
	bool active = false;
	for (auto p : m_players)
	{
		if (p->GetActivePower() == 7)
		{
			active = true;
			m_lightsOut = true;
		}
	}
	if (m_lightsOut == true && active == false)
	{
		m_lightsOut = false;
	}

	//Invisible terrain 
	active = false;
	for (auto p : m_players)
	{
		if (p->GetActivePower() == 9) //Questionmark 
		{
			active = true;
			m_terrain = false;
		}
	}
	if (m_terrain == false && active == false)
	{
		m_terrain = true;
	}


	for (size_t i = 0; i < m_dynamicsWorld->getNumCollisionObjects(); i++)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;

		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		int nrOfPlatforms = 1;
		int isPlayer = i - nrOfPlatforms - m_objects.size() - m_powerUps.size() - m_oribtObjects.size();
		int isPowerUp = i - nrOfPlatforms - m_objects.size() - m_players.size() - m_oribtObjects.size();
		int isObject = i - nrOfPlatforms - m_powerUps.size() - m_players.size() - m_oribtObjects.size();

		if (i < m_oribtObjects.size())
		{
			m_oribtObjects.at(i)->rotateAroundCenter(dt);
		}

		if (isObject >= 0)
		{
			m_objects.at(isObject)->Update();
		}

		// GHOSTS
		if (i < m_ghosts.size())
		{
			m_ghosts.at(i)->UpdateGhost(dt);
			for (uint j = 0; j < m_ghosts.size(); j++)
			{
				if (m_ghosts.at(j)->GetLightSwitch())
				{
					m_lightsOut = true;
				}

				if (m_ghosts.at(j)->GetBombSwitch())
				{
					int width = m_cube->GetCurrentWidth() - 10;
					if (width < 2)
					{
						width = 2;
					}
					cout << width << endl;
					for (int i = 0; i < 1; i++)
					{
						vec3 temp = vec3(rand() % width - (width / 2), 0, rand() % width - (width / 2));
						float height = m_cube->GetHeight(temp);
						temp.y = height;

						cout << temp.x << ", " << temp.y << ", " << temp.z << endl;
						m_bombZone.push_back(temp);
					}
					m_ghosts.at(j)->SetBombSwitch(false);
				}

				if (m_ghosts.at(j)->GetCtrlSwitch())
				{
					for (uint j = 0; j < m_players.size(); j++)
					{
						m_players.at(j)->GivePower(1);
					}
				}
			}
		}

		if (isPowerUp >= 0)
		{
			if (m_powerUps[isPowerUp]->update(dt))
			{
				RemovePowerUp(isPowerUp);
			}
		}

		if (isPlayer >= 0)
		{
			m_players[isPlayer]->Update(dt);
			if (m_players[isPlayer]->updatePower(dt))
			{
				m_dynamicsWorld->removeRigidBody(m_players[isPlayer]->GetBody());
				m_players[isPlayer]->removePower(m_players[isPlayer]->GetActivePower());
				m_dynamicsWorld->addRigidBody(m_players[isPlayer]->GetBody());
			}

			if (m_soundEngine)
			{
				if (m_players[isPlayer]->GetCurrentPos().y() < -0.1f && !m_players[isPlayer]->GetFallen())
				{
					filename = m_soundFiles[randomNumber];
					m_soundEngine->play2D(filename, false);
					m_players[isPlayer]->SetFallen();
					m_players[isPlayer]->StopEngineSounds();
				}
				// DEATH -> DO WE WANT BOX?
				vec3 currentPosition = vec3(m_players[isPlayer]->GetCurrentPos().x(), m_players[isPlayer]->GetCurrentPos().y(), m_players[isPlayer]->GetCurrentPos().z());
				if ((currentPosition.y < -20.f || currentPosition.x < -75.f || currentPosition.x > 75.f || currentPosition.z < -75.f || currentPosition.z > 65.f) && m_players[isPlayer]->GetLives() > 0)
				{
					bool spawnFound = false;
					vec3 spawn = vec3(0); // = vec3((rand() % 30) - 15, 7, (rand() % 20 - 15)));
					while (!spawnFound)
					{
						spawn = vec3((rand() % 15) - 7, 7, (rand() % 15 - 7));
						if (m_cube->IsNotHole(spawn))
						{
							spawnFound = true;
						}
					}
					m_players[isPlayer]->SetPos(spawn);
					m_players[isPlayer]->ReduceLife();
					m_players[isPlayer]->SetBoolLights(true);
					m_players[isPlayer]->SetNotFallen();
					m_soundEngine->setSoundVolume(1.4f);
					m_soundEngine->play2D("src/Audio/Powerup - Spawn.mp3", false);
					m_soundEngine->setSoundVolume(0.6f);
					m_players[isPlayer]->StartEngineSounds();
					// used for statsMenu
					m_death = true;
					m_dead = m_players[isPlayer]->GetControllerID();
				}
				else if (m_players[isPlayer]->GetLives() == 0)
				{
					AddGhost(m_players[isPlayer]->GetControllerID());
					m_deathOrder.push_back(m_players[isPlayer]->GetControllerID());
					RemovePlayer(isPlayer);
					isPlayer--;
				}
			}
			else
			{
				if (m_players[isPlayer]->GetCurrentPos().y() < -20.f && m_players[isPlayer]->GetLives() > 0)
				{
					bool spawnFound = false;
					vec3 spawn = vec3(0); // = vec3((rand() % 30) - 15, 7, (rand() % 20 - 15)));
					while (!spawnFound)
					{
						spawn = vec3((rand() % 15) - 7, 7, (rand() % 15 - 7));
						if (m_cube->IsNotHole(spawn))
						{
							spawnFound = true;
						}
					}
					m_players[isPlayer]->SetPos(spawn);
					m_players[isPlayer]->ReduceLife();
					m_players[isPlayer]->SetBoolLights(true);
					// used for statsMenu
					m_death = true;
					m_dead = m_players[isPlayer]->GetControllerID();
				}
				else
				{
					if (m_players[isPlayer]->GetLives() == 0)
					{
						// used for statsMenu
						AddGhost(m_players[isPlayer]->GetControllerID());
						m_deathOrder.push_back(m_players[isPlayer]->GetControllerID());
						RemovePlayer(isPlayer);
						isPlayer--;
					}
				}
			}
		}
	}
}

void ObjectHandler::AddPlayer(vec3 pos, int controllerID, int modelId, vec3 color, Model* model)
{
	// Till players konstruktor skall meshens vertices passas + ID f�r att identifiera vilken modell som tillh�r objektet!
	m_players.push_back(new Player(model, modelId, pos));
	m_players.back()->SetControllerID(controllerID);
	m_players.back()->GetBody()->setUserIndex(controllerID);
	m_players.back()->SetModelId(modelId);
	m_players.back()->SetColor(color);

	m_dynamicsWorld->addRigidBody(m_players.back()->GetBody());

	if (m_soundEngine)
		m_players.back()->StartEngineSounds();
}

void ObjectHandler::AddObject(vec3 pos, int modelId, Model* model, float scale)
{
	m_objects.push_back(new Object(btVector3(pos.x, pos.y, pos.z), modelId, model, 0, scale));
	m_dynamicsWorld->addRigidBody(m_objects.back()->GetObject());
}

void ObjectHandler::AddOrbitObjects(vec3 pos, int modelId, Model* model, float speed, float scale)
{
	m_oribtObjects.push_back(new Object(btVector3(pos.x, pos.y, pos.z), modelId, model, speed, scale));
	m_dynamicsWorld->addRigidBody(m_oribtObjects.back()->GetObject());
}

void ObjectHandler::RemoveOrbitObjects(int index)
{
	m_dynamicsWorld->removeCollisionObject(m_oribtObjects.at(index)->GetObject());
	btRigidBody* body = m_oribtObjects.at(index)->GetObject();
	delete m_oribtObjects.at(index);
	m_oribtObjects.erase(m_oribtObjects.begin() + index);
	if (body && body->getMotionState())
	{
		delete body->getMotionState();
	}
	delete body;
}

void ObjectHandler::RemoveAllOrbitObjects()
{
	int size = m_oribtObjects.size();
	for(int i = 0; i < size; i ++)
	{
		RemoveOrbitObjects(0);
	}
}

void ObjectHandler::RemoveObject(int index)
{
	m_dynamicsWorld->removeCollisionObject(m_objects.at(index)->GetObject());
	btRigidBody* body = m_objects.at(index)->GetObject();
	delete m_objects.at(index);
	m_objects.erase(m_objects.begin() + index);
	if (body && body->getMotionState())
	{
		delete body->getMotionState();
	}
	delete body;
}

void ObjectHandler::SetScale(int id, vec3 scale)
{
	m_players.at(id)->SetScale(scale);
}

void ObjectHandler::SetPlayerFinishRotation(int index)
{
	m_players[index]->FinishRotation();
}

void ObjectHandler::RemovePlayer(int index)
{
	m_players[index]->StopEngineSounds();
	m_dynamicsWorld->removeCollisionObject(m_players.at(index)->GetBody());
	btRigidBody* body = m_players.at(index)->GetBody();
	delete m_players.at(index);
	m_players.erase(m_players.begin() + index);

	if (body && body->getMotionState())
	{
		delete body->getMotionState();
	}
	delete body;
}

void ObjectHandler::AddPlatform(int modelId, Model* model)
{
	// Till platforms konstruktor skall meshens vertices passas + ID f�r att identifiera vilken modell som tillh�r objektet!
	m_platforms.push_back(new Platform(model, modelId));
	m_platforms.back()->SetModelId(modelId);

	m_dynamicsWorld->addRigidBody(m_platforms.back()->getBody(), 1, -1);
}

void ObjectHandler::AddGhost(int index)
{
	m_ghosts.push_back(new Ghost());
	m_ghosts.back()->SetControllerID(index);
}

void ObjectHandler::AddPowerUp()
{
	int type = rand() % (11);
	bool spawnFound = false;
	vec3 spawn = vec3(0); // = vec3((rand() % 30) - 15, 7, (rand() % 20 - 15)));
	while (!spawnFound)
	{
		spawn = vec3((rand() % m_cube->GetWidth()) - m_cube->GetWidth() / 2, 7,
		             (rand() % m_cube->GetWidth()) - m_cube->GetWidth() / 2);
		if (m_cube->IsNotHole(spawn))
		{
			spawnFound = true;
			m_cube->GetHeight(spawn);
		}
	}

	btVector3 spawnPoint = btVector3(spawn.x, m_cube->GetHeight(spawn) + 10.f, spawn.z);
	m_powerUps.push_back(new PowerUp(0, spawnPoint, type));
	m_dynamicsWorld->addRigidBody(m_powerUps.back()->getObject());

	if (m_soundEngine)
	{
		m_soundEngine->setSoundVolume(1.4f);
		m_soundEngine->play2D("src/Audio/Powerup - Spawn.mp3", false);
		m_soundEngine->setSoundVolume(0.6f);
	}
}

int ObjectHandler::GetNumPowerUps()
{
	return m_powerUps.size();
}

void ObjectHandler::RemovePowerUp(int index)
{
	m_dynamicsWorld->removeCollisionObject(m_powerUps.at(index)->getObject());
	m_usedSpawns[m_powerUps.at(index)->GetSpawn()] = false;
	ObjectInfo* temp = m_powerUps.at(index)->GetObjectInfo();

	for (int i = 0; i < m_structs.size(); i++)
	{
		if (m_structs.at(i)->modelMatrix == temp->modelMatrix && m_structs.at(i)->typeId == temp->typeId)
		{
			delete m_structs.at(i);
			m_structs.at(i) = new ObjectInfo(mat4(0), 1, 1, vec3(0), false);
		}
	}

	delete temp;

	delete m_powerUps.at(index)->getObject()->getMotionState();
	delete m_powerUps.at(index)->getObject();
	delete m_powerUps.at(index)->GetObjectInfo();
	delete m_powerUps.at(index);
	m_powerUps.erase(m_powerUps.begin() + index);
}

int ObjectHandler::GetNumPlayers()
{
	return m_players.size();
}

int ObjectHandler::GetPlayerLives(int index)
{
	return m_players[index]->GetLives();
}

vec3 ObjectHandler::GetPlayerPos(int index)
{
	btVector3 temp = m_players[index]->GetCurrentPos();
	return vec3(temp.getX(), temp.getY(), temp.getZ());
}

void ObjectHandler::SetPlayerPos(vec3 pos, int index)
{
	m_players[index]->SetFinishPos(pos);
}

void ObjectHandler::SetWholePlayerPos(vec3 pos, int index)
{
	m_players[index]->SetPos(pos);
}

void ObjectHandler::RotatePlayer(int index, float speed)
{
	m_players[index]->Rotate(speed);
}

void ObjectHandler::SetPlayerSpotlights(bool b)
{
	for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->SetBoolLights(b);
	}
}

mat4 ObjectHandler::GetPlayerMatrix(int index)
{
	return m_players[index]->GetMatrix();
}

void ObjectHandler::SetNumberOfLives(int num)
{
	for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->SetLives(num);
	}
}

vec3 ObjectHandler::GetPlayerDirection(int index)
{
	return m_players[index]->GetDirection();
}

int ObjectHandler::GetPlayerControllerID(int index)
{
	return m_players[index]->GetControllerID();
}

bool ObjectHandler::GetExplosion()
{
	return m_cube->GetExplosion();
}

void ObjectHandler::SetExplosion(bool b)
{
	m_cube->SetExplosion(b);
}

vec3 ObjectHandler::GetExplosionPosition()
{
	return m_cube->GetExplosionPosition();
}

void ObjectHandler::SetPlayerControllerID(int index, int id)
{
	m_players[index]->SetControllerID(id);
}

int ObjectHandler::GetIndexByControllerId(int controllerId)
{
	for (int i = 0; i < m_players.size(); i++)
	{
		if (m_players[i]->GetControllerID() == controllerId)
		{
			return i;
		}
	}

	return -1;
}

int ObjectHandler::GetPlayerModelID(int index)
{
	return m_players[index]->GetModelId();
}

void ObjectHandler::SetPlayerModelID(int index, int id)
{
	m_players[index]->SetModelId(id);
}

float ObjectHandler::GetPlayerSpeed(int index)
{
	return m_players[index]->GetSpeed();
}

vec3 ObjectHandler::GetPlayerColor(int index)
{
	return m_players[index]->GetColor();
}

void ObjectHandler::SetPlayerColor(int index, vec3 color)
{
	m_players[index]->SetColor(color);
}

vec3 ObjectHandler::GetPlayerScale(int index)
{
	return m_players[index]->GetScale();
}

void ObjectHandler::SetPlayerScale(int index, vec3 scale)
{
	m_players[index]->SetScale(scale);
}

int ObjectHandler::GetACollisionId()
{
	return m_aCollisionPlayerId;
}

int ObjectHandler::GetBCollisionId()
{
	return m_bCollisionPlayerId;
}

bool ObjectHandler::GetCollisionHappened()
{
	return m_collisionHappened;
}

void ObjectHandler::SetCollisionHappened(bool setfalse)
{
	m_collisionHappened = setfalse;
}

bool ObjectHandler::GetDeath()
{
	return m_death;
}

void ObjectHandler::SetDeath(bool setFalse)
{
	m_death = setFalse;
}

bool ObjectHandler::GetSpawnBall()
{
	return m_spawnBall;
}

void ObjectHandler::SetSpawnBall(bool spawn)
{
	m_spawnBall = spawn;
}

int ObjectHandler::GetDeadId()
{
	int temp = m_dead;
	m_dead = -1;
	return temp;
}

vector<int> ObjectHandler::GetDeathOrder()
{
	vector<int> temp = m_deathOrder;
	m_deathOrder.clear();
	return temp;
}

void ObjectHandler::StopAllSound()
{
	m_soundEngine->stopAllSounds();
	for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->StopEngineSounds();
	}
}

vector<ObjectInfo*> ObjectHandler::GetObjects()
{
	for (size_t i = 0; i < m_structs.size(); i++)
	{
		delete m_structs.at(i);
	}
	m_structs.clear();

	for (uint i = 0; i < m_players.size(); i++)
	{
		m_structs.push_back(m_players[i]->GetObjectInfo());
	}
	for (uint i = 0; i < m_platforms.size(); i++)
	{
		m_structs.push_back(m_platforms[i]->GetObjectInfo());
	}
	for (int i = 0; i < m_powerUps.size(); i++)
	{
		m_structs.push_back(m_powerUps[i]->GetObjectInfo());
	}
	for (int i = 0; i < m_objects.size(); i++)
	{
		m_structs.push_back(m_objects[i]->GetObjectInfo());
	}
	for (int i = 0; i < m_oribtObjects.size(); i++)
	{
		m_structs.push_back(m_oribtObjects[i]->GetObjectInfo());
	}
	return m_structs;
}

vector<Light*> ObjectHandler::GetLights()
{
	for (size_t i = 0; i < m_carLights.size(); i++)
	{
		delete m_carLights.at(i);
	}
	m_carLights.clear();

	for (uint j = 0; j < m_players.size(); j++)
	{
		if (m_players.at(j)->GetBoolLights())
		{
			m_carLights.push_back(m_players[j]->GetLight(0));
			m_carLights.push_back(m_players[j]->GetLight(1));
		}
	}


	return m_carLights;
}

vector<Light*> ObjectHandler::GetObjLight()
{
	for (size_t i = 0; i < m_objLights.size(); i++)
	{
		delete m_objLights.at(i);
	}
	m_objLights.clear();
	vector<Light*> temp;
	for (uint i = 0; i < m_objects.size(); i++)
	{
		temp = m_objects.at(i)->GetAllLight();
		if (temp.size() > 0)
		{
			for (uint j = 0; j < temp.size(); j++)
			{
				m_objLights.push_back(temp.at(j));
			}
		}
	}

	return m_objLights;
}

btDiscreteDynamicsWorld* ObjectHandler::GetWorld()
{
	return m_dynamicsWorld;
}

DebugDrawer* ObjectHandler::GetDebugDrawer()
{
	return m_debugDrawer;
}

void ObjectHandler::AddDynamicPlatformMesh(MarchingCubes* cube)
{
	m_cube = cube;
	m_dynamicsWorld->addRigidBody(cube->GetBody());
}

void ObjectHandler::RemoveDynamicPlatformMesh(MarchingCubes* cube)
{
	m_dynamicsWorld->removeRigidBody(cube->GetBody());
}

vector<vec3> ObjectHandler::GetBomb()
{
	return m_bombZone;
}

void ObjectHandler::ClearBombs()
{
	m_bombZone.clear();
}

int ObjectHandler::GetWinnerID()
{
	int winnerID = 0;
	int winnerLife = 0;
	for (auto w : m_players)
	{
		if (w->GetLives() > winnerLife)
		{
			winnerLife = w->GetLives();
			winnerID = w->GetControllerID();
		}
	}
	return winnerID;
}

void ObjectHandler::UpdateVibration(float dt)
{
	int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; ++i)
	{
		btPersistentManifold* contactManifold = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

		btCollisionShape* shapeA = obA->getCollisionShape();
		btCollisionShape* shapeB = obB->getCollisionShape();

		//Collision between spheres(cars)
		if (shapeA->getShapeType() == 8 && shapeB->getShapeType() == 8)
		{
			// used for statsMenu
			int oldAIndex = m_aCollisionPlayerId;
			int oldBIndex = m_bCollisionPlayerId;
			double time = glfwGetTime();

			for (int f = 0; f < m_players.size(); f++)
			{
				if (m_players.at(f)->GetBody() == obA)
				{
					m_players.at(f)->GetController()->Vibrate(m_players.at(f)->GetControllerID(), 6553, 65535);
					// used for statsMenu
					m_aCollisionPlayerId = m_players.at(f)->GetControllerID();
				}
				if (m_players.at(f)->GetBody() == obB)
				{
					m_players.at(f)->GetController()->Vibrate(m_players.at(f)->GetControllerID(), 6553, 65535);
					// used for statsMenu
					m_bCollisionPlayerId = m_players.at(f)->GetControllerID();
				}
			}
			// used for statsMenu
			if (!(time - m_collidTimer <= 0.5 && ((oldAIndex == m_aCollisionPlayerId && oldBIndex ==
				m_bCollisionPlayerId) || (oldBIndex == m_aCollisionPlayerId && oldAIndex == m_bCollisionPlayerId))))
			{
				// TODO: kanske om det fuckar för mycket när fera krockar samtidigt så fixa bättre lösning
				m_collisionHappened = true;
				m_collidTimer = time;
			}
		}
	}

	for (int f = 0; f < m_players.size(); f++)
	{
		if (m_players.at(f)->GetController()->GetVibrationTime() < 0.2)
		{
			m_players.at(f)->GetController()->AddVibrationTime(dt);
		}
		else
		{
			m_players.at(f)->GetController()->Vibrate(m_players.at(f)->GetControllerID(), 0, 0);
		}
	}
}

void ObjectHandler::ClearHoles()
{
	m_cube->ClearHoles();
}

MarchingCubes* ObjectHandler::GetCube()
{
	return m_cube;
}

void ObjectHandler::CheckPowerUpCollision()
{
	int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; ++i)
	{
		//TODO
		//Powerup spawn on car, exception thrown
		btPersistentManifold* contactManifold = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

		btCollisionShape* shapeA = obA->getCollisionShape();
		btCollisionShape* shapeB = obB->getCollisionShape();


		//Collision between sphere(cars) && BOX (PowerUp)
		if (shapeA->getShapeType() == 8 && shapeB->getShapeType() == 0) // TYPE 0 = BOX , TYPE 8 = SPHERE
		{
			for (int f = 0; f < m_players.size(); f++)
			{
				if (m_players.at(f)->GetBody() == obA)
				{
					PUpCollision[f] = true;
					for (int k = 0; k < m_powerUps.size(); k++)
					{
						if (m_powerUps.at(k)->getObject() == obB)
						{
							if (m_powerUps.at(k)->GetType() == 5 || m_powerUps.at(k)->GetType() == 4 || m_powerUps
							                                                                            .at(k)->
							                                                                            GetType() == 1)
							{
								for (int l = 0; l < m_players.size(); l++)
								{
									if (l != f)
									{
										m_dynamicsWorld->removeRigidBody(m_players.at(l)->GetBody());
										m_players.at(l)->GivePower(m_powerUps.at(k)->GetType());

										if (m_soundEngine)
											m_soundEngine->play2D("src/Audio/Powerup - Pickup.mp3", false);
										m_dynamicsWorld->addRigidBody(m_players.at(l)->GetBody());
									}
								}
							}
							else if (m_powerUps.at(k)->GetType() == 6)
							{
								vec3 temp = vec3(m_powerUps.at(k)->GetPos().x(), m_powerUps.at(k)->GetPos().y(),
								                 m_powerUps.at(k)->GetPos().z());
								m_bombZone.push_back(temp);
							}
							else if (m_powerUps.at(k)->GetType() == 10)
							{
								m_spawnBall = true;
							}
							else
							{
								if (m_soundEngine)
								{
									m_soundEngine->play2D("src/Audio/Powerup - Pickup.mp3", false);
								}
								m_dynamicsWorld->removeRigidBody(m_players.at(f)->GetBody());
								m_players.at(f)->GivePower(m_powerUps.at(k)->GetType());
								m_dynamicsWorld->addRigidBody(m_players.at(f)->GetBody());
							}
							RemovePowerUp(k);
						}
					}
				}
			}
		}
		else if (shapeA->getShapeType() == 0 && shapeB->getShapeType() == 8)
		{
			for (int f = 0; f < m_players.size(); f++)
			{
				if (m_players.at(f)->GetBody() == obB)
				{
					PUpCollision[f] = true;
					for (int k = 0; k < m_powerUps.size(); k++)
					{
						if (m_powerUps.at(k)->getObject() == obA)
						{
							if (m_powerUps.at(k)->GetType() == 5 || m_powerUps.at(k)->GetType() == 4 || m_powerUps
							                                                                            .at(k)->
							                                                                            GetType() == 1)
							{
								for (int l = 0; l < m_players.size(); l++)
								{
									if (l != f)
									{
										m_dynamicsWorld->removeRigidBody(m_players.at(l)->GetBody());
										m_players.at(l)->GivePower(m_powerUps.at(k)->GetType());

										if (m_soundEngine)
											m_soundEngine->play2D("src/Audio/Powerup - Pickup.mp3", false);
										m_dynamicsWorld->addRigidBody(m_players.at(l)->GetBody());
									}
								}
							}
							else if (m_powerUps.at(k)->GetType() == 6)
							{
								vec3 temp = vec3(m_powerUps.at(k)->GetPos().x(), m_powerUps.at(k)->GetPos().y(),
								                 m_powerUps.at(k)->GetPos().z());
								m_bombZone.push_back(temp);
							}
							else if (m_powerUps.at(k)->GetType() == 10)
							{
								m_spawnBall = true;
							}
							else
							{
								if (m_soundEngine)
									m_soundEngine->play2D("src/Audio/Powerup - Pickup.mp3", false);
								m_dynamicsWorld->removeRigidBody(m_players.at(f)->GetBody());
								m_players.at(f)->GivePower(m_powerUps.at(k)->GetType());
								m_dynamicsWorld->addRigidBody(m_players.at(f)->GetBody());
							}
							RemovePowerUp(k);
						}
					}
				}
			}
		}
	}
}

bool ObjectHandler::CheckCollisionCars(float dt)
{
	m_collision = false;
	int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; ++i)
	{
		btPersistentManifold* contactManifold = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());
		btCollisionShape* shapeA = obA->getCollisionShape();
		btCollisionShape* shapeB = obB->getCollisionShape();

		//Collision between spheres(cars)
		if (shapeA->getShapeType() == 8 && shapeB->getShapeType() == 8)
		{
			for (int f = 0; f < m_players.size(); f++)
			{
				if (m_players.at(f)->GetBody() == obA)
				{
					m_collision = true;
					m_players.at(f)->SetBoolLights(false);
				}
				if (m_players.at(f)->GetBody() == obB)
				{
					m_collision = true;
					m_players.at(f)->SetBoolLights(false);
				}
			}
		}
	}

	return m_collision;
}

bool ObjectHandler::GetLightsOut()
{
	//for (uint i = 0; i < m_ghosts.size(); i++)
	//{
	//	if (m_ghosts.at(i)->GetLightSwitch())
	//	{
	//		m_lightsOut = true;
	//	}
	//	if (!m_ghosts.at(i)->GetLightSwitch())
	//	{
	//		m_lightsOut = false;
	//	}
	//}
	return m_lightsOut;
}

void ObjectHandler::SetLightsOut(bool state)
{
	m_lightsOut = state;
}

bool ObjectHandler::GetTerrain()
{
	return m_terrain;
}

void ObjectHandler::SetTerrain(bool state)
{
	m_terrain = state;
}


void ObjectHandler::RenderParticles()
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		m_particles.at(i)->Draw();
	}
}

void ObjectHandler::RemoveAllObjects()
{
	int nrOf = m_objects.size();
	for (int i = 0; i < nrOf; i++)
	{
		RemoveObject(0);
	}
}

void ObjectHandler::RemoveAllGhost()
{
	int temp = m_ghosts.size();
	for (int i = 0; i < temp; i++)
	{
		delete m_ghosts.at(i);
	}
	m_ghosts.clear();
}

void ObjectHandler::UpdateLastPos()
{
	//For Hook

	int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; ++i)
	{
		btPersistentManifold* contactManifold = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

		btCollisionShape* shapeA = obA->getCollisionShape();
		btCollisionShape* shapeB = obB->getCollisionShape();

		//Collision between car and terrain
		for (auto p : m_players)
		{
			if (p->GetCurrentPos().y() >= 0)
			{
				if (obA == p->GetBody() && obB == m_cube->GetBody())
				{
					btVector3 btPos = p->GetCurrentPos();

					//This is stopid unrelialbe way- will need to fix better 
					vec3 diff = vec3(-7, 6, -7);
					if (btPos.x() < 0)
					{
						diff.x *= -1;
					}
					if (btPos.z() < 0)
					{
						diff.z *= -1;
					}
					vec3 lastPos = vec3(btPos.x() + diff.x, btPos.y() + diff.y, btPos.z() + diff.z);
					p->SetLastPos(lastPos);
				}
			}
		}
	}
}

void ObjectHandler::UpdateHook(float dt)
{
	for (auto p : m_players)
	{
		if (p->GetNrOfRockets() > 0)
		{
			if (p->GetCurrentPos().y() < -0.5)
			{
				p->SetHook(true);
			}
		}
	}

	for (auto p : m_players)
	{
		if (p->GetHook() == true)
		{
			vec3 lastPos = p->GetLastPos();
			btVector3 btPos = p->GetCurrentPos();
			vec3 curPos = vec3(btPos.x(), btPos.y(), btPos.z());
			vec3 dir = curPos - lastPos;

			dir = normalize(dir);
			vec3 pos = curPos - (dir * dt * 15.f);

			p->SetPos(pos);
			if (curPos.y > 6.f)
			{
				p->removeRocket(); //Also deactivates hookActive bool
			}
		}
	}
}

bool ObjectHandler::GetPlayerHook(int index)
{
	return m_players[index]->GetHook();
}

void ObjectHandler::PowerUpParticles(Scene *m_scene) 
{ 
	for (int i = 0; i < m_players.size(); i++) 
	{ 
		if (PUpCollision[i] == true) 
		{ 
			btVector3 btPos= m_players.at(i)->GetCurrentPos(); 
			vec3 pos = vec3(btPos.x(), btPos.y(), btPos.z()); 
 
			m_scene->AddParticleEffect( pos, vec3(NULL), vec3(NULL), 2, 4, vec3(1, 0, 0), 35, 0.5, 0.2,-9.82); 
			PUpCollision[i] = false; 
		} 
	} 
 
} 
