#pragma once

struct DirLight
{
	vec3 dir, color;
	float ambient, diffuse, specular;
	DirLight(vec3 d = vec3( 0.0, -1.0, 0.0 ), vec3 c = vec3(0.5), float a = 0.01f, float dif = 0.5f, float s = 0.1f) {
		dir = d;
		color = c;
		ambient = a;
		diffuse = dif;
		specular = s;
	}
};

struct PointLight
{
	vec3 pos, color;
	float ambient, diffuse, specular;
	float constant = 1.0f, linear  = 0.09f, quadratic = 0.032f;
	PointLight(vec3 p = vec3(0,1,0), vec3 c = vec3(0,0,1), float a = 0.05f, float dif = 0.8f, float s = 1.0f) {
		pos = p;
		color = c;
		ambient = a;
		diffuse = dif;
		specular = s;
	}
};

struct SpotLight
{
	vec3 pos, color;
	float ambient, diffuse, specular;
	float constant = 1.0f, linear = 0.09f, quadratic = 0.032f;
	float cutOff;

	SpotLight(vec3 p = vec3(0,1,0), vec3 c = vec3(0,0,1), float deg = 24, float a = 0.5f, float dif = 0.6f, float s = 1.0f) {
		pos = p;
		color = c;
		cutOff = CutOff(deg);
		ambient = a;
		diffuse = dif;
		specular = s;
	}

	float CutOff(float deg) {
		return cos(radians(deg));
	}

};

class Light
{
private:

public:
	DirLight m_dirLight;
	vector<SpotLight> m_spotLights;
	vector<PointLight> m_pointLights;

	Light();
	~Light();

	void AddDirLight(vec3 dir, vec3 color);
	void AddPointLight(vec3 pos, vec3 color);
	void AddSpotLight(vec3 pos, vec3 color);
	//void ChangePos(vec3 pos);
	//void ChangeColor(vec3 color);
	//void ChangeLightIntensity(float ambient, float diffuse, float specular);
	//void ChangeCutOff(float deg);

};
