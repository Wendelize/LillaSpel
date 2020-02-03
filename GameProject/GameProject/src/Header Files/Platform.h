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
public:
	Platform();
	~Platform();
	ObjectInfo* GetObjectInfo();
	int GetModelId();
	void SetModelId(int id);
	void SetSkyboxId(int id);
};

