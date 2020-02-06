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
	vec3 m_scale;
	vec3 m_color;
	Controller* m_controller;
	Transform* m_transform;
	ObjectInfo* m_info;

public:
	Player(vec3 pos);
	~Player();
	void Update(float dt);
	string GetName();
	void SetName(string name);
	int GetHealth();
	void SetHealth(int health);
	int GetModelId();
	void SetModelId(int id);
	float GetSpeed();
	void SetSpeed(float speed);
	vec3 GetColor();
	void SetColor(vec3 color);
	float GetWeight();
	void SetWeight(float weight);
	vec3 GetScale();
	void SetScale(vec3 scale);
	int GetControllerID();
	void SetControllerID(int id);
	vec3 GetDirection();
	ObjectInfo* GetObjectInfo();

};
