#pragma once
#include "Include.h"
#include "Transform.h"

class Object
{
public:
	Object(btVector3 pos, int type, Model* model, float speed = 1, float scale = 1);
	~Object();
	ObjectInfo* GetObjectInfo();
	btRigidBody* GetObject();
	void Update();
	void SetScale(float scale);
	float GetScale();
	void SetRotation(float degrees);
	void Move(vec3 dir);
	vec3 GetPos();
	void SetWay(bool way);
	bool GetWay();
	void rotateAroundCenter(float dt);
	void rotate(float dt);
private:
	bool m_way = false;
	btTriangleMesh m_tetraMesh;
	btConvexHullShape* m_physicsMesh;
	btRigidBody* m_body;
	btTransform* m_btTransform;
	btDefaultMotionState* m_motionState;
	btVector3 m_currentPos;
	int m_model;
	vec3 m_color;
	Transform* m_transform;
	ObjectInfo* m_info;
	float m_scale;
	float m_speed;
};