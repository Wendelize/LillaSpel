#pragma once

using namespace glm;

class Camera
{
private:
	vec3 m_direction, m_position, m_up, m_parallel;
	mat4 m_viewMatrix;
	GLFWwindow * m_window;

public:
	Camera(GLFWwindow * window, vec3 pos = { 0,0,2 }, vec3 point = { 0,0,0 }, vec3 up = { 0,1,0 });
	~Camera();

	vec3 GetDir();
	vec3 GetPos();
	vec3 GetUp();
	mat4 GetView();
	void ChangePos(vec3 pos);
	void ChangeDir(vec3 dir);
	void ChangeUp(vec3 up);
	void MoveCamera(vec3 newPos);
	void UpdateMovement(float delta, float speed);
};

Camera::Camera(GLFWwindow * window, vec3 pos, vec3 point, vec3 up)
{
	m_position = pos;
	m_direction = normalize(m_position - point);
	m_up = up;
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

inline mat4 Camera::GetView()
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
		//m_position += m_parallel * deltaTime * speed;
		pressed = true;
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
		//m_position -= m_parallel * deltaTime * speed;
		pressed = true;
	}
	m_viewMatrix = lookAt(m_position, m_direction, m_up);
}

