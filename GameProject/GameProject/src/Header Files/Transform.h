#pragma once 
#include "Include.h" 

class Transform
{
private:
	mat4 m_modelMatrix;
	vec3 m_position;
	vec3 m_rotation;
	vec3 m_scale;
	bool m_isDirty;

	void calculateMatrix();
public:
	Transform();
	~Transform();

	void Translate(vec3 vec);
	void Rotate(vec3 vec);
	void Rotate(float pitch, float yaw, float roll);
	void Scale(float x, float y, float z);

	void SetTranslation(vec3 vec);
	void SetRotation(float pitch, float yaw, float roll);
	void SetScale(float x, float y, float z);

	mat4 GetMatrix();
};
