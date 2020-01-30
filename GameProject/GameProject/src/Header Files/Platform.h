#pragma once

#include "Include.h"
#include "Transform.h"

using namespace std;
using namespace glm;

class Platform
{
private:
	Transform* m_transform;
	int m_modelId;
public:
	Platform();
	~Platform();
	//ObjectInfo* GetObjectInfo();
};

