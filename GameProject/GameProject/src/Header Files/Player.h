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
	float m_restitution;
	vec3 m_color;
	Controller* m_controller;
	Transform* m_transform;
	ObjectInfo* m_info;

	//Variables 4 Fysik
	btCollisionShape* m_carShape;
	btRigidBody* m_body;
	btTransform* m_btTransform;
	btDefaultMotionState* m_motionState;
	btVector3 m_currentPos;
public:
	Player(Model* model, int modelId, vec3 pos);
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
	void SetScale(vec3 scale);
	int GetControllerID();
	void SetControllerID(int id);
	vec3 GetDirection();
	ObjectInfo* GetObjectInfo();
	btRigidBody* GetBody();
	btVector3 GetCurrentPos();
	void SetPos(vec3 pos);

};
