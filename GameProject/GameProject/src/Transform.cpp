#include "Header Files/Transform.h" 

Transform::Transform()
{
	m_position = vec3(0.f, 0.f, 0.f);
	m_scale = vec3(1.f, 1.f, 1.f);
	m_rotation = vec3(0.f, 0.f, 0.f);

	m_modelMatrix = mat4(1.0f); // Identity Matrix 

	m_isDirty = false;
}

Transform::~Transform()
{
}

void Transform::calculateMatrix()
{

	mat4 translationMatrix = translate(mat4(1.0f), m_position);
	mat4 scaleMatrix = scale(mat4(1.0f), m_scale);
	mat4 rotationMatrix = rotate(mat4(1.0f), m_rotation.x, vec3(1.f, 0.f, 0.f)) *
		rotate(mat4(1.0f), m_rotation.y, vec3(0.f, 1.f, 0.f)) *
		rotate(mat4(1.0f), m_rotation.z, vec3(0.f, 0.f, 1.f));

	m_modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

}

void Transform::Translate(vec3& vec)
{
	m_position += vec;
	m_isDirty = true;
}

void Transform::Rotate(vec3& vec)
{
	m_rotation += vec;
	m_isDirty = true;
}

void Transform::Rotate(float pitch, float yaw, float roll)
{
	m_rotation += vec3(pitch, yaw, roll);
	m_isDirty = true;
}

void Transform::Scale(float x, float y, float z)
{
	m_scale += vec3(x, y, z);
	m_isDirty = true;
}

void Transform::SetTranslation(vec3& vec)
{
	m_position = vec;
	m_isDirty = true;
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
	m_rotation = vec3(pitch, yaw, roll);
	m_isDirty = true;
}

void Transform::SetScale(float x, float y, float z)
{
	m_scale = vec3(x, y, z);
	m_isDirty = true;
}

mat4 Transform::GetMatrix()
{
	if (m_isDirty)
	{
		calculateMatrix();
		m_isDirty = false;
	}

	return m_modelMatrix;
}