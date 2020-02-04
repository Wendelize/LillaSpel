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
	vec3 m_color;
	Controller* m_controller;
	Transform* m_transform;
	ObjectInfo* m_info;

	//Variables 4 Fysik
	btCollisionShape* m_carShape;
	btRigidBody* m_body;
	btTransform* m_btTransform;
	vec3 m_currentPos;
public:
	Player(vec3 pos);
	~Player();
	void Update(float dt, vec3 newPos);
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
	int GetControllerID();
	void SetControllerID(int id);
	ObjectInfo* GetObjectInfo();
	btRigidBody* GetBody();
	vec3 GetCurrentPos();

};
