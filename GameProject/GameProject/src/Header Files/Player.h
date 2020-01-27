#pragma once
#include "Include.h"

class Player
{
private:
	string m_name;
	int m_health;
	int m_controllerID;
	float m_weight;
	float m_speed;
public:
	Object* m_object;

	Player();
	~Player();
	string GetName();
	void SetName(string name);
	int GetHealth();
	void SetHealth(int health);
	float GetSpeed();
	void SetSpeed(float speed);
	float GetWeight();
	void SetWeight(float weight);
	int GetControllerID();
	void SetControllerID(int id);
	Object* GetObject();
	void SetObject(Object* object);
};

Player::Player()
{
	m_object = nullptr;
	m_name = "";
	m_health = 0;
	m_controllerID = 0;
	m_weight = 0.f;
	m_speed = 0.f;
}

Player::~Player()
{
	delete m_object;
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

Object* Player::GetObject()
{
	return m_object;
}

void Player::SetObject(Object* object)
{
	m_object = object;
}