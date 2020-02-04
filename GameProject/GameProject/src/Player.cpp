#include "Header Files/Player.h"

Player::Player(vec3 pos)
{
	m_controller = new Controller;
	m_transform = new Transform;
	m_transform->SetTranslation(pos);
	m_name = "";
	m_health = 0;
	m_controllerID = 0;
	m_weight = 0.f;
	m_speed = 0.f;

	m_transform->SetScale(0.5, 0.5, 0.5);

}

Player::~Player()
{
	delete m_controller;
	delete m_transform;
}

void Player::Update(float dt)
{
	vec3 rotate(0, 0, 0);
	vec3 direction(0, 0, 0);
	float rotationSpeed = 0.2f;
	float maxSpeed = -15.f;
	float maxReverseSpeed = 7.f;

	if (glfwJoystickPresent(m_controllerID) == 1)
	{
		if (m_controller->ButtonOptionsIsPressed(m_controllerID))
		{
			//Temporary
			if(m_speed > maxSpeed)
			m_speed -= 0.5f;
		}

		if (m_controller->ButtonAIsPressed(m_controllerID))
		{
			//Acceleration
			if (m_speed > maxSpeed)
			m_speed -= 0.5f;
		}

		if (m_controller->ButtonXIsPressed(m_controllerID))
		{
			//Reverse
			if (m_speed < maxReverseSpeed)
			m_speed -= -0.3f;
		}

		//Triggers
		if (m_controller->GetLefTrigger(m_controllerID) != -1)
		{
			//Left trigger pressed
			//Power-Up
			m_speed = -30.f;
		}

		//Left stick horisontal input
		if(m_controller->GetLeftStickHorisontal(m_controllerID) > 0.2f || m_controller->GetLeftStickHorisontal(m_controllerID) < -0.2f)
			rotate.y -= m_controller->GetLeftStickHorisontal(m_controllerID);
		
		if (m_controller->GetRightTrigger(m_controllerID) != -1)
		{
			//Right trigger pressed
			//Drift
			if (rotate.y < -0.2)
			{
				rotate.y -= 0.5;
			}
			else if(rotate.y > 0.2)
			{
				rotate.y -= (-0.5);
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
		
	
		if(m_speed != 0)
		direction = m_transform->TranslateDirection(rotate*dt* rotationSpeed * (-m_speed));
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

	m_transform->Translate(  direction* m_speed* dt);
	
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