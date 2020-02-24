#include "Header Files\Controller.h" 
 
Controller::Controller() 
{ 
	 
} 
 
Controller::~Controller() 
{ 
} 
 
const float Controller::GetLeftStickHorisontal(int ID) 
{		 
	int count; 
	m_axes = glfwGetJoystickAxes(ID, &count); 
	return m_axes[0];	 
}						 
						 
const float Controller::GetLeftStickVertical(int ID) 
{ 
	int count; 
	m_axes = glfwGetJoystickAxes(ID, &count); 
	return m_axes[1];		 
}						 
						 
const float Controller::GetLeftTrigger(int ID) 
{ 
	int count; 
	m_axes = glfwGetJoystickAxes(ID, &count); 
	return m_axes[4];		 
}						 
						 
const float Controller::GetRightTrigger(int ID) 
{ 
	int count; 
	m_axes = glfwGetJoystickAxes(ID, &count); 
	return m_axes[5]; 
} 
 
bool Controller::ButtonAIsPressed(int ID) 
{ 
	if (glfwGetGamepadState(ID, &m_state)) 
	{ 
		if (m_state.buttons[GLFW_GAMEPAD_BUTTON_A]) 
		{ 
			return true; 
		} 
	} 
	return false; 
} 
 
bool Controller::ButtonBIsPressed(int ID) 
{ 
	if (glfwGetGamepadState(ID, &m_state)) 
	{ 
		if (m_state.buttons[GLFW_GAMEPAD_BUTTON_B]) 
		{ 
			return true; 
		} 
	} 
	return false; 
} 
 
bool Controller::ButtonXIsPressed(int ID) 
{ 
	if (glfwGetGamepadState(ID, &m_state)) 
	{ 
		if (m_state.buttons[GLFW_GAMEPAD_BUTTON_X]) 
		{ 
			return true; 
		} 
	} 
	return false; 
} 
 
bool Controller::ButtonYIsPressed(int ID) 
{ 
	if (glfwGetGamepadState(ID, &m_state)) 
	{ 
		if (m_state.buttons[GLFW_GAMEPAD_BUTTON_Y]) 
		{ 
			return true; 
		} 
	} 
	return false; 
}
bool Controller::ButtonOptionsIsPressed(int ID)
{
	if (glfwGetGamepadState(ID, &m_state))
	{
		if (m_state.buttons[GLFW_GAMEPAD_BUTTON_START])
		{
			return true;
		}
	}
	return false;
}

bool Controller::ButtonRightJoystickIsPressed(int ID)
{
	if (glfwGetGamepadState(ID, &m_state))
	{
		if (m_state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB])
		{
			return true;
		}
	}
	return false;
}

void Controller::Vibrate(int ID, int leftVal, int rightVal)
{
	// Create a Vibraton State
	XINPUT_VIBRATION Vibration;

	// Zeroise the Vibration
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	// Vibrate the controller
	XInputSetState(ID, &Vibration);
	m_vibrationTime = 0;
}

float Controller::GetVibrationTime()
{
	return m_vibrationTime;
}

void Controller::AddVibrationTime(float dt)
{
	m_vibrationTime += dt;
}
