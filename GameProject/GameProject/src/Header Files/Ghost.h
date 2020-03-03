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

	int m_nrOfLightSwitch, m_nrOfCtrlSwitch, m_nrOfBombSwitch;
	bool m_lightSwitch, m_bombSwitch, m_ctrlSwitch;

public:
	Ghost();
	~Ghost();

	void UpdateGhost(float dt);

	int GetControllerID();
	bool GetBombSwitch();
	bool GetCtrlSwitch();
	bool GetLightSwitch();

	void SetControllerID(int index);
	void SetBombSwitch(bool state);
	void SetCtrlSwitch(bool state);
	void SetLightSwitch(bool state);

};
