#pragma once
#include "Include.h"

class AntiAliasing
{
private:
	Shader* m_shader;
	unsigned int m_FBO = 0, m_multiSampled = 0, m_renderBuffer = 0;
	unsigned int m_secondFBO = 0, m_screenTexture;
public:
	AntiAliasing(int width, int height);
	~AntiAliasing();

	void Init(int width, int height);
	unsigned int GetTexture();
	unsigned int GetFBO();
	unsigned int GetSecondFBO();
};

