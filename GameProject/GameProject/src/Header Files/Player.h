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
	bool m_powerActive;
	int m_powerType;
	int m_powerDuration;
	float m_weight;
	float m_speed;
	float m_powerMultiplier;
	float m_restitution;
	int m_lives;
	vec3 m_scale;
	vec3 m_color;
	Controller* m_controller;
	Transform* m_transform;
	ObjectInfo* m_info;
	ISoundEngine* m_soundEngine;
	ISoundEngine* m_honkEngine;

	vector<ISoundSource*> m_carSounds;
	ISound* m_sound;
	ISound* m_honk;
	bool m_fallen;
	bool m_inverted;
	Light* m_lights[2];
	bool m_lightsOn = true;

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

	int GetLives();
	void SetLives(int num);
	void ReduceLife();
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
	mat4 GetMatrix();
	ObjectInfo* GetObjectInfo();
	Light* GetLight(int index);
	btRigidBody* GetBody();
	btVector3 GetCurrentPos();
	void SetPos(vec3 pos);
	bool GetFallen();
	void SetFallen();
	void SetNotFallen();
	float GetLinearVelocity();
	void StartEngineSounds();
	void StopEngineSounds();
	void GivePower(int type);
	void removePower(int type);
	bool updatePower(float dt);
	int GetActivePower();
	Controller* GetController();
	bool GetBoolLights();
	void SetBoolLights(bool state);
};
