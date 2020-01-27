#pragma once

#include "Include.h"

using namespace std;
using namespace glm;

class Platform
{
private:
	Object m_object;
	// Cubemap... 3D texture...
public:
	Platform();
	~Platform();
	Object GetObject();
	Object SetObject();
	// LoadHeightMap();
	// Textureish();
};

