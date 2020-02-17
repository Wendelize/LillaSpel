#pragma once
#include "Include.h"

class ShadowMap
{
public:

	ShadowMap(int width, int height);
	~ShadowMap();

	bool Init(unsigned int windowWidth = 2000, unsigned int windowHeight = 2000);
	void CalcLightSpaceMatrix(vector<Light*> light);
	mat4 GetLSP();
	Shader* GetShader();
	GLuint GetTexture();
	GLuint GetFBO();

private:
	GLuint m_FBO = 0;
	GLuint m_shadowMap = 0;

	mat4 m_lightSpaceMatrix = mat4(0);
	Shader* m_shadowShader;
};