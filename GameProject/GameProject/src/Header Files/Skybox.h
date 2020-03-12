#pragma once

#include "Include.h"

struct skyboxData
{
	vec3 pos;
};

class Skybox
{
private:
	skyboxData* m_verticesSkybox;
	unsigned int* m_indicesSkybox;

	GLuint m_nrOfVSkybox = 0;
	GLuint m_nrOfISkybox = 0;

	GLuint m_textureSkybox;
	int m_width, m_height, m_nrChannels;

	GLsizeiptr vertexBufferSizeSkybox() const
	{
		return (m_nrOfVSkybox * sizeof(VertexData));
	}

	GLsizeiptr indexBufferSizeSkybox() const
	{
		return (m_nrOfISkybox * sizeof(unsigned int));
	}

public:
	GLuint m_vertexBufferSkybox, m_indicesBufferSkybox, m_vertexArraySkybox;

	Skybox();
	~Skybox();
	void CreateSkybox();
	void InitSkybox();
	void DrawSkybox(Shader* shaderProgram);
	void LoadSkyboxTexture();
	void Bind3DTexture();
};
