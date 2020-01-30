#pragma once
#include "Include.h"
#include "Transform.h"

class Player
{
private:
	string m_name;
	int m_health;
	int m_controllerID;
	int m_modelId;
	float m_weight;
	float m_speed;
	Controller* m_controller;
	Transform* m_transform;

public:
	Player();
	~Player();
	void Update();
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
	//ObjectInfo* GetObjectInfo();

};
