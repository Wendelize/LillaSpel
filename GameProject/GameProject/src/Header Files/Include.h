#pragma once
//INCLUDES 3D
#include <GL/glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "btBulletDynamicsCommon.h"
#define STB_IMAGE_IMPLEMENTATION

//IMGUI
#include "../IMGUI/imgui.h"
#include "../IMGUI/imgui_impl_opengl3.h"
#include "../IMGUI/imgui_impl_glfw.h"


//INCLUDES C++
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <ctime>

//MEMORY LEAK DETECTION
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#ifdef _DEBUG
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#else
//#define new new
//#endif

//CLASSES
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Mesh.h"
#include "Controller.h"
#include "Light.h"
#include "Skybox.h"
#include "ShadowMap.h"

//NAMESPACES
using namespace std;
using namespace glm;

struct ObjectInfo
{
	mat4 modelMatrix;
	int modelId; //Refererar till en array med modeller i Scene 
	int typeId; // 0 = vehicle, 1 = platform, 2 = power-up
	vec3 hue; // F�rgnyans

	ObjectInfo(mat4 m, int mID, int tID, vec3 h) {
		modelMatrix = m;
		modelId = mID;
		typeId = tID;
		hue = h;
	}
	~ObjectInfo() {

	}
};