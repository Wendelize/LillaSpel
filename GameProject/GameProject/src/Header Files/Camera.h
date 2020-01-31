#pragma once

class Camera
{
private:
	vec3 m_direction, m_position, m_up, m_parallel;
	mat4 m_viewMatrix;

public:
	Camera(vec3 pos = { 0,0,2 }, vec3 point = { 0,0,0 }, vec3 up = { 0,1,0 });
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

