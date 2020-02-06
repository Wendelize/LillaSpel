#include "Header Files/Transform.h" 

Transform::Transform()
{
	m_position = vec3(0.f, 0.f, 0.f);
	m_scale = vec3(1.f, 1.f, 1.f);
	m_rotation = vec3(0.f, 0.f, 0.f);
	//Vad ska forward s�ttas till?
	m_forward = vec3(0.f, 0.f, 1.f);
	

	m_modelMatrix = mat4(1.0f); // Identity Matrix 

	m_isDirty = false;
}

Transform::~Transform()
{
}

mat4 Transform::GetPhysicsMatrix(btMatrix3x3 matrix)
{

	vec3 temp1 = vec3(matrix.getColumn(0).x(), matrix.getColumn(0).y(), matrix.getColumn(0).z());
	vec3 temp2 = vec3(matrix.getColumn(1).x(), matrix.getColumn(1).y(), matrix.getColumn(1).z());
	vec3 temp3 = vec3(matrix.getColumn(2).x(), matrix.getColumn(2).y(), matrix.getColumn(2).z());
	return mat4(mat3(temp1, temp2, temp3));
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

void Transform::Translate(vec3 vec)
{
	m_position += vec;
	m_isDirty = true;
}

void Transform::Rotate(vec3 vec)
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

vec3 Transform::TranslateDirection(vec3 vec)
{
	Rotate(vec);

	mat4 transformMatrix = mat4(1.0f);
	transformMatrix = translate(transformMatrix, m_position);
	transformMatrix = rotate(transformMatrix, m_rotation.x, vec3(1.f, 0.f, 0.f));
	transformMatrix = rotate(transformMatrix, m_rotation.y, vec3(0.f, 1.f, 0.f));
	transformMatrix = rotate(transformMatrix, m_rotation.z, vec3(0.f, 0.f, 1.f));
	transformMatrix = scale(transformMatrix, m_scale);
	
	
	m_forward = vec3( m_transformMatrix * vec4(0,0,1,0));
	m_isDirty = true;

	return m_forward;
}

void Transform::SetTranslation(vec3 vec)
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

vec3 Transform::GetForward()
{
	return m_forward;
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