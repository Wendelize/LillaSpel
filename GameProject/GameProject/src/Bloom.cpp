#include "Header Files/Bloom.h"

Bloom::Bloom()
{
	m_blur = new Shader("src/Shaders/GaussianBlurVS.glsl", "src/Shaders/GaussianBlurFS.glsl");
	m_bloom = new Shader("src/Shaders/BloomVS.glsl", "src/Shaders/BloomFS.glsl");
	Init();
	InitPingPong();


	m_blur->UseShader();
	m_blur->Uniform("u_BrightImage", 0);
	m_bloom->UseShader();
	m_bloom->Uniform("u_Scene", 0);
	m_bloom->Uniform("u_BloomBlur", 1);
}

Bloom::~Bloom()
{
	delete m_blur;
	delete m_bloom;
}

void Bloom::Init()
{
	glGenFramebuffers(1, &m_FBO1);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO1);

	glGenTextures(2, m_colorBuffers);

	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1920, 1080, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorBuffers[i], 0);
	}

	glGenRenderbuffers(1, &m_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth);

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Bloom::InitPingPong()
{
	glGenFramebuffers(2, m_pingPongFBO);
	glGenTextures(2, m_pingPongColorBuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_pingPongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, m_pingPongColorBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1920, 1080, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingPongColorBuffer[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Bloom::PingPongRender()
{
	m_horizontal = true;
	m_firstIteration = true;

	unsigned int amount = 10;
	m_blur->UseShader();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_pingPongFBO[m_horizontal]);
		m_blur->Uniform("u_Horizontal", m_horizontal);
		glBindTexture(GL_TEXTURE_2D, m_firstIteration ? m_colorBuffers[1] : m_pingPongColorBuffer[!m_horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
		RenderQuad();
		m_horizontal = !m_horizontal;
		if (m_firstIteration)
			m_firstIteration = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void Bloom::RenderBloom()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_bloom->UseShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_pingPongColorBuffer[!m_horizontal]);
	m_bloom->Uniform("u_Bloom", m_bool);
	m_bloom->Uniform("u_Exposure", m_exposure);
	RenderQuad();

}

void Bloom::RenderQuad()
{
	if (m_quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &m_quadVAO);
		glGenBuffers(1, &m_quadVBO);
		glBindVertexArray(m_quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(m_quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

Shader* Bloom::GetBlurShader()
{
	return m_blur;
}

Shader* Bloom::GetBloomShader()
{
	return m_bloom;;
}

unsigned int Bloom::getFBO()
{
	return m_FBO1;
}
