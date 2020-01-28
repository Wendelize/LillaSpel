#include "Header Files/Include.h"

Camera::Camera(GLFWwindow* window, vec3 pos, vec3 point, vec3 up)
{
	m_position = pos;
	m_direction = normalize(m_position - point);
	m_up = up;
	m_parallel = { 1, 0, 0 };
	m_window = window;
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
	bool pressed = false;
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
		m_position += m_direction * deltaTime * speed;
		pressed = true;

	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
		m_position -= m_direction * deltaTime * speed;
		pressed = true;
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
		m_position += m_parallel * deltaTime * speed;
		pressed = true;
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
		m_position -= m_parallel * deltaTime * speed;
		pressed = true;
	}
	m_viewMatrix = lookAt(m_position, m_direction, m_up);
}

