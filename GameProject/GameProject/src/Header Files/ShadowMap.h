#pragma once

class ShadowMap
{
public:
	ShadowMap(vector<Light> light);
	~ShadowMap();

	bool Init(unsigned int windowWidth = 1024, unsigned int windowHeight = 1024);
	void BindForWriting();
	void BindForReading(GLenum texture);
private:
	GLuint m_FBO = 0;
	GLuint m_shadowMap = 0;
	mat4 m_lightSpaceMatrix = mat4(0);
	Shader m_shadowShader = Shader("src/Shaders/ShadowVS.glsl", "src/Shaders/ShadowFS.glsl");
};