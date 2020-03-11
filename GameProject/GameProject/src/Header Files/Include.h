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
#include "BulletCollision\CollisionDispatch\btCollisionWorld.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
//IMGUI
#include "../IMGUI/imgui.h"
#include "../IMGUI/imgui_impl_opengl3.h"
#include "../IMGUI/imgui_impl_glfw.h"
//include "stb_image.h"

//AUDIO
// irrKlang is not Creative Commons. If we want to release the game we need to change audio library. OpenAL perhaps.
#include "irrKlang.h"

//INCLUDES C++
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>
#include <cstdlib>
#include <ctime>

//MEMORY LEAK DETECTION
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//
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
#include "Sky.h"
#include "DebugDrawer.h"
#include "ShadowMap.h"
#include "Bloom.h"
#include "ParticleSystem.h"

//DEFINES 
#define NRDEATHSOUNDS 30
#define CAMERAPOS_SELECT vec3(0, 10, 50)
#define CAMERAPOS_LEVELSELECT vec3(0, 40, 3)
#define CAMERAPOS_GAME vec3(0, 22, 28)

#define SELECTPOS1 CAMERAPOS_SELECT + vec3(0, -1, 1) * 3.f + vec3(4.4, 0, 2)
#define SELECTPOS2 CAMERAPOS_SELECT + vec3(0, -1, 1) * 3.f + vec3(1.4, 0, 2)
#define SELECTPOS3 CAMERAPOS_SELECT + vec3(0, -1, 1) * 3.f + vec3(-1.4, 0, 2)
#define SELECTPOS4 CAMERAPOS_SELECT + vec3(0, -1, 1) * 3.f + vec3(-4.4, 0, 2)

//NAMESPACES
using namespace std;
using namespace glm;
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")

struct ObjectInfo
{
	mat4 modelMatrix;
	int modelId; //Refererar till en array med modeller i Scene 
	int typeId; // 0 = vehicle, 1 = platform, 2 = power-up
	vec3 hue; // F�rgnyans
	bool glow;

	ObjectInfo(mat4 m, int mID, int tID, vec3 h, bool g) {
		modelMatrix = m;
		modelId = mID;
		typeId = tID;
		hue = h;
		glow = g;
	}
	~ObjectInfo() {

	}
};

struct Hole {
	vec3 position;
	float depth;
};