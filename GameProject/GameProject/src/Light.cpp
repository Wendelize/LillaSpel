#include "Header Files/Include.h"


Light::Light() {

}

Light::~Light()
{
}

void Light::AddDirLight(vec3 dir, vec3 color)
{
	m_dirLight = DirLight(dir, color);
}

void Light::AddPointLight(vec3 pos, vec3 color)
{
	m_pointLights.push_back(PointLight(pos, color));
}

void Light::AddSpotLight(vec3 pos, vec3 color)
{
	m_spotLights.push_back(SpotLight(pos, color));
}
