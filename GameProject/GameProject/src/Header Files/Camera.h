#pragma once

class Camera
{
private:
	vec3 m_direction, m_position, m_up, m_parallel, m_focusPoint, m_focusTranslation, m_focusTranslationPoint, m_positionTranslation, m_translationPoint;
	float m_translationDistance;
	mat4 m_viewMatrix;

	vec3 m_posBeforeShake;
	float m_shakeCounter;
	float m_shakeIntensity;
	float m_shakeFade;
	float m_speed = 1.f;
	int k; // används för att sänka m_shakefade varannan iteration.

public:
	Camera(vec3 pos = { 0,0,2 }, vec3 point = { 0,0,0 }, vec3 up = { 0,1,0 });
	~Camera();

	void SetFocusPoint(vec3 p);
	void TranslateFocusPoint(vec3 p);
	void TranslatePos(vec3 pos, float speed);
	vec3 GetDir();
	vec3 GetPos();
	vec3 GetUp();
	mat4 GetView();
	void Shake(float intensity, float duration);
	void ChangePos(vec3 pos);
	void ChangeDir(vec3 dir);
	void ChangeUp(vec3 up);
	void MoveCamera(vec3 newPos);
	void UpdateMovement(float delta);
};

