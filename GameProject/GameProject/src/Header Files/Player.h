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
	Object* object;

	Player();
	~Player();
	string GetName();
};