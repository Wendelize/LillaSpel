#pragma once
//INCLUDES 3D
#include <GL/glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"


//INCLUDES C++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

//CLASSES
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "ObjectData.h"
#include "Model.h"
#include "Mesh.h"
#include "Controller.h"
#include "Light.h"
#include "Level.h"

//NAMESPACES
using namespace std;
using namespace glm;

struct ObjectInfo
{
	mat4 modelMatrix;
	int modelId; //Refererar till en array med modeller i Scene 
	int typeId; // 0 = vehicle, 1 = platform, 2 = power-up

	ObjectInfo(mat4 m, int a, int b) {
		modelMatrix = m;
		modelId = a;
		typeId = b;
	}
};