#pragma once

using namespace glm;

class Camera
{
private:
	vec3 _direction, _position, _up;
	mat4 _viewMatrix;
	GLFWwindow * _window;

public:
	Camera(GLFWwindow * window, vec3 pos = { 0,0,2 }, vec3 point = { 0,0,0 }, vec3 up = { 0,1,0 });
	~Camera();

	vec3 getDir();
	vec3 getPos();
	vec3 getUp();
	mat4 getView();
	void changePos(vec3 pos);
	void changeDir(vec3 dir);
	void changeUp(vec3 up);
	void moveCamera(vec3 newPos);
	void update(float delta, float speed);
};

Camera::Camera(GLFWwindow * window, vec3 pos, vec3 point, vec3 up)
{
	_position = pos;
	_direction = normalize(_position - point);
	_up = up;
	_window = window;
	_viewMatrix = lookAt(_position, _direction, _up);
}

Camera::~Camera()
{
}

vec3 Camera::getDir() 
{
	return _direction;
}

vec3 Camera::getPos()
{
	return _position;
}

vec3 Camera::getUp()
{
	return _up;
}

inline mat4 Camera::getView()
{
	return _viewMatrix;
}

void Camera::changePos(vec3 pos)
{
	_position = pos;
}

void Camera::changeDir(vec3 dir)
{
	_direction = dir;
}

void Camera::changeUp(vec3 up)
{
	_up = up;
}

void Camera::moveCamera(vec3 newPos)
{
	_position += newPos;
	_viewMatrix = lookAt(_position, _position + _direction, _up);
}

void Camera::update(float deltaTime, float speed)
{
	bool pressed = false;
	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
		_position += _direction * deltaTime * speed;
		pressed = true;
		
	}
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
		_position -= _direction * deltaTime * speed;
		pressed = true;
	}
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
		//_position += paralell * deltaTime * speed;
		pressed = true;
	}
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
		//_position -= paralell * deltaTime * speed;
		pressed = true;
	}
	_viewMatrix = lookAt(_position, _direction, _up);
}

