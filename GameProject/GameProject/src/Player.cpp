#include "Header Files/Player.h"

Player::Player(Model* model, int modelId, vec3 pos)
{
	m_controller = new Controller;
	m_transform = new Transform;
	m_transform->SetScale(0.5, 0.5, 0.5);

	//m_transform->SetTranslation(pos);
	m_name = "";
	m_health = 0;
	m_controllerID = 0;
	m_modelId = modelId;
	m_weight = 0.f;
	m_speed = 0.f;

	/* ConvexHullShape for car. Very precise but expensive since it creates A LOT of lines.
	vector<btVector3> points;

	for (int i = 0; i < model->GetMeshes().size(); i++)
	{
		for (int j = 0; j < model->GetMeshes()[i].m_vertices.size(); j++)
		{
			points.push_back(btVector3(model->GetMeshes()[i].m_vertices[j].pos.x, model->GetMeshes()[i].m_vertices[j].pos.y, model->GetMeshes()[i].m_vertices[j].pos.z));
		}
	}
	
	m_carShape = new btConvexHullShape(&points[0].getX(), points.size(), sizeof(btVector3));
	*/
	// m_carShape = new btBoxShape(btVector3(btScalar(0.5f), btScalar(0.5f), btScalar(0.5f))); // BoxShape

	m_carShape = new btSphereShape(1.);

	m_btTransform = new btTransform;
	m_btTransform->setIdentity();

	btScalar mass(0.01f);
	m_btTransform->setOrigin(btVector3(pos.x,pos.y,pos.z));
	vec3 testPos = vec3(m_btTransform->getOrigin().x(), m_btTransform->getOrigin().y()-2.f, m_btTransform->getOrigin().z());
	m_transform->SetTranslation(testPos);
	cout << "Pos: " << testPos.x << ", " << testPos.y << ", " << testPos.z << endl;
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		m_carShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	m_motionState = new btDefaultMotionState(*m_btTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_motionState, m_carShape, localInertia);
	m_body = new btRigidBody(rbInfo);
	m_body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	m_body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
	m_body->clearForces();
	m_currentPos = m_btTransform->getOrigin();
}

Player::~Player()
{
	delete m_controller;
	delete m_transform;
	delete m_btTransform;
	delete m_carShape;
}

void Player::Update(float dt)
{
	vec3 rotate(0, 0, 0);
	vec3 direction(0, 0, 0);
	float rotationSpeed = 5.f;
	float maxSpeed = -15.f;
	float maxReverseSpeed = 7.f;
	//m_speed = 0;
	if (glfwJoystickPresent(m_controllerID) == 1)
	{
		if (m_controller->ButtonOptionsIsPressed(m_controllerID))
		{
			//Temporary
				m_speed = 0;
				m_body->setLinearVelocity(btVector3(0, 0, 0));
		}

		if (m_controller->ButtonAIsPressed(m_controllerID))
		{
			//Acceleration
			if (m_speed > maxSpeed)
				m_speed = 0.5f;
		}

		if (m_controller->ButtonXIsPressed(m_controllerID))
		{
			//Reverse
			if (m_speed < maxReverseSpeed)
				m_speed = -0.4f;
		}

		//Triggers
		if (m_controller->GetLefTrigger(m_controllerID) != -1)
		{
			//Left trigger pressed
			//Power-Up
			m_speed = -1.f;
		}

		//Left stick horisontal input
		if (m_controller->GetLeftStickHorisontal(m_controllerID) > 0.2f || m_controller->GetLeftStickHorisontal(m_controllerID) < -0.2f)
			rotate.y -= m_controller->GetLeftStickHorisontal(m_controllerID);

		if (m_controller->GetRightTrigger(m_controllerID) != -1)
		{
			//Right trigger pressed
			//Drift
			if (rotate.y < -0.2)
			{
				rotate.y = 0.5;
			}
			else if (rotate.y > 0.2)
			{
				rotate.y = (-0.5);
			}
		}

		//Set rotationSpeed depending on your speed, more speed--> Can turn less. 
		if (m_speed < maxSpeed - 1)
		{
			rotationSpeed = 0.1;
		}
		else if (m_speed < maxSpeed)
		{
			rotationSpeed = 0.15;
		}


		if (m_speed != 0)
			direction = m_transform->TranslateDirection(rotate * dt * rotationSpeed);
	}

	//"Friction"
	if (m_speed < 0)
	{
		m_speed = m_speed + 9.82f * dt;
	}
	else if (m_speed > 0)
	{
		m_speed = m_speed - 9.82f * dt;
	}

	if (m_speed < 0.2 && m_speed > -0.2)
	{
		m_speed = 0;
	}
	//	m_transform->Translate(  direction* m_speed* dt);


	btVector3 tempMove = { direction.x,0,direction.z };
	btVector3 tempMove2 = tempMove * m_body->getLinearVelocity();
	m_body->setLinearVelocity((tempMove * 10 * -m_speed) + btVector3(tempMove2.x(), m_body->getLinearVelocity().y() ,tempMove2.z()));
	// m_body->applyForce(m_speed * 500 * tempMove, m_body->getWorldTransform().getOrigin());

	if (vec2( m_body->getLinearVelocity().x(), m_body->getLinearVelocity().z()).length() > 7) {

		vec2 temp = normalize(vec2(m_body->getLinearVelocity().x(), m_body->getLinearVelocity().z()));

	//	temp = vec2(m_body->getLinearVelocity().x(), m_body->getLinearVelocity().z()) / 
		cout << "temp: " << temp.x << ", " << temp.y << endl;
		m_body->setLinearVelocity(btVector3(temp.x, m_body->getLinearVelocity().y(), temp.y));

	}
	cout << tempMove.x() << ", " << tempMove.y()<< ", " << tempMove.z() << endl;
	btVector3 moveVector = m_body->getWorldTransform().getOrigin() - m_currentPos;
	m_transform->Translate(vec3(moveVector.x(), moveVector.y(), moveVector.z()));


	m_currentPos = m_body->getWorldTransform().getOrigin();
}

string Player::GetName()
{
	return m_name;
}

void Player::SetName(string name)
{
	m_name = name;
}

int Player::GetHealth()
{
	return m_health;
}

void Player::SetHealth(int health)
{
	m_health = health;
}

int Player::GetModelId()
{
	return m_modelId;
}

void Player::SetModelId(int id)
{
	m_modelId = id;
}

float Player::GetSpeed()
{
	return m_speed;
}

void Player::SetSpeed(float speed)
{
	m_speed = speed;
}

vec3 Player::GetColor()
{
	return m_color;
}

void Player::SetColor(vec3 color)
{
	m_color = color;
}

float Player::GetWeight()
{
	return m_weight;
}

void Player::SetWeight(float weight)
{
	m_weight = weight;
}

int Player::GetControllerID()
{
	return m_controllerID;
}

void Player::SetControllerID(int id)
{
	m_controllerID = id;
}

ObjectInfo* Player::GetObjectInfo()
{
	m_info = new ObjectInfo(m_transform->GetMatrix(), m_modelId, 0, m_color);
	return m_info;
}

btRigidBody* Player::GetBody()
{
	return m_body;
}

btVector3 Player::GetCurrentPos()
{
	return m_body->getWorldTransform().getOrigin();
}
