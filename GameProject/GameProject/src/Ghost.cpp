#include "Header Files\Ghost.h"

Ghost::Ghost()
{
	m_controller = new Controller();

	m_controllerID = 0;
	m_nrOfBombSwitch = 2;
	m_nrOfLightSwitch = 2;
	m_nrOfCtrlSwitch = 1;

	m_bombSwitch = false;
	m_ctrlSwitch = false;
	m_lightSwitch = false;

	m_honkEngine = createIrrKlangDevice();
	m_tauntEngine = createIrrKlangDevice();

	// Create sound
	if (m_honkEngine && m_tauntEngine)
	{
		m_sounds.push_back(m_honkEngine->addSoundSourceFromFile("src/Audio/Player - HonkDELUXE.mp3"));

		m_tauntEngine->setSoundVolume(0.3);
		m_taunt = m_tauntEngine->play2D(m_sounds[0], false, true);

		m_honkEngine->setSoundVolume(0.2f);
		m_honk = m_honkEngine->play2D(m_sounds[0], false, true);
	}

	// SpookSounds
}

Ghost::~Ghost()
{
	delete m_controller;


	if (m_honkEngine )
	{
		for (uint i = 0; i < m_sounds.size(); i++)
		{
			m_sounds[i]->drop();
		}
		m_sounds.clear();

		m_honk->drop();
		m_taunt->drop();
		m_tauntEngine->drop(); 
	}
}

void Ghost::UpdateGhost(float dt)
{
	m_timeOut += dt;
	const char* filename;
	int randomNumber = rand() % 10;

	if (glfwJoystickPresent(m_controllerID))
	{
		if (m_honkEngine && m_controller->ButtonRightJoystickIsPressed(m_controllerID))
		{
			m_honkEngine->play2D(m_honk->getSoundSource(), false, false, true);
		}

		if (m_tauntEngine)
		{
			if (m_controller->ButtonYIsPressed(m_controllerID))
			{
				filename = m_soundFiles[randomNumber];
				m_tauntEngine->play2D(filename, false);
				cout << "Pressing Y" << endl;
			}
		}

		if (m_nrOfBombSwitch > 0 && m_timeOut > 5)
		{
			if (m_controller->ButtonBIsPressed(m_controllerID))
			{
				// Play "Here comes a bomb"" 
				m_nrOfBombSwitch--;
				m_timeOut = 0;
				cout << "Pressing B" << endl;
			}
		}

		if (m_nrOfLightSwitch > 0 && m_timeOut > 5)
		{
			if (m_controller->ButtonAIsPressed(m_controllerID))
			{
				// Play "Buuuuh, daaark"
				m_nrOfLightSwitch--;
				m_timeOut = 0;
				cout << "Pressing A" << endl;
			}
		}

		if (m_nrOfCtrlSwitch > 0 && m_timeOut > 5)
		{
			if (m_controller->ButtonXIsPressed(m_controllerID))
			{
				// Play "HEHEHEHE, TRY DIZ"
				m_nrOfCtrlSwitch--;
				m_timeOut = 0;
				cout << "Pressing X" << endl;
			}
		}
	}
}

int Ghost::GetControllerID()
{
	return m_controllerID;
}

bool Ghost::GetBombSwitch()
{
	return m_bombSwitch;
}

bool Ghost::GetCtrlSwitch()
{
	return m_ctrlSwitch;
}

bool Ghost::GetLightSwitch()
{
	return m_lightSwitch;
}

void Ghost::SetControllerID(int index)
{
	m_controllerID = index;
}

void Ghost::SetBombSwitch(bool state)
{
	m_bombSwitch = state;
}

void Ghost::SetCtrlSwitch(bool state)
{
	m_ctrlSwitch = state;
}

void Ghost::SetLightSwitch(bool state)
{
	m_lightSwitch = state;
}


