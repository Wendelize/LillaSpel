#pragma once
#include "Include.h"
#include "Transform.h"

class Object
{
public:
	Object(btVector3 pos, int type, Model* model);
	~Object();
	ObjectInfo* GetObjectInfo();
	btRigidBody* getObject();
	void update();

private:
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
};