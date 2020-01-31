#pragma once 
#include "Include.h" 
 
// #0: Left sticker : left(-1) right(1) 
// #1: Left sticker : Up(-1) down(1) 
// #2: Right sticker: left(-1) right(1) 
// #3: Right sticker: Up(-1) down(1) 
// #4: Left trigger: pressed 1 
// #5: Right trigger: pressed 1 
// S�ger att size �r 4?  
 
class Controller 
{ 
private: 
	const float* axes; 
	GLFWgamepadstate state; 
 
public:  
	Controller(); 
	~Controller(); 
	 
	const float GetLeftStickHorisontal(int ID); 
	const float GetLeftStickVertical(int ID); 
	const float GetLefTrigger(int ID); 
	const float GetRightTrigger(int ID); 
 
	bool ButtonAIsPressed(int ID); 
	bool ButtonBIsPressed(int ID); 
	bool ButtonXIsPressed(int ID); 
	bool ButtonYIsPressed(int ID); 
	bool ButtonOptionsIsPressed(int ID);
	 
};