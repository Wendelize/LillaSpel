#include "Header Files/Include.h"
#include "Header Files/ObjectHandler.h"

ObjectHandler::ObjectHandler()
{
	m_broadphase = new btDbvtBroadphase();
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);	// btConstraintSolver* m_solver; Kanske använda denna istället.
	m_solver = new btSequentialImpulseConstraintSolver;
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

	m_dynamicsWorld->setGravity(btVector3(0, -9.82f, 0));
	//m_dynamicsWorld->setInternalTickCallback(myTickCallback); Kanske använd update function istället. Callbackar collision check function  https://gamedev.stackexchange.com/questions/22442/how-get-collision-callback-of-two-specific-objects-using-bullet-physics/120881#120881

}

ObjectHandler::~ObjectHandler()
{
	for (size_t i = 0; i < m_temp.size(); i++)
	{
		delete m_temp.at(i);
	}
	m_temp.clear();

	for (size_t i = 0; i < m_players.size(); i++)
	{
		delete m_players.at(i);
	}
	m_players.clear();

	for (size_t i = 0; i < m_platforms.size(); i++)
	{
		delete m_platforms.at(i);
	}
	m_platforms.clear();
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
		//printf("world pos object %d = %f,%f,%f\n", i, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		if (isPlayer >= 0) {
			m_players[isPlayer]->Update(dt, vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
			vec3 tempPos = m_players[isPlayer]->GetCurrentPos();
			m_dynamicsWorld->getCollisionObjectArray()[i]->getWorldTransform().setOrigin(btVector3(tempPos.x,tempPos.y,tempPos.z));
		}
	}

	//Check collison && Act if detected.
}

void ObjectHandler::AddPlayer(vec3 pos, int controllerID, int modelId, vec3 color)
{
	m_players.push_back(new Player(pos));
	m_players.back()->SetControllerID(controllerID);
	m_players.back()->SetModelId(modelId);
	m_players.back()->SetColor(color);

	m_dynamicsWorld->addRigidBody(m_players.back()->GetBody());

}

void ObjectHandler::RemovePlayer(int controllerID)
{
	for (uint i = 0; i < m_players.size(); i++)
	{
		if (m_players[i]->GetControllerID() == controllerID)
		{
			m_players.erase(m_players.begin(), m_players.begin() + i);
		}
	}
}

void ObjectHandler::AddPlatform(int modelId)
{
	m_platforms.push_back(new Platform);
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

vector<ObjectInfo*> ObjectHandler::GetObjects()
{
	for (size_t i = 0; i < m_temp.size(); i++)
	{
		delete m_temp.at(i);
	}
	m_temp.clear();

	for (uint i = 0; i < m_players.size(); i++)
	{
		m_temp.push_back(m_players[i]->GetObjectInfo());
	}

	for (uint i = 0; i < m_platforms.size(); i++)
	{
		m_temp.push_back(m_platforms[i]->GetObjectInfo());
	}

	//for (int i = 0; i < m_powerUps.size; i++)
	//{
	//	  temp.push_back(m_powerUps[i]->GetObject());
	//}

	return m_temp;
}