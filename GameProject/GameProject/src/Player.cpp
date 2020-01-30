#include "Header Files/Player.h"

Player::Player()
{
	m_controller = new Controller;
	m_transform = new Transform;
	m_name = "";
	m_health = 0;
	m_controllerID = 0;
	m_weight = 0.f;
	m_speed = 0.f;

}

Player::~Player()
{
	delete m_controller;
	delete m_transform;
}

void Player::Update(float dt)
{
	vec3 movmentInput(0, 0, 0);
	vec3 rotate(0, 0, 0);
	vec3 direction(0, 0, 0);
	float speed = 0;
	float rotationSpeed = 2.f;

	if (glfwJoystickPresent(m_controllerID) == 1)
	{
		if (m_controller->ButtonAIsPressed(m_controllerID))
		{
			//Acceleration
			speed = -10.f;
		}

		if (m_controller->ButtonXIsPressed(m_controllerID))
		{
			//Reverse
			speed = 10.f;
		}

		//Triggers
		if (m_controller->GetLefTrigger(m_controllerID) != -1)
		{
			//Left trigger pressed
			//Power-Up
			speed = -30.f;
		}

		if (m_controller->GetRightTrigger(m_controllerID) != -1)
		{
			//Right trigger pressed
			//Drift
		}

			rotate.y -= m_controller->GetLeftStickHorisontal(m_controllerID);
	

		if(speed != 0)
		direction = m_transform->TranslateDirection(rotate*dt* rotationSpeed);
	}

	m_transform->Translate(  direction*   speed* dt);
	
	
	
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
	return new ObjectInfo(m_transform->GetMatrix(), m_modelId, 0);
}