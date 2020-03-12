#include "Header Files/Ghost.h"

Ghost::Ghost()
{
	m_controller = new Controller();

	m_controllerID = 0;
	m_nrOfBombSwitch = 5;
	m_nrOfLightSwitch = 20;
	m_nrOfCtrlSwitch = 10;

	m_bombSwitch = false;
	m_ctrlSwitch = false;
	m_lightSwitch = false;

	m_honkEngine = createIrrKlangDevice();
	m_tauntEngine = createIrrKlangDevice();

	// Create sound
	if (m_honkEngine && m_tauntEngine)
	{
		m_sounds.push_back(m_honkEngine->addSoundSourceFromFile("src/Audio/Player - HonkDELUXE.mp3"));

		m_tauntEngine->setSoundVolume(0.7f);
		m_taunt = m_tauntEngine->play2D(m_sounds[0], false, true);

		m_honkEngine->setSoundVolume(0.2f);
		m_honk = m_honkEngine->play2D(m_sounds[0], false, true);
	}

	// SpookSounds
	m_soundFiles[0] = "src/Audio/Ghost sound 1.mp3";
	m_soundFiles[1] = "src/Audio/Ghost sound 2.mp3";
	m_soundFiles[2] = "src/Audio/Ghost sound 3.mp3";
	m_soundFiles[3] = "src/Audio/Ghost sound 4.mp3";
	m_soundFiles[4] = "src/Audio/Ghost sound 5.mp3";
	m_soundFiles[5] = "src/Audio/Ghost sound 6.mp3";
	m_soundFiles[6] = "src/Audio/Ghost sound 7.mp3";
	m_soundFiles[7] = "src/Audio/Ghost sound 8.mp3";
	m_soundFiles[8] = "src/Audio/Ghost sound 9.mp3";
	m_soundFiles[9] = "src/Audio/Ghost sound 10.mp3";
	m_soundFiles[10] = "src/Audio/Ghost sound 11.mp3";
	m_soundFiles[11] = "src/Audio/Ghost sound 12.mp3";
	m_soundFiles[12] = "src/Audio/Ghost sound 13.mp3";
	m_soundFiles[13] = "src/Audio/Ghost sound 14.mp3";
	m_soundFiles[14] = "src/Audio/Ghost sound 15.mp3";
	m_soundFiles[15] = "src/Audio/Ghost sound 16.mp3";
	m_soundFiles[16] = "src/Audio/Ghost sound 17.mp3";
	m_soundFiles[17] = "src/Audio/Ghost sound 18.mp3";
	m_soundFiles[18] = "src/Audio/Ghost sound 19.mp3";
	m_soundFiles[19] = "src/Audio/Ghost sound 20.mp3";
	m_soundFiles[20] = "src/Audio/Ghost sound 21.mp3";
	m_soundFiles[21] = "src/Audio/Ghost sound 22.mp3";
	m_soundFiles[22] = "src/Audio/Ghost sound 23.mp3";
	m_soundFiles[23] = "src/Audio/Ghost sound 24.mp3";
	m_soundFiles[24] = "src/Audio/Ghost sound 25.mp3";
	m_nrOfSpookSounds = 25;

	m_soundFiles[25] = "src/Audio/Ghost - bomb.mp3";
	m_soundFiles[26] = "src/Audio/Ghost - lights.mp3";
	m_soundFiles[27] = "src/Audio/Ghost - turns.mp3";
}

Ghost::~Ghost()
{
	delete m_controller;


	if (m_honkEngine)
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
	int randomNumber = rand() % m_nrOfSpookSounds;

	if (glfwJoystickPresent(m_controllerID))
	{
		if (m_honkEngine && m_controller->ButtonRightJoystickIsPressed(m_controllerID))
		{
			m_honkEngine->play2D(m_honk->getSoundSource(), false, false, true);
		}

		if (m_tauntEngine && m_timeOut > 1.0f)
		{
			if (m_controller->ButtonYIsPressed(m_controllerID))
			{
				filename = m_soundFiles[randomNumber];
				m_tauntEngine->play2D(filename, false);
				cout << "Pressing Y and sound nr " << randomNumber << endl;
				m_timeOut = 0;
			}
		}

		if (m_nrOfBombSwitch > 0 && m_timeOut > 1.5f)
		{
			if (m_controller->ButtonBIsPressed(m_controllerID))
			{
				// Play "Here comes a bomb"" 
				filename = m_soundFiles[25];
				m_tauntEngine->play2D(filename, false);
				m_nrOfBombSwitch--;
				m_timeOut = 0;
				m_bombSwitch = true;
				cout << "Pressing B" << endl;
			}
		}

		if (m_nrOfLightSwitch > 0 && m_timeOut > 5.f)
		{
			if (m_controller->ButtonAIsPressed(m_controllerID))
			{
				// Play "Buuuuh, daaark"
				filename = m_soundFiles[26];
				m_tauntEngine->play2D(filename, false);
				m_lightSwitch = true;
				m_nrOfLightSwitch--;
				m_timeOut = 0;
				cout << "Pressing A" << endl;
			}
		}

		if (m_nrOfCtrlSwitch > 0 && m_timeOut > 5.f)
		{
			if (m_controller->ButtonXIsPressed(m_controllerID))
			{
				// Play "HEHEHEHE, TRY DIZ"
				filename = m_soundFiles[27];
				m_tauntEngine->play2D(filename, false);
				m_ctrlSwitch = true;
				m_nrOfCtrlSwitch--;
				m_timeOut = 0;
				cout << "Pressing X" << endl;
			}
		}
	}

	if (m_timeOut >= 3.f)
	{
		m_lightSwitch = false;
		m_ctrlSwitch = false;
		m_bombSwitch = false;
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
