#pragma once

#include "Include.h"
#include "stb_image.h"

using namespace std;
using namespace glm;

struct levelData {
	vec3 pos;
	vec3 normal;
	vec3 color;
	vec2 uv;
};

class Level
{
private:
	vertexData* m_verticesPlane;
	unsigned int* m_indicesPlane;
	vertexData* m_verticesSkybox;
	unsigned int* m_indicesSkybox;

	GLuint m_nrOfVPlane = 0;
	GLuint m_nrOfIPlane = 0;
	GLuint m_nrOfVSkybox = 0;
	GLuint m_nrOfISkybox = 0;

	float m_size;
	mat4 m_modelMatrix;
	int m_type;
	GLuint m_texturePlane;
	GLuint m_textureSkybox;
	int m_width, m_height, m_nrChannels;

	GLsizeiptr vertexBufferSizePlane()const {
		return (m_nrOfVPlane * sizeof(vertexData));
	}

	GLsizeiptr indexBufferSizePlane()const {
		return (m_nrOfIPlane * sizeof(unsigned int));
	}

	GLsizeiptr vertexBufferSizeSkybox()const {
		return (m_nrOfVSkybox * sizeof(vertexData));
	}

	GLsizeiptr indexBufferSizeSkybox()const {
		return (m_nrOfISkybox * sizeof(unsigned int));
	}

public:
	// VARIABLES
	GLuint m_vertexBufferPlane, m_indicesBufferPlane, m_vertexArrayPlane;
	GLuint m_vertexBufferSkybox, m_indicesBufferSkybox, m_vertexArraySkybox;

	//FUNCTIONS
	Level(float size);
	~Level();

	void CreatePlane();
	void CreateSkybox();
	void InitLevel();
	void DrawPlane(Shader* shaderProgram);
	void DrawSkybox(Shader* shaderProgram);
	void DrawLevel(Shader* shaderProgramPlane, Shader* shaderProgramSkybox);
	mat4 GetModelMat();
	void LoadPlaneTexture();
	void LoadSkyboxTexture();
	void BindTexture();
	void Bind3DTexture();

	// Future things?
	// void LoadHeightmap();
};
