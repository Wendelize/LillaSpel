#pragma once
#include "Include.h"
//Vibrating controller
#include <windows.h>
#include <XInput.h>
#pragma comment(lib, "XInput.lib")

// axis[#]
// #0: Left sticker : left(-1) right(1) 
// #1: Left sticker : Up(-1) down(1) 
// #2: Right sticker: left(-1) right(1) 
// #3: Right sticker: Up(-1) down(1) 
// #4: Left trigger: pressed 1 
// #5: Right trigger: pressed 1 


class Controller
{
private:
	const float* m_axes;
	GLFWgamepadstate m_state;
	float m_vibrationTime = 0;

public:
	Controller();
	~Controller();

	const float GetLeftStickHorisontal(int ID);
	const float GetLeftStickVertical(int ID);
	const float GetLeftTrigger(int ID);
	const float GetRightTrigger(int ID);

	bool ButtonAIsPressed(int ID);
	bool ButtonBIsPressed(int ID);
	bool ButtonXIsPressed(int ID);
	bool ButtonYIsPressed(int ID);
	bool ButtonOptionsIsPressed(int ID);
	bool ButtonRightJoystickIsPressed(int ID);
	void Vibrate(int ID, int leftVal = 0, int rightVal = 0);
	float GetVibrationTime();
	void AddVibrationTime(float dt);
};
