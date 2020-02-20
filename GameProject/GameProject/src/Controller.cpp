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
	axes = glfwGetJoystickAxes(ID, &count); 
	return axes[0];	 
}						 
						 
const float Controller::GetLeftStickVertical(int ID) 
{ 
	int count; 
	axes = glfwGetJoystickAxes(ID, &count); 
	return axes[1];		 
}						 
						 
const float Controller::GetLefTrigger(int ID) 
{ 
	int count; 
	axes = glfwGetJoystickAxes(ID, &count); 
	return axes[4];		 
}						 
						 
const float Controller::GetRightTrigger(int ID) 
{ 
	int count; 
	axes = glfwGetJoystickAxes(ID, &count); 
	return axes[5]; 
} 
 
bool Controller::ButtonAIsPressed(int ID) 
{ 
	if (glfwGetGamepadState(ID, &state)) 
	{ 
		if (state.buttons[GLFW_GAMEPAD_BUTTON_A]) 
		{ 
			return true; 
		} 
	} 
	return false; 
} 
 
bool Controller::ButtonBIsPressed(int ID) 
{ 
	if (glfwGetGamepadState(ID, &state)) 
	{ 
		if (state.buttons[GLFW_GAMEPAD_BUTTON_B]) 
		{ 
			return true; 
		} 
	} 
	return false; 
} 
 
bool Controller::ButtonXIsPressed(int ID) 
{ 
	if (glfwGetGamepadState(ID, &state)) 
	{ 
		if (state.buttons[GLFW_GAMEPAD_BUTTON_X]) 
		{ 
			return true; 
		} 
	} 
	return false; 
} 
 
bool Controller::ButtonYIsPressed(int ID) 
{ 
	if (glfwGetGamepadState(ID, &state)) 
	{ 
		if (state.buttons[GLFW_GAMEPAD_BUTTON_Y]) 
		{ 
			return true; 
		} 
	} 
	return false; 
}
bool Controller::ButtonOptionsIsPressed(int ID)
{
	if (glfwGetGamepadState(ID, &state))
	{
		if (state.buttons[GLFW_GAMEPAD_BUTTON_START])
		{
			return true;
		}
	}
	return false;
}

bool Controller::ButtonRightJoystickIsPressed(int ID)
{
	if (glfwGetGamepadState(ID, &state))
	{
		if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB])
		{
			return true;
		}
	}
	return false;
}
