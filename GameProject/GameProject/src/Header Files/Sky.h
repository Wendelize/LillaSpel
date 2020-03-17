#pragma once

#include "Include.h"

class Sky
{
private:
	GLuint m_texture[3];
	int m_width, m_height, m_nrChannels;
	Model *m_skyDome, *m_skyPlane;
	vec3 m_apexColor, m_centerColor;

	float m_translationSpeed[4];
	float m_textureTranslation[4];

public:
	Sky();
	~Sky();

	void Init();
	void Update(float dt);

	void RenderSkyDome(Shader* shader, mat4 model, mat4 view, mat4 proj);
	void RenderSkyPlane(Shader* shader, mat4 model, mat4 view, mat4 proj);
};
