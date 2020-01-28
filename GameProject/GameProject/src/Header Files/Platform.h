#pragma once

#include "Include.h"

using namespace std;
using namespace glm;

class Platform
{
private:
	// Cubemap... 3D texture...
public:
	Object* m_object;

	Platform();
	~Platform();
	Object* GetObject();
	void SetObject(Object* theObject);
	// LoadHeightMap();
	// Textureish();
};

