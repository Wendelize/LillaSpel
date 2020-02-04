#pragma once
#include "Include.h"

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	bool Init(unsigned int windowWidth = 1024, unsigned int windowHeight = 1024);
	void BindForWriting();
	void BindForReading(GLenum texture);
	void RenderShadowMap(vector<Light> light);
private:
	GLuint m_FBO = 0;
	GLuint m_shadowMap = 0;
	mat4 m_lightSpaceMatrix = mat4(0);
	Shader * m_shadowShader ;
};