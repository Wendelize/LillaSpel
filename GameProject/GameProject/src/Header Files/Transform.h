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
	vec3 m_forward;

	void calculateMatrix();
public:
	Transform();
	~Transform();
	mat4 GetPhysicsMatrix(btMatrix3x3 matrix);
	void Translate(vec3 vec);
	void Rotate(vec3 vec);
	void Rotate(float pitch, float yaw, float roll);
	void Scale(float x, float y, float z);
	vec3 TranslateDirection(vec3 vec);

	void SetTranslation(vec3 vec);
	void SetRotation(float pitch, float yaw, float roll);
	void SetScale(float x, float y, float z);

	vec3 GetForward();
	void SetForward(vec3 forward);
	vec3 GetPos();
	mat4 GetMatrix();
};
