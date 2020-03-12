#include "Header Files/Include.h"


Light::Light(int i, vec3 dir, vec3 pos, vec3 color, float cutOff)
{
	switch (i)
	{
	case 0:
		m_type = i;
		m_dir = dir;
		m_pos = vec3(10, 10, 0);
		m_color = color;
		break;
	case 1:
		m_type = i;
		m_pos = pos;
		m_color = color;
		break;
	case 2:
		m_type = i;
		m_pos = pos;
		m_dir = dir;
		m_color = color;
		m_cutOff = CutOff(cutOff);
		m_outerCutOff = CutOff(cutOff + 5.0f);
		break;
	default:
		break;
	}
	m_ambient = 0.05f;
	m_diffuse = 0.7f;
	m_specular = 1.0f;
}


Light::~Light()
{
}

int Light::GetType()
{
	return m_type;
}

vec3 Light::GetPos()
{
	return m_pos;
}

vec3 Light::GetColor()
{
	return m_color;
}

vec3 Light::GetDirection()
{
	return m_dir;
}

float Light::GetOuterCutOff()
{
	return m_outerCutOff;
}

float Light::GetCutOff()
{
	return m_cutOff;
}

float Light::GetAmbient()
{
	return m_ambient;
}

float Light::GetDiffuse()
{
	return m_diffuse;
}

float Light::GetSpecular()
{
	return m_specular;
}

void Light::ChangePos(vec3 p)
{
	m_pos = p;
}

void Light::ChangeColor(vec3 c)
{
	m_color = c;
}
