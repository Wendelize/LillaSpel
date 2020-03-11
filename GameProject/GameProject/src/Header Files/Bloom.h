#pragma once
#include "Include.h"

class Bloom
{
private:
	Shader* m_blur, * m_bloom;
	unsigned int m_FBO1, m_pingPongFBO[2];
	unsigned int m_colorBuffers[2], m_depth, m_pingPongColorBuffer[2];
	unsigned int m_quadVAO = 0;
	unsigned int m_quadVBO;

	bool m_horizontal = true, m_firstIteration = true, m_bool = true;
	float m_exposure = 1.0f;

	int m_width;
	int m_height;
	float m_resolutionScale;

public:
	Bloom(int width, int height, float m_resolutionScale);
	~Bloom();

	void Init();
	void InitPingPong();
	void PingPongRender(int nrOfSteps);
	void RenderBloom(GLFWwindow* w);
	void RenderQuad();

	void EnableBloom(bool b);

	Shader* GetBlurShader();
	Shader* GetBloomShader();

	unsigned int getFBO();


};
