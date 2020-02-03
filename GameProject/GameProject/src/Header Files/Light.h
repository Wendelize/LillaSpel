#pragma once

class Light
{
public:

	Light(int i, vec3 dir = { 0, -1, 0 }, vec3 pos = { 2,2,2 }, vec3 color = { 0,1,0 }, float cutOff = 12.0);
	~Light();


	int GetType();
	vec3 GetPos();
	vec3 GetColor();
	vec3 GetDirection();
	float GetOuterCutOff();
	float GetCutOff();
	float GetAmbient();
	float GetDiffuse();
	float GetSpecular();

	void ChangePos(vec3 p);
	void ChangeColor(vec3 c);

	
private:
	// 0 = dirLight, 1 = pointLight, 2 = spotLight
	int m_type;
	vec3 m_pos, m_dir, m_color;
	float m_ambient, m_diffuse, m_specular;
	float m_cutOff, m_outerCutOff;

	float CutOff(float deg) {
		return cos(radians(deg));
	}

};
