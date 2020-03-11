#include "Header Files/Sky.h"
#include "Header Files/stb_image.h"

Sky::Sky()
{
	m_skyPlane = nullptr;

	m_skyDome = new Model("src/Models/SkyDome.obj");
	m_skyPlane = new Model("src/Models/SkyPlane.obj");

	Init();
}

Sky::~Sky()
{
	delete m_skyDome;
	delete m_skyPlane;
}

void Sky::Init()
{
	m_centerColor = vec3(0.5, 0.0, 0.3);
	m_apexColor = vec3(0.0, 0.0, 0.0);

	m_translationSpeed[0] = 0.115f;
	m_translationSpeed[1] = 0.04f;
	m_translationSpeed[2] = 0.15f;
	m_translationSpeed[3] = 0.05f;

	m_textureTranslation[0] = 0.4f;
	m_textureTranslation[1] = 0.0f;
	m_textureTranslation[2] = 0.9f;
	m_textureTranslation[3] = 0.8f;


	glGenTextures(3, m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture[0]);
	unsigned char* texture = stbi_load("src/Textures/c.gif", &m_width, &m_height, &m_nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	stbi_image_free(texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, m_texture[1]);
	unsigned char* texture2 = stbi_load("src/Textures/c2.gif", &m_width, &m_height, &m_nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture2);
	stbi_image_free(texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, m_texture[2]);
	unsigned char* texture3 = stbi_load("src/Textures/earth2.jpg", &m_width, &m_height, &m_nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture3);
	stbi_image_free(texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Sky::Update(float dt)
{
	m_textureTranslation[0] += m_translationSpeed[0] * dt;
	m_textureTranslation[1] += m_translationSpeed[1] * dt;
	m_textureTranslation[2] += m_translationSpeed[2] * dt;
	m_textureTranslation[3] += m_translationSpeed[3] * dt;

	if (m_textureTranslation[0] > 1.0f) 
		m_textureTranslation[0] -= 1.0f;

	if (m_textureTranslation[1] > 1.0f) 
		m_textureTranslation[1] -= 1.0f;

	if (m_textureTranslation[2] > 1.0f) 
		m_textureTranslation[2] -= 1.0f;

	if (m_textureTranslation[3] > 1.0f) 
		m_textureTranslation[3] -= 1.0f;

}

void Sky::RenderSkyDome(Shader* shader, mat4 model, mat4 view, mat4 proj)
{
	shader->UseShader();
	shader->SetUniform("u_Model", model);
	shader->SetUniform("u_View", view);
	shader->SetUniform("u_Projection", proj);

	shader->SetUniform("u_ApexColor", m_apexColor);
	shader->SetUniform("u_CenterColor", m_centerColor);

	m_skyDome->Draw(shader);
}

void Sky::RenderSkyPlane(Shader* shader, mat4 model, mat4 view, mat4 proj)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->UseShader();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);
	shader->SetTexture2D(0, "u_Texture1", m_texture[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture[1]);
	shader->SetTexture2D(1, "u_Texture2", m_texture[1]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_texture[2]);
	shader->SetTexture2D(2, "u_Texture3", m_texture[2]);

	shader->SetUniform("u_Model", model);
	shader->SetUniform("u_View", view);
	shader->SetUniform("u_Projection", proj);
	shader->SetUniform("u_firstTranslationX", m_textureTranslation[0]);
	shader->SetUniform("u_firstTranslationZ", m_textureTranslation[1]);
	shader->SetUniform("u_secondTranslationX", m_textureTranslation[2]);
	shader->SetUniform("u_secondTranslationZ", m_textureTranslation[3]);


	

	m_skyPlane->Draw(shader);

	glDisable(GL_BLEND);
}
