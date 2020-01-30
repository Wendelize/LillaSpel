#pragma once

struct DirLight
{
	vec3 dir, color;
	float ambient, diffuse, specular;

};

struct PointLight
{
	vec3 pos, color;
	float ambient, diffuse, specular;
	float constant, linear, quadratic;
};

struct SpotLight
{
	vec3 pos, color;
	float ambient, diffuse, specular;
	float cutOff;
	float CutOff(float deg) {
		return cos(radians(deg));
	}
};

class Light
{
private:

public:
	Light();
	~Light();

};
