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
	for (int i = 0; i < 20; i++) {
		m_usedSpawns[i] = false;
	}


	m_soundEngine = createIrrKlangDevice();
	m_soundEngine->setSoundVolume(0.6f);

	m_crashes.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Player - Crash Small.mp3"));
	m_crashes.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Player - Crash Medium.mp3"));
	m_crashes.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Player - Crash Biggest.mp3"));

	for (int i = 0; i < m_crashes.size(); i++)
	{
		m_crashes[i]->setDefaultVolume(0.55f);
	}

	m_soundEngine->setListenerPosition(vec3df(0, 18, 33), vec3df(0, -4, 3)); // Listener position, view direction
	m_soundEngine->setDefault3DSoundMinDistance(10.0f);
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
	int temp = m_powerUps.size();
	for (uint i = 0; i < temp; i++) {
		RemovePowerUp(0);
	}
	m_powerUps.clear();
	m_platforms.clear();
	for (size_t i = 0; i < m_structs.size(); i++)
	{
		delete m_structs.at(i);
	}
	m_structs.clear();

	for (uint i = 0; i < m_crashes.size(); i++)
	{
		m_crashes[i]->drop();
	}
	m_crashes.clear();

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

	// m_soundEngine->drop(); // Might need to delete but no memory leaks found.

}

void ObjectHandler::Update(float dt)
{
	const char* filename;
	int randomNumber = rand() % 12;

	m_dynamicsWorld->stepSimulation(dt, 10);
	int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < m_powerUps.size(); i++)
	{
		btAlignedObjectArray< btCollisionObject* >& pairs = m_powerUps.at(i)->getObject()->getOverlappingPairs();
		for (int j = 0; j < pairs.size(); j++)
		{
			bool notFound = true;
			for (int k = 0; k < m_players.size() && notFound; k++) 
			{
				int checkPlayer = m_players.at(k)->GetControllerID();
				int collidingPlayer = pairs.at(j)->getUserIndex();
				if (checkPlayer == collidingPlayer) 
				{
					notFound = false;
					if (m_powerUps.at(i)->GetType() == 5 || m_powerUps.at(i)->GetType() == 4) 
					{
						for (int l = 0; l < m_players.size(); l++) 
						{
							if (l != k) 
							{
								m_dynamicsWorld->removeRigidBody(m_players.at(l)->GetBody());
								m_players.at(l)->GivePower(m_powerUps.at(i)->GetType());
								m_dynamicsWorld->addRigidBody(m_players.at(l)->GetBody());
							}
						}
					}
					else 
					{
						m_dynamicsWorld->removeRigidBody(m_players.at(k)->GetBody());
						m_players.at(k)->GivePower(m_powerUps.at(i)->GetType());
						m_dynamicsWorld->addRigidBody(m_players.at(k)->GetBody());
					}
				}
			}
			RemovePowerUp(i);
		}
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

		int isPlayer = i - m_platforms.size() - m_powerUps.size();
		int isPowerUp = i - m_platforms.size() - m_players.size();

		if (isPowerUp >= 0) {
			if (m_powerUps[isPowerUp]->update(dt)) {
				RemovePowerUp(isPowerUp);
			}
		}

		if (isPlayer >= 0) {
			m_players[isPlayer]->Update(dt);
			if (m_players[isPlayer]->updatePower(dt)) {
				m_dynamicsWorld->removeRigidBody(m_players[isPlayer]->GetBody());
				m_players[isPlayer]->removePower(m_players[isPlayer]->GetActivePower());
				m_dynamicsWorld->addRigidBody(m_players[isPlayer]->GetBody());

			}

			if (m_players[isPlayer]->GetCurrentPos().y() < -20.f) 
			{
				m_players[isPlayer]->SetPos(vec3(rand() % 10 - 10, 7, rand() % 10 - 10));
				m_players[isPlayer]->SetNotFallen();
				m_soundEngine->play3D("src/Audio/Powerup - Spawn.mp3", vec3df(m_players[isPlayer]->GetCurrentPos().x(), m_players[isPlayer]->GetCurrentPos().y(), m_players[isPlayer]->GetCurrentPos().z()));
				m_players[isPlayer]->StartEngineSounds();
			}

			if (m_players[isPlayer]->GetCurrentPos().y() < -0.1f && !m_players[isPlayer]->GetFallen())
			{
				if (randomNumber == 0)
					filename = "src/Audio/Player - Dying 1.mp3";
				else if (randomNumber == 1)
					filename = "src/Audio/Player - Dying 2.mp3";
				else if (randomNumber == 2)
					filename = "src/Audio/Player - Dying 3.mp3";
				else if (randomNumber == 3)
					filename = "src/Audio/Player - Dying 4.mp3";
				else if (randomNumber == 4)
					filename = "src/Audio/Player - Dying 5.mp3";
				else if (randomNumber == 5)
					filename = "src/Audio/Player - Dying 6.mp3";
				else if (randomNumber == 6)
					filename = "src/Audio/Player - Dying 7.mp3";
				else if (randomNumber == 7)
					filename = "src/Audio/Player - Dying 8.mp3";
				else if (randomNumber == 8)
					filename = "src/Audio/Player - Dying 9.mp3";
				else if (randomNumber == 9)
					filename = "src/Audio/Player - Dying 10.mp3";
				else if (randomNumber == 10)
					filename = "src/Audio/Player - Dying 11.mp3";
				else
					filename = "src/Audio/Player - Dying 12.mp3";

				m_soundEngine->play3D(filename, vec3df(m_players[isPlayer]->GetCurrentPos().x(), m_players[isPlayer]->GetCurrentPos().y(), m_players[isPlayer]->GetCurrentPos().z()));
				m_players[isPlayer]->SetFallen();
				m_players[isPlayer]->StopEngineSounds();
			}

			int knockableCars = 0;

			for (int i = 0; i < m_players.size(); i++)
			{
				if ( m_players[i]->GetCurrentPos().y() > -0.1f && m_players[i]->GetCurrentPos().y() < 2.0f)
					knockableCars++;
			}

			if (numManifolds >= knockableCars + 1) // knockableCars are cars inside collision space. +1 because of platform. 
			{
				if (m_players[isPlayer]->GetLinearVelocity() > 4.5f && m_players[isPlayer]->GetLinearVelocity() < 10.0f && m_players[isPlayer]->GetCurrentPos().y() > -0.1f)
				{
					m_soundEngine->play3D(m_crashes[0], vec3df(m_players[isPlayer]->GetCurrentPos().x(), m_players[isPlayer]->GetCurrentPos().y(), m_players[isPlayer]->GetCurrentPos().z()));
				}
				else if (m_players[isPlayer]->GetLinearVelocity() >= 10.0f && m_players[isPlayer]->GetLinearVelocity() < 17.0f && m_players[isPlayer]->GetCurrentPos().y() > -0.1f)
				{
					m_soundEngine->play3D(m_crashes[1], vec3df(m_players[isPlayer]->GetCurrentPos().x(), m_players[isPlayer]->GetCurrentPos().y(), m_players[isPlayer]->GetCurrentPos().z()));
				}
				else if (m_players[isPlayer]->GetLinearVelocity() >= 17.0f && m_players[isPlayer]->GetCurrentPos().y() > -0.1f)
				{
					m_soundEngine->play3D(m_crashes[2], vec3df(m_players[isPlayer]->GetCurrentPos().x(), m_players[isPlayer]->GetCurrentPos().y(), m_players[isPlayer]->GetCurrentPos().z()));
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

	m_players.back()->StartEngineSounds();
}

void ObjectHandler::SetScale(int id, vec3 scale)
{
	m_players.at(id)->SetScale(scale);
}

void ObjectHandler::RemovePlayer(int index)
{
	delete m_players.at(index);
	m_players.erase(m_players.begin() + index);

	// MOVE TO PLAYER DESTRUCTOR SO THAT WHEN A CAR IS DELETED, ITS RIGID BODY IS DESTROYED?
	//remove the rigidbodies from the dynamics world and delete them
	int removeIndex = m_dynamicsWorld->getNumCollisionObjects() - 1;

	btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[removeIndex];

	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		delete body->getMotionState();
	}
	m_dynamicsWorld->removeCollisionObject(obj);
	delete obj;
}

void ObjectHandler::AddPlatform(int modelId, Model* model)
{
	// Till platforms konstruktor skall meshens vertices passas + ID f�r att identifiera vilken modell som tillh�r objektet!
	m_platforms.push_back(new Platform(model, modelId));
	m_platforms.back()->SetModelId(modelId);

	m_dynamicsWorld->addRigidBody(m_platforms.back()->getBody());
}

void ObjectHandler::RemovePlatform()
{
	// Kommer aldrig anropas?
}

void ObjectHandler::AddPowerUp()
{
	srand(time(NULL));
	int spawnLocation = rand() % (20);
	int type = rand() % (6);
	if (m_usedSpawns[spawnLocation] == true) {
		bool notFound = true;
		int i = 0;
		while (notFound) {
			i++;
			spawnLocation++;
			if (spawnLocation == 20) {
				spawnLocation = 0;
			}
			if (m_usedSpawns[spawnLocation] == false) {
				notFound = false;
			}
			if (i == 20) {
				notFound = false;
				spawnLocation = 19;
			}
		}
	}
	m_usedSpawns[spawnLocation] = true;
		
	m_powerUps.push_back(new PowerUp(spawnLocation, m_spawnpoints[spawnLocation], type));
	m_dynamicsWorld->addCollisionObject(m_powerUps.back()->getObject());

	m_soundEngine->play3D("src/Audio/Powerup - Spawn.mp3", vec3df(m_powerUps.back()->GetPos().x(), m_powerUps.back()->GetPos().y(), m_powerUps.back()->GetPos().z()));
}

void ObjectHandler::RemovePowerUp(int index)
{
	m_dynamicsWorld->removeCollisionObject(m_powerUps.at(index)->getObject());
	m_usedSpawns[m_powerUps.at(index)->GetSpawn()] = false;
	ObjectInfo* temp = m_powerUps.at(index)->GetObjectInfo();

	for (int i = 0; i < m_structs.size(); i++) {
		if (m_structs.at(i)->modelMatrix == temp->modelMatrix && m_structs.at(i)->typeId == temp->typeId) {
			delete m_structs.at(i);
			m_structs.at(i) = new ObjectInfo(mat4(0),1,1,vec3(0), false);
		}
	}
	delete temp;

	m_dynamicsWorld->removeCollisionObject(m_powerUps.at(index)->getObject());
	delete m_powerUps.at(index)->getObject();
	delete m_powerUps.at(index)->GetObjectInfo();
	delete m_powerUps.at(index);
	m_powerUps.erase(m_powerUps.begin() + index);
}

int ObjectHandler::GetNumPlayers()
{
	return m_players.size();
}

vec3 ObjectHandler::GetPlayerDirection(int index)
{
	return m_players[index]->GetDirection();
}

int ObjectHandler::GetPlayerControllerID(int index)
{
	return m_players[index]->GetControllerID();
}

void ObjectHandler::SetPlayerControllerID(int index, int id)
{
	m_players[index]->SetControllerID(id);
}

int ObjectHandler::GetPlayerModelID(int index)
{
	return m_players[index]->GetModelId();
}

void ObjectHandler::SetPlayerModelID(int index, int id)
{
	m_players[index]->SetModelId(id);
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
	return m_structs;
}

btDiscreteDynamicsWorld* ObjectHandler::GetWorld()
{
	return m_dynamicsWorld;
}

DebugDrawer* ObjectHandler::GetDebugDrawer()
{
	return m_debugDrawer;
}
