#include "Header Files/Include.h"

Camera::Camera(vec3 pos, vec3 point, vec3 up)
{
	m_position = pos;
	m_direction = normalize(m_position - point);
	m_parallel = normalize(cross({ 0, 1, 0 }, m_direction));
	m_up = normalize(cross(m_direction, m_parallel));
	m_viewMatrix = lookAt(m_position, m_direction, m_up);
}

Camera::~Camera()
{
}

vec3 Camera::GetDir()
{
	return m_direction;
}

vec3 Camera::GetPos()
{
	return m_position;
}

vec3 Camera::GetUp()
{
	return m_up;
}

mat4 Camera::GetView()
{
	return m_viewMatrix;
}

void Camera::ChangePos(vec3 pos)
{
	m_position = pos;
}

void Camera::ChangeDir(vec3 dir)
{
	m_direction = dir;
}

void Camera::ChangeUp(vec3 up)
{
	m_up = up;
}

void Camera::MoveCamera(vec3 newPos)
{
	m_position += newPos;
	m_viewMatrix = lookAt(m_position, m_position + m_direction, m_up);
}

void Camera::UpdateMovement(float deltaTime, float speed)
{
	m_viewMatrix = lookAt(m_position, m_direction, m_up);
}

