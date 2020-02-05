#include "Header Files/Player.h"

Player::Player(vec3 pos)
{
	m_controller = new Controller;
	m_transform = new Transform;
	m_transform->SetScale(0.5, 0.5, 0.5);

	//m_transform->SetTranslation(pos);
	m_name = "";
	m_health = 0;
	m_controllerID = 0;
	m_weight = 0.f;
	m_speed = 0.f;

	//m_carShape = new btBoxShape(btVector3(btScalar(0.5f), btScalar(0.5f), btScalar(0.5f)));
	m_carShape = new btSphereShape(0.7);

	m_btTransform = new btTransform;
	m_btTransform->setIdentity();

	btScalar mass(1000.f);
	m_btTransform->setOrigin(btVector3(pos.x,pos.y,pos.z));
	vec3 testPos = vec3(m_btTransform->getOrigin().x(), m_btTransform->getOrigin().y()-2.f, m_btTransform->getOrigin().z());
	m_transform->SetTranslation(testPos);
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		m_carShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	m_motionState = new btDefaultMotionState(*m_btTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_motionState, m_carShape, localInertia);
	rbInfo.m_additionalDamping = true;
	m_body = new btRigidBody(rbInfo);
	m_body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	m_body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
	m_body->clearForces();
	m_body->setRestitution(2);
	m_currentPos = m_btTransform->getOrigin();
	m_body->setDamping(0.1,0.9);
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
	m_body->activate(true);

	vec3 rotate(0, 0, 0);
	vec3 direction(0, 0, 0);
	float rotationSpeed = 2.f;
	m_speed = 0;
	if (glfwJoystickPresent(m_controllerID) == 1)
	{
		if (m_controller->ButtonOptionsIsPressed(m_controllerID))
		{
			//Temporary
			m_body->setLinearVelocity(btVector3(0, 0, 0));
		}

		if (m_controller->ButtonAIsPressed(m_controllerID))
		{
			//Acceleration
			m_speed = 400000.f;

		}

		if (m_controller->ButtonXIsPressed(m_controllerID))
		{
			m_speed = -300000.f;
		}

		//Triggers
		if (m_controller->GetRightTrigger(m_controllerID) != -1)
		{
			//Left trigger pressed
			//Power-Up
			m_speed = 800000.f;
		}
		if (m_controller->GetLefTrigger(m_controllerID) != -1)
		{
			//Left trigger pressed
			//Power-Up
			m_speed = -600000.f;
		}

		//Left stick horisontal input
		if (m_controller->GetLeftStickHorisontal(m_controllerID) > 0.2f || m_controller->GetLeftStickHorisontal(m_controllerID) < -0.2f)
			rotate.y -= m_controller->GetLeftStickHorisontal(m_controllerID);

		//Set rotationSpeed depending on your speed, less speed--> Can turn less. 
		if (m_body->getLinearVelocity().length() < 3.f)
		{
			rotationSpeed = 2 * (m_body->getLinearVelocity().length()/3);
		}
		direction = m_transform->TranslateDirection(rotate * dt * rotationSpeed);
	}
	//apply force
	btVector3 directionBt = { direction.x,0,direction.z };
	m_body->applyForce(directionBt * -m_speed * dt , m_body->getWorldTransform().getOrigin());
	m_body->applyDamping(dt);


	//Move Object and save current Pos
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
