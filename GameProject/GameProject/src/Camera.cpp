#include "Header Files/Include.h"

Camera::Camera(vec3 pos, vec3 point, vec3 up)
{
	m_position = pos;
	m_direction = normalize(m_position - point);
	m_parallel = normalize(cross({ 0,1,0 }, m_direction));
	m_up = normalize(cross(m_direction, m_parallel));
	m_focusPoint = vec3(0, -6, 3);
	m_viewMatrix = lookAt(m_position, m_focusPoint, m_up);
	m_focusTranslation = vec3(0);
	m_focusTranslationPoint = vec3(0);
	m_translationDistance = 0;
}

Camera::~Camera()
{
}

void Camera::SetFocusPoint(vec3 p)
{
	m_focusPoint = p;
	m_direction = normalize(m_position - m_focusPoint);
	m_parallel = normalize(cross({ 0,1,0 }, m_direction));
	m_up = normalize(cross(m_direction, m_parallel));
	m_viewMatrix = lookAt(m_position, m_focusPoint, m_up);
}

void Camera::TranslateFocusPoint(vec3 p)
{
	m_focusTranslationPoint = p;
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
	m_viewMatrix = lookAt(m_position, vec3(0, -6, 3), m_up);
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
	m_focusTranslation = m_focusTranslationPoint - m_focusPoint;
	m_translationDistance = m_focusTranslation.length();

	m_focusPoint += m_focusTranslation * deltaTime * speed;
	m_direction = normalize(m_position - m_focusPoint);
	m_parallel = normalize(cross({ 0,1,0 }, m_direction));
	m_up = normalize(cross(m_direction, m_parallel));
	m_viewMatrix = lookAt(m_position, m_focusPoint, m_up);

}

