#pragma once
#include "Include.h"
#include "Transform.h"

class PowerUp
{
private:
	int m_type;
	float m_duration;
	int m_spawn;
	vec3 m_color;
	btBoxShape* m_shape;
	ObjectInfo* m_info;
	Transform* m_transform;
	btGhostObject* m_body;
	btTransform* m_btTransform;
	btVector3 m_pos;
	
public:
	PowerUp(int spawn, btVector3 pos = btVector3(0, 1, 0), int type = 0, float duration = 10.f);
	~PowerUp();
	void SetPos(btVector3 pos);
	btVector3 GetPos();
	void SetType(int type);
	int GetType();
	void SetDuration(float duration);
	float GetDuration();
	btGhostObject* getObject();
	ObjectInfo* GetObjectInfo();
	bool update(float dt);
	int GetSpawn();
};
 