#pragma once

#include "Include.h"
#include "Transform.h"

using namespace std;
using namespace glm;

class Platform
{
private:
	Transform* m_transform;
	ObjectInfo* m_info;
	int m_modelId;
	int m_skyboxId;

	//Variables 4 Fysik
	btRigidBody* m_body;
	btCollisionShape* m_platformShape;
	btTransform* m_btTransform;
	btDefaultMotionState* m_motionState;
public:
	Platform(Model* model, int platformIndex);
	~Platform();
	ObjectInfo* GetObjectInfo();
	int GetModelId();
	btRigidBody* getBody();
	void SetModelId(int id);
	void SetSkyboxId(int id);
};
