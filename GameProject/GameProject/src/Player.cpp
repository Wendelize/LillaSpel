#include "Header Files/Player.h"

Player::Player()
{
	m_controller = nullptr;
	m_transform = nullptr;
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

void Player::Update()
{
	if (glfwJoystickPresent(m_controllerID) == 1)
	{
		if (m_controller->ButtonAIsPressed(m_controllerID))
		{

		}

		if (m_controller->ButtonXIsPressed(m_controllerID))
		{

		}

		//Triggers
		if (m_controller->GetLefTrigger(m_controllerID) != -1)
		{
			//Left trigger pressed
		}

		if (m_controller->GetRightTrigger(m_controllerID) != -1)
		{
			//Right trigger pressed
		}

		//Joysticks
		if (m_controller->GetLeftStickVertical(m_controllerID) < -0.5f)
		{
			//Left stick - up 
		}
		if (m_controller->GetLeftStickVertical(m_controllerID) > 0.5f)
		{
			//Left stick - down
		}
		if (m_controller->GetLeftStickHorisontal(m_controllerID) < -0.5f)
		{
			//Left stick - left
		}
		if (m_controller->GetLeftStickHorisontal(m_controllerID) > 0.5f)
		{
			// Left stick - right
		}
	}
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

//ObjectInfo* Player::GetObjectInfo()
//{
//	//ObjectInfo* temp = new ObjectInfo();
//	/*temp->modelId = m_modelId;
//	temp->typeId = 0;
//	temp->modelMatrix = m_transform->GetMatrix();
//	return temp;*/
//}