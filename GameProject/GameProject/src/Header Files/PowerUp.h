#pragma once
#include "Include.h"
#include "Transform.h"

class PowerUp
{
private:
	int m_type;
	float m_duration;
	int m_spawn;
	int m_model;
	vec3 m_color;
	btVector3 m_currentPos;
	bool m_particlesActive = false;
	bool m_bombBlown = false;
	btBoxShape* m_shape;
	ObjectInfo* m_info;
	Transform* m_transform;
	btRigidBody* m_body;
	btTransform* m_btTransform;
	btVector3 m_pos;
	btDefaultMotionState* m_motionState;

public:
	PowerUp(int spawn, btVector3 pos = btVector3(0, 1, 0), int type = 0, float duration = 10.f);
	~PowerUp();
	void SetPos(btVector3 pos);
	btVector3 GetPos();
	void SetType(int type);
	int GetType();
	void SetDuration(float duration);
	float GetDuration();
	btRigidBody* getObject();
	ObjectInfo* GetObjectInfo();
	bool update(float dt);
	int GetSpawn();
	void StartParticles();
	void BombBlown();
	bool IsBombBlown();
};
 