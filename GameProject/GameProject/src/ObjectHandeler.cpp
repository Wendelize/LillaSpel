#include "Header Files/Include.h"
#include "Header Files/ObjectHandler.h"

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
}

ObjectHandler::~ObjectHandler()
{
	for (size_t i = 0; i < m_structs.size(); i++)
	{
		delete m_structs.at(i);
	}
	m_structs.clear();

	for (uint i = 0; i < m_players.size(); i++)
	{
		delete m_players.at(i);
	}
	m_players.clear();

	for (uint i = 0; i < m_platforms.size(); i++)
	{
		delete m_platforms.at(i);
	}
	m_platforms.clear();

	// MOVE TO PLAYER DESTRUCTOR SO THAT WHEN A CAR IS DELETED, ITS RIGID BODY IS DESTROYED?
	//remove the rigidbodies from the dynamics world and delete them
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

	// MOVE TO PLAYER DESTRUCTOR SO THAT WHEN A CAR IS DELETED, ITS RIGID BODY IS DESTROYED?
	//delete collision shapes
	for (int j = 0; j < m_collisionShapes.size(); j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		m_collisionShapes[j] = 0;
		delete shape;
	}

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

	m_dynamicsWorld->stepSimulation(dt, 10);

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
		int isPlayer = i - m_platforms.size();
		if (isPlayer >= 0) {
			m_players[isPlayer]->Update(dt);
			if (m_players[isPlayer]->GetCurrentPos().y() < -20.f) {
				m_players[isPlayer]->SetPos(vec3(rand() % 10 - 10, 3, rand() % 10 - 10));
			}
		}
	}
}

void ObjectHandler::AddPlayer(vec3 pos, int controllerID, int modelId, vec3 color, Model* model)
{
	// Till players konstruktor skall meshens vertices passas + ID f�r att identifiera vilken modell som tillh�r objektet!
	m_players.push_back(new Player(model, modelId, pos));
	m_players.back()->SetControllerID(controllerID);
	m_players.back()->SetModelId(modelId);
	m_players.back()->SetColor(color);

	m_dynamicsWorld->addRigidBody(m_players.back()->GetBody());

}

void ObjectHandler::SetScale(int id, vec3 scale)
{
	m_players.at(id)->SetScale(scale);
}

void ObjectHandler::RemovePlayer(int index)
{
	delete m_players.at(index);
	m_players.erase(m_players.begin() + index);
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
	// to be continued
}

void ObjectHandler::RemovePowerUp()
{
	// to be continued
}

int ObjectHandler::GetNumPlayers()
{
	cout << m_players.size() << endl;
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

	//for (int i = 0; i < m_powerUps.size; i++)
	//{
	//	  temp.push_back(m_powerUps[i]->GetObject());
	//}

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
