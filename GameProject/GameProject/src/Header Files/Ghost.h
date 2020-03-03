#pragma once
#include "Include.h"
#include <stdlib.h>

class Ghost
{
private:
	Controller* m_controller;
	int m_controllerID;

	const char* m_soundFiles[10];

	ISoundEngine* m_honkEngine;
	ISoundEngine* m_tauntEngine;
	ISound* m_honk;
	ISound* m_taunt;
	vector<ISoundSource*> m_sounds;

	float m_timeOut;

	int m_nrOfLightOff, m_nrOfReverseController, m_nrOfBombs;

public:
	Ghost();
	~Ghost();

	void UpdateGhost(float dt);

	int GetControllerID();

	void SetControllerID(int index);


};
