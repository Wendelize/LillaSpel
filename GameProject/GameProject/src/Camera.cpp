#include "Header Files/Include.h"

Camera::Camera(vec3 pos, vec3 point, vec3 up)
{
	m_position = pos;
	m_translationPoint = pos;
	m_direction = normalize(m_position - point);
	m_parallel = normalize(cross({0, 1, 0}, m_direction));
	m_up = normalize(cross(m_direction, m_parallel));
	m_focusPoint = CAMERAPOS_SELECT + vec3(0, -1, 1);
	m_viewMatrix = lookAt(m_position, m_focusPoint, m_up);
	m_focusTranslation = vec3(0);
	m_focusTranslationPoint = vec3(0);
	m_translationDistance = 0;
	m_positionTranslation = vec3(0);
	m_shakeCounter = 0;
}

Camera::~Camera()
{
}

void Camera::SetFocusPoint(vec3 p)
{
	m_focusPoint = p;
	m_direction = normalize(m_position - m_focusPoint);
	m_parallel = normalize(cross({0, 1, 0}, m_direction));
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

void Camera::Shake(float intensity, float duration)
{
	m_shakeCounter = duration;
	m_shakeIntensity = intensity;
	m_posBeforeShake = m_position;
	m_shakeFade = 1;
	k = 0;
}

void Camera::ChangePos(vec3 pos)
{
	m_position = pos;
	m_translationPoint = pos;
	m_viewMatrix = lookAt(m_position, vec3(0, -6, 3), m_up);
}

void Camera::TranslatePos(vec3 pos, float speed)
{
	m_speed = speed;
	m_translationPoint = pos;
	m_positionTranslation = m_translationPoint - m_position;
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

void Camera::UpdateMovement(float deltaTime)
{
	if (m_shakeCounter > 0)
	{
		m_position.x = m_posBeforeShake.x - sin(m_shakeCounter * 83) * m_shakeFade * m_shakeIntensity;
		m_position.y = m_posBeforeShake.y - sin(m_shakeCounter * 71) * m_shakeFade * m_shakeIntensity;

		k++;
		if (k >= 2)
		{
			k = 0;
			m_shakeFade = m_shakeCounter * 0.9f;
		}

		m_shakeCounter -= deltaTime;
	}
	m_positionTranslation = m_translationPoint - m_position;
	m_position += m_positionTranslation * deltaTime * m_speed;

	m_focusTranslation = m_focusTranslationPoint - m_focusPoint;
	m_translationDistance = m_focusTranslation.length();

	m_focusPoint += m_focusTranslation * deltaTime * m_speed;
	m_direction = normalize(m_position - m_focusPoint);
	m_parallel = normalize(cross({0, 1, 0}, m_direction));
	m_up = normalize(cross(m_direction, m_parallel));
	m_viewMatrix = lookAt(m_position, m_focusPoint, m_up);
}
