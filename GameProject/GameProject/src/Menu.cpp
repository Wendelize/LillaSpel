#include "Header Files/Menu.h"
#include "Header Files/stb_image.h"

Menu::Menu(Scene* scene, ObjectHandler* objHand)
{
	m_scene = scene;
	m_objHand = objHand;
	vector<int> temp;
	m_kills.push_back(temp);
	m_kills.push_back(temp);
	m_kills.push_back(temp);
	m_kills.push_back(temp);
	m_soundEngine = createIrrKlangDevice();

	if (m_soundEngine)
	{
		m_soundEngine->setListenerPosition(vec3df(0, 18, 33), vec3df(0, -4, 3)); // Listener position, view direction
		m_soundEngine->setDefault3DSoundMinDistance(1.0f);

		m_menuSounds.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/UI - Toggle.mp3"));
		m_menuSounds.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/UI - Select.mp3"));
		m_menuSounds.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/UI - Back.mp3"));

		m_soundEngine->setSoundVolume(0.65f);
	}
	
	m_killers.push_back(temp);
	m_killers.push_back(temp);
	m_killers.push_back(temp);
	m_killers.push_back(temp);

	m_playerColor.push_back(m_p1Col);
	m_playerColor.push_back(m_p2Col);
	m_playerColor.push_back(m_p3Col);
	m_playerColor.push_back(m_p4Col);
}

Menu::~Menu()
{
	if (m_soundEngine)
	{
		for (uint i = 0; i < m_menuSounds.size(); i++)
		{
			m_menuSounds[i]->drop();
		}
		m_menuSounds.clear();
	}
}


void Menu::RenderMenu(bool gameOver, float timer,Model* model)
{
	GLFWgamepadstate state;
	float borderCol = 0.2;
	float p4FadeCol = 0.3;

	Window* w = m_scene->GetOurWindow();
	//if (gameOver)
	//{
	//	m_menu = ActiveMenu::restart;
	//}
	if ((m_menu == ActiveMenu::playerHud || m_menu == ActiveMenu::select) && glfwGetGamepadState(0, &state))
	{
		// TODO: maybe one should be able to access the pause menu from more places?
		if (state.buttons[GLFW_GAMEPAD_BUTTON_START])
		{
			if (m_soundEngine)
				m_soundEngine->play2D(m_menuSounds[1], false);
			m_menu = ActiveMenu::pause;
		}
	}
	double time = glfwGetTime();
	int winnerIndex = 0;

	GLFWwindow* wW = m_scene->GetWindow();

	int width = 0;
	int height = 0;
	int slWidth = 0;
	int mainMenuButonHeight = 0;
	int pauseMenuWidth = 0;
	int pauseMenuHeight = 0;

	glfwGetWindowSize(wW, &width, &height);

	// resize menyerna om 720p
	if (height <= 720)
	{
		slWidth = 200;
		pauseMenuWidth = 75;
		pauseMenuHeight = 100;
		mainMenuButonHeight = 200;

	}

	switch (m_menu)
	{
	case ActiveMenu::start:
		ImGui::SetNextWindowPos(ImVec2(-4, -2));
		

		//float wi = *width;
		//float hi = *height;
		ImGui::SetNextWindowSize(ImVec2((float)width+8, (float)height+2));//(float)w->GetWidht() + 4, w->GetHeight() + 4));
		if (ImGui::Begin("##Background", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav))
		{
			ImGui::SetCursorPos(ImVec2(0, 0));
			ImGui::Image((void*)m_mainMenuPic, ImVec2(width+8, height+2), ImVec2(0, 0), ImVec2(1, 1));
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(-2, -2));
		ImGui::SetNextWindowSize(ImVec2((float)width + 4, height + 4));
		if (ImGui::Begin("##MainMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiNavInput_Activate))
		{
			if (time - m_inputSeconds >= 0.3 && glfwGetGamepadState(0, &state))
			{
				if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP])
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[0], false);
					m_inputSeconds = time;
				}
			}

			float middle = (float)width * 0.5f;
			float heightMiddle = (float)height * 0.5f;
			ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[1]);


			//	NavHighlight is the border around the button
			ImGui::PushStyleColor(ImGuiCol_NavHighlight, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0, 0.0, 0.0, 1));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.1, 0.1, 0.3));



			ImGui::SetCursorPos(ImVec2(middle - (middle / 2.f), heightMiddle));
			if (ImGui::Button("Start", ImVec2(200, 75)))
			{
				if (m_soundEngine)
					m_soundEngine->play2D(m_menuSounds[1], false);
				m_menu = ActiveMenu::select;
				m_objHand->GetCube()->SetCurrentLevel(m_mapID);
				m_objHand->AddPlayer(SELECTPOS1, 0, 0, m_p1Col, model);
				m_p1Seconds = time;
				m_p2Seconds = time;
				m_p3Seconds = time;
				m_p4Seconds = time;
			}

			ImGui::SetCursorPos(ImVec2(middle - (middle / 2.f), heightMiddle + (heightMiddle / 4.f)));
			if (ImGui::Button("Exit", ImVec2(200, 75)))
			{
				if (m_soundEngine)
					m_soundEngine->play2D(m_menuSounds[2], false);
				glfwSetWindowShouldClose(m_scene->GetWindow(), 1);
			}
			//ImGui::PopStyleVar(1); // pop all the styles
			ImGui::PopFont();
			ImGui::PopStyleColor(4);

		}
		ImGui::End();
		break;
	case ActiveMenu::select:

		m_scene->SetBloom(false);
		m_scene->ResetCameraFOV();
		m_scene->SetOnlySky(true);
		m_scene->SetInstantCameraFocus(CAMERAPOS_SELECT + vec3(0, -1, 1));

		ImGui::SetNextWindowPos(ImVec2(0, height / 2));
		ImGui::SetNextWindowSize(ImVec2(width / 4, height / 2)); 
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
		ImGui::GetStyle().WindowRounding = 0.0f;
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_p1Col.x * borderCol, m_p1Col.y * borderCol, m_p1Col.z * borderCol, 1));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p1Col.x / 3, m_p1Col.y / 3, m_p1Col.z / 3, 1));
		if (ImGui::Begin("##player1Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs))
		{
			int index = 0;
			for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
			{
				if (m_objHand->GetPlayerControllerIDBloo(i, 27) == 0)
				{
					index = i;
				}
			}
			m_p1Col = m_objHand->GetPlayerColor(index);
			m_playerColor[0] = m_p1Col;
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
			//ImGui::SetCursorPos(ImVec2(width / 8 - (width/16), 0));

			if (m_selected[0] == 0)
			{
				string temp = "PLAYER 1";
				ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
				ImGui::Text(temp.c_str());

				temp = " < Vehicle Model :" + to_string(m_p1ModelId) + "  >";
				ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
				ImGui::Text(temp.c_str());
			}
			else
			{
				string temp = "PLAYER 1";
				ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
				ImGui::Text(temp.c_str());

				temp = "Press \"B\" to Unselect!";
				ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
				ImGui::Text(temp.c_str());
			}

			if (time - m_p1Seconds >= 0.3 && (glfwGetGamepadState(0, &state)))
			{
				if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] && m_selected[0] == 0)
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[0], false);
					if ((m_p1ModelId + 1) <= m_scene->GetNumPlayerModels() - 1)
					{
						m_p1ModelId += 1;
					}
					else
					{
						m_p1ModelId = 0;
					}

					m_objHand->SetPlayerModelID(index, m_p1ModelId);

					m_p1Seconds = time;
				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] && m_selected[0] == 0)
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[0], false);
					if ((m_p1ModelId - 1) >= 0)
					{
						m_p1ModelId -= 1;
					}
					else
					{
						m_p1ModelId = m_scene->GetNumPlayerModels() - 1;
					}

					m_objHand->SetPlayerModelID(index, m_p1ModelId);

					m_p1Seconds = time;
				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[1], false);
					m_selected[0] = 1;
					m_p1Seconds = time;
				}
				else if (m_selected[0] == 1 && state.buttons[GLFW_GAMEPAD_BUTTON_B])
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[2], false);
					m_selected[0] = 0;
				}
			}

			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(width / 4, height / 2));
		ImGui::SetNextWindowSize(ImVec2(width / 4, height / 2));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_p2Col.x * borderCol, m_p2Col.y * borderCol, m_p2Col.z * borderCol, 1));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p2Col.x / 3, m_p2Col.y / 3, m_p2Col.z / 3, 1));
		if (ImGui::Begin("##player2Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs))
		{
			if (m_p2Joined == false)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				if (time - m_p3Seconds >= 0.3 && (glfwGetGamepadState(1, &state)))
				{
					string temp = " Join? Press \"A\"!";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());

					if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[1], false);
						m_p2Joined = true;
						m_objHand->AddPlayer(SELECTPOS2, 1, 0, m_p2Col, model);
						m_p2Seconds = time;
						m_p2ModelId = 0;
					}
				}
			}
			else
			{
				//GLFWgamepadstate state;
				int index = 1;
				for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
				{
					if (m_objHand->GetPlayerControllerIDBloo(i, 28) == 1)
					{
						index = i;
					}
				}
				m_p2Col = m_objHand->GetPlayerColor(index);
				m_playerColor[1] = m_p2Col;
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				if (m_selected[1] == 0)
				{
					string temp = "PLAYER 2";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());

					temp = "Vehicle Model : " + to_string(m_p2ModelId) + "  >";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());
				}
				else
				{
					string temp = "PLAYER 2";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());

					temp = "Press \"B\" to Unselect!";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());
				}

				if (time - m_p2Seconds >= 0.3 && (glfwGetGamepadState(1, &state)))
				{
					if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] && m_selected[1] == 0)
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[0], false);
						if ((m_p2ModelId + 1) <= m_scene->GetNumPlayerModels() - 1)
						{
							m_p2ModelId += 1;
						}
						else
						{
							m_p2ModelId = 0;
						}
						m_objHand->SetPlayerModelID(index, m_p2ModelId);
						m_p2Seconds = time;
					}
					else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] && m_selected[1] == 0)
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[0], false);
						if ((m_p2ModelId - 1) >= 0)
						{
							m_p2ModelId -= 1;
						}
						else
						{
							m_p2ModelId = m_scene->GetNumPlayerModels() - 1;
						}

						m_objHand->SetPlayerModelID(index, m_p2ModelId);
						m_p2Seconds = time;
					}
					else if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[1], false);
						m_selected[1] = 1;
						m_p2Seconds = time;
					}
					else if (m_selected[1] == 1 && state.buttons[GLFW_GAMEPAD_BUTTON_B])
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[2], false);
						m_selected[1] = 0;
						m_p2Seconds = time;
					}
				}
			}
			
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
			ImGui::End();
		}

		// TODO: Kanske Fixa s� det f�ljer kontrollerId eller n�tt
		//if(glfwGetGamepadState(2, &state))//m_objHand->GetNumPlayers() >= 3)
		//{
		ImGui::SetNextWindowPos(ImVec2(width / 2, height / 2));
		ImGui::SetNextWindowSize(ImVec2(width / 4, height / 2));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_p3Col.x * borderCol, m_p3Col.y * borderCol, m_p3Col.z * borderCol, 1));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p3Col.x / 3, m_p3Col.y / 3, m_p3Col.z / 3, 1));
		if (ImGui::Begin("##player3Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs))
		{
			//GLFWgamepadstate state;
			if (m_p3Joined == false)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				if (time - m_p3Seconds >= 0.3 && (glfwGetGamepadState(2, &state)))
				{
					string temp = " Join? Press \"A\"!";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());

					if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[1], false);
						m_p3Joined = true;
						m_objHand->AddPlayer(SELECTPOS3, 2, 0, m_p3Col, model);
						m_p3Seconds = time;
						m_p3ModelId = 0;
					}

				}
			}
			else
			{
				int index = 2;
				for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
				{
					if (m_objHand->GetPlayerControllerIDBloo(i, 29) == 2)
					{
						index = i;
					}
				}
				m_p3Col = m_objHand->GetPlayerColor(index);
				m_playerColor[2] = m_p3Col;
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				if (m_selected[2] == 0)
				{
					string temp = "PLAYER 3";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());

					temp = "Vehicle Model : " + to_string(m_p3ModelId) + "  >";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());
				}
				else
				{
					string temp = "PLAYER 3";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());

					temp = "Press \"B\" to Unselect!";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());
				}

				if (time - m_p3Seconds >= 0.3 && (glfwGetGamepadState(2, &state)))
				{
					if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] && m_selected[2] == 0)
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[0], false);
						if ((m_p3ModelId + 1) <= m_scene->GetNumPlayerModels() - 1)
						{
							m_p3ModelId += 1;
						}
						else
						{
							m_p3ModelId = 0;
						}

						m_objHand->SetPlayerModelID(index, m_p3ModelId);
						m_p3Seconds = time;
					}
					else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] && m_selected[2] == 0)
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[0], false);
						if ((m_p3ModelId - 1) >= 0)
						{
							m_p3ModelId -= 1;
						}
						else
						{
							m_p3ModelId = m_scene->GetNumPlayerModels() - 1;
						}

						m_objHand->SetPlayerModelID(index, m_p3ModelId);
						m_p3Seconds = time;
					}
					else if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[1], false);
						m_selected[2] = 1;
						m_p3Seconds = time;
					}
					else if (m_selected[2] == 1 && state.buttons[GLFW_GAMEPAD_BUTTON_B])
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[2], false);
						m_selected[2] = 0;
						m_p3Seconds = time;
					}

				}
			}

			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
		}
		ImGui::End();
		//}

		//if (m_p3Joined && glfwGetGamepadState(3, &state))//m_objHand->GetNumPlayers() >= 4)
		//{
		ImGui::SetNextWindowPos(ImVec2(width / 2 + width / 4, height / 2));
		ImGui::SetNextWindowSize(ImVec2(width / 4, height / 2));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
		p4FadeCol = 0.25;
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_p4Col.x * borderCol, m_p4Col.y * borderCol, m_p4Col.z * borderCol, 1));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p4Col.x / 3 - p4FadeCol, m_p4Col.y / 3 - p4FadeCol, m_p4Col.z / 3 - p4FadeCol, 1));
		if (ImGui::Begin("##player4Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs))
		{
				
			if (m_p4Joined == false)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				if (time - m_p4Seconds >= 0.3 && (glfwGetGamepadState(3, &state)))
				{
					string temp = " Join? Press \"A\"!";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());

					if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[1], false);
						m_p4Joined = true;
						m_objHand->AddPlayer(SELECTPOS4, 3, 0, m_p4Col, model);
						m_p4Seconds = time;
						m_p4ModelId = 0;
					}

				}
				//ImGui::PopStyleColor();
			}
			else
			{
				int index = 3;
				for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
				{
					if (m_objHand->GetPlayerControllerIDBloo(i, 30) == 3)
					{
						index = i;
					}
				}
				m_p4Col = m_objHand->GetPlayerColor(index);
				m_playerColor[3] = m_p4Col;
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				if (m_selected[3] == 0)
				{
					string temp = "PLAYER 4";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());

					temp = "Vehicle Model : " + to_string(m_p4ModelId) + "  >";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());
				}
				else
				{
					string temp = "PLAYER 4";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());

					temp = "Press \"B\" to Unselect!";
					ImGui::SetCursorPosX(width / 8 - ImGui::CalcTextSize(temp.c_str()).x / 2);
					ImGui::Text(temp.c_str());
				}

				if (time - m_p4Seconds >= 0.3 && (glfwGetGamepadState(3, &state)))
				{
					if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] && m_selected[3] == 0)
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[0], false);
						if ((m_p4ModelId + 1) <= m_scene->GetNumPlayerModels() - 1)
						{
							m_p4ModelId += 1;
						}
						else
						{
							m_p4ModelId = 0;
						}

						m_objHand->SetPlayerModelID(index, m_p4ModelId);
						m_p4Seconds = time;
					}
					else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] && m_selected[3] == 0)
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[0], false);
						if ((m_p4ModelId - 1) >= 0)
						{
							m_p4ModelId -= 1;
						}
						else
						{
							m_p4ModelId = m_scene->GetNumPlayerModels() - 1;
						}

						m_objHand->SetPlayerModelID(index, m_p4ModelId);
						m_p4Seconds = time;
					}
					else if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[1], false);
						m_selected[3] = 1;
						m_p4Seconds = time;
					}
					else if (m_selected[3] == 1 && state.buttons[GLFW_GAMEPAD_BUTTON_B])
					{
						if (m_soundEngine)
							m_soundEngine->play2D(m_menuSounds[2], false);
						m_selected[3] = 0;
						m_p4Seconds = time;
					}

				}
			}

			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
		}
		ImGui::End();
		//}

		for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
		{

			if (m_selected[i] == 1)
			{
				m_continue += 1;
			}
		}


		if (m_continue == m_objHand->GetNumPlayers())
		{
			m_menu = ActiveMenu::selectLevel;

			for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
			{
				m_selected[i] = 0;
			}
		}

		m_continue = 0;
		break;
	case ActiveMenu::selectLevel:

		if(m_objHand->GetNumPlayers() == 1)
			m_objHand->AddPlayer(SELECTPOS2, 1, 0, m_p2Col, model);

		m_scene->SetBloom(true);
		m_scene->SetOnlySky(false);
		m_scene->SetInstantCameraFocus(vec3(0, 0, 0));
		m_scene->SetCameraPos(CAMERAPOS_GAME);

		//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 10);
		ImGui::SetNextWindowPos(ImVec2((float)width / 3 - (slWidth / 2), 0));
		ImGui::SetNextWindowSize(ImVec2((float)width / 3 + slWidth, height / 4));

		if (ImGui::Begin("##selectLevel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiNavInput_Activate))
		{
			if (m_selected[0] == 0)
			{
				float windowSizeX = (float)width / 3 + slWidth;
				m_mapID = m_objHand->GetCube()->GetCurrentLevel();
				ImGui::PushFont(w->m_fonts[1]);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8, 0.8, 0.8, 1));
				string temp = "Select Level";
				ImGui::SetCursorPosX(windowSizeX/2 - ImGui::CalcTextSize(temp.c_str()).x / 2);
				ImGui::Text(temp.c_str());
				ImGui::PopFont();
				ImGui::PushFont(w->m_fonts[8]);
				temp = to_string(m_mapID);
				ImGui::SetCursorPosX(windowSizeX / 2 - ImGui::CalcTextSize(temp.c_str()).x / 2);
				ImGui::Text(temp.c_str());
				ImGui::PopStyleColor();
				ImGui::PopFont();
			}
			else if (m_selected[0] == 1)
			{
				m_menu = ActiveMenu::selectLives;
				m_selected[0] = 0;
				m_objHand->SetNumberOfLives(m_maxLives);
			}
			else // b�r bara kunna vara -1
			{
				m_menu = ActiveMenu::select;
				m_selected[0] = 0;
			}

			if (time - m_p1Seconds >= 0.3 && (glfwGetGamepadState(0, &state)))
			{
				if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[0], false);
					if (m_selected[0] == 0)
					{
						if (m_mapID < 6)
						{
							m_mapID += 1;
							m_objHand->GetCube()->SetCurrentLevel(m_mapID);
							m_updateMap = true;

						}
						else
						{
							m_mapID = 0;
							m_objHand->GetCube()->SetCurrentLevel(m_mapID);
							m_updateMap = true;

						}

					}
					m_p1Seconds = time;

				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[0], false);
					if (m_selected[0] == 0)
					{
						if (m_mapID <= 6 && m_mapID > 0)
						{
							m_mapID -= 1;
							m_objHand->GetCube()->SetCurrentLevel(m_mapID);
							m_updateMap = true;

						}
						else
						{
							m_mapID = 6;
							m_objHand->GetCube()->SetCurrentLevel(m_mapID);
							m_updateMap = true;

						}
					}
					m_p1Seconds = time;
				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_A] && m_selected[0] <= 2)
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[1], false);
					m_selected[0] += 1;
					m_objHand->GetCube()->SetCurrentLevel(m_mapID);
					m_updateMap = true;
					m_p1Seconds = time;

				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_B] && m_selected[0] > -1)
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[2], false);
					m_selected[0] -= 1;
					m_p1Seconds = time;

				}
			}
			//ImGui::PopStyleColor();
			//ImGui::PopFont();
			//ImGui::PopStyleVar();
		}
		ImGui::End();
		break;

	case ActiveMenu::selectLives:
		m_scene->SetBloom(true);
		m_scene->SetOnlySky(false);
		m_scene->SetInstantCameraFocus(vec3(0, 0, 0));
		m_scene->SetCameraPos(CAMERAPOS_GAME);

		for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
		{
			if (i == 0)
				m_objHand->SetWholePlayerPos(vec3(-10, 6, 3), i);
			else if (i == 1)
				m_objHand->SetWholePlayerPos(vec3(10, 6, 3), i);
			else if (i == 2)
				m_objHand->SetWholePlayerPos(vec3(-7, 6, 15), i);
			else if (i == 3)
				m_objHand->SetWholePlayerPos(vec3(7, 6, 15), i);
		}

		//GLFWgamepadstate state;
	////	int fillerSize = 300;
	//	if (height <= 720)
	//	{
	//		fillerSize = 200;
	//	}
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 10);
		ImGui::SetNextWindowPos(ImVec2((float)width / 3 - (slWidth / 2), 0));
		ImGui::SetNextWindowSize(ImVec2((float)width / 3 + slWidth, height / 4));
		if (ImGui::Begin("##selectLives", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiNavInput_Activate))
		{
			if (m_selected[0] == 0)
			{
				//ImGui::Text("\t\t\t\t\t\t\t\t\t\t\t\t\t\t <  Lives : %d  > ", m_maxLives);
				//ImGui::Text("\t\t\t\t\t\t\t Press \"B\" to Select Level Again ");

				float windowSizeX = (float)width / 3 + slWidth;
				ImGui::PushFont(w->m_fonts[1]);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8, 0.8, 0.8, 1));
				string temp = "Select Lives";
				ImGui::SetCursorPosX(windowSizeX / 2 - ImGui::CalcTextSize(temp.c_str()).x / 2);
				ImGui::Text(temp.c_str());
				ImGui::PopFont();
				ImGui::PushFont(w->m_fonts[8]);
				temp = to_string(m_maxLives);
				ImGui::SetCursorPosX(windowSizeX / 2 - ImGui::CalcTextSize(temp.c_str()).x / 2);
				ImGui::Text(temp.c_str());
				ImGui::PopStyleColor();
				ImGui::PopFont();

			}
			else if (m_selected[0] == 1)
			{
				int minutes = m_maxTime / 60;
				int seconds = m_maxTime % 60;
				//ImGui::Text("\t\t\t\t\t\t\t\t\t\t\t\t\t <  Time : %dm : %ds > ", minutes, seconds);
				//ImGui::Text("\t\t\t\t\t\t\t Press \"B\" to Select Lives Again ");

				float windowSizeX = (float)width / 3 + slWidth;
				ImGui::PushFont(w->m_fonts[1]);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8, 0.8, 0.8, 1));
				string temp = "Set Time Limit";
				ImGui::SetCursorPosX(windowSizeX / 2 - ImGui::CalcTextSize(temp.c_str()).x / 2);
				ImGui::Text(temp.c_str());
				ImGui::PopFont();
				ImGui::PushFont(w->m_fonts[8]);
				temp = to_string(minutes) + "m : " + to_string(seconds) + "s";
				ImGui::SetCursorPosX(windowSizeX / 2 - ImGui::CalcTextSize(temp.c_str()).x / 2);
				ImGui::Text(temp.c_str());
				ImGui::PopStyleColor();
				ImGui::PopFont();
			}
			else if (m_selected[0] == 2)
			{
				m_menu = ActiveMenu::playerHud;
				m_selected[0] = 0;
				m_objHand->SetNumberOfLives(m_maxLives);
			}
			else // b�r bara kunna vara -1
			{
				m_menu = ActiveMenu::selectLevel;
				m_selected[0] = 0;
			}

			if (time - m_p1Seconds >= 0.3 && (glfwGetGamepadState(0, &state)))
			{
				if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[0], false);
					if (m_selected[0] == 0)
					{
						if (m_maxLives < 10)
						{
							m_maxLives += 1;
						}
						else
						{
							m_maxLives = 1;
						}

					}
					else if (m_selected[0] == 1)
					{
						if (m_maxTime < 60)
						{
							m_maxTime += 15;
						}
						else if (m_maxTime < 600)
						{
							m_maxTime += 30;
						}
						else
						{
							m_maxTime = 15;
						}
					}
					m_p1Seconds = time;
				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[0], false);
					if (m_selected[0] == 0)
					{
						if (m_maxLives <= 10 && m_maxLives > 1)
						{
							m_maxLives -= 1;
						}
						else
						{
							m_maxLives = 10;
						}

					}
					else if (m_selected[0] == 1)
					{
						if (m_maxTime <= 60 && m_maxTime >= 30)
						{
							m_maxTime -= 15;
						}
						else if (m_maxTime <= 600 && m_maxTime >= 60)
						{
							m_maxTime -= 30;
						}
						else if (m_maxTime == 15)
						{
							m_maxTime = 600;
						}
					}
					m_p1Seconds = time;
				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_A] && m_selected[0] <=2)
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[1], false);
					m_selected[0] += 1;
					m_p1Seconds = time;
				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_B] && m_selected[0] > -1)
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[2], false);
					m_selected[0] -= 1;
					m_p1Seconds = time;
				}

			}
			//ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			//ImGui::PopFont();

		}
		ImGui::End();
		break;
	case ActiveMenu::pause:
		m_scene->SetBloom(true);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 15);
		ImGui::SetNextWindowPos(ImVec2((float)width / 3 - (pauseMenuWidth / 2), height / 4 - (pauseMenuHeight / 2)));
		ImGui::SetNextWindowSize(ImVec2((float)width / 3 + pauseMenuWidth, height / 4 + 150 + pauseMenuHeight));
		if (ImGui::Begin("##PauseMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiNavInput_Activate))
		{
			if (time - m_inputSeconds >= 0.3 && glfwGetGamepadState(0, &state))
			{
				if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] || state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP])
				{
					if (m_soundEngine)
						m_soundEngine->play2D(m_menuSounds[0], false);
					m_inputSeconds = time;
				}
			}

			float middle = (float)width * 0.5f;
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3 - 25, 15));
			ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[1]);
			ImGui::Text("Pause Menu");
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3, 115));
			if (ImGui::Button("Resume", ImVec2(200, 75)))
			{
				if (m_soundEngine)
					m_soundEngine->play2D(m_menuSounds[1], false);
				m_menu = ActiveMenu::playerHud;
			}
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3 - 100, 215));
			if (ImGui::Button("Main Menu", ImVec2(400, 75)))
			{
				// resettar banan m.m. och laddar start menyn
				if (m_soundEngine)
					m_soundEngine->play2D(m_menuSounds[1], false);
				m_menu = ActiveMenu::start;
				m_reset = true;
				m_p2Joined = false;
				m_p3Joined = false;
				m_p4Joined = false;
				m_p1ModelId = 0;
				m_p2ModelId = 0;
				m_p3ModelId = 0;
				m_p4ModelId = 0;
				for (int i = 0; i < 4; i++)
				{
					if (m_objHand->GetNumPlayers() > 0)
					{
						m_objHand->RemovePlayer(m_objHand->GetNumPlayers() - 1);
					}
				}
				/*if (m_objHand->GetNumPlayers() == 0)
				{
					m_objHand->AddPlayer(vec3(-10, 6, 3), 0, m_p1ModelId, m_p1Col, model);
				}*/
			}
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3, 315));
			if (ImGui::Button("Exit", ImVec2(200, 75)))
			{
				if (m_soundEngine)
					m_soundEngine->play2D(m_menuSounds[2], false);
				glfwSetWindowShouldClose(m_scene->GetWindow(), 1);
			}
			ImGui::PopStyleVar(1); // pop all the styles
			ImGui::PopFont();
			//ImGui::PopStyleColor();

		}
		ImGui::End();
		break;
	case ActiveMenu::restart:
		m_scene->SetBloom(true);
		m_scene->SetOnlySky(false);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 15);
		ImGui::SetNextWindowPos(ImVec2((float)width / 3 - (pauseMenuWidth / 2), height / 4 - (pauseMenuHeight / 2)));
		ImGui::SetNextWindowSize(ImVec2((float)width / 3 + pauseMenuWidth, height / 4 + 150 + pauseMenuHeight));
		ImGui::SetNextWindowBgAlpha(0.9);
		if (ImGui::Begin("##RestartMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiNavInput_Activate))
		{

			float middle = (float)width * 0.5f;
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3 - 10, 15));
			ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[1]);
			ImGui::Text("Start Menu");
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3 - 25, 115));
			if (ImGui::Button("Restart", ImVec2(250, 75)))
			{
				if (m_soundEngine)
					m_soundEngine->play2D(m_menuSounds[1], false);
				m_menu = ActiveMenu::playerHud;

				m_reset = true;
				// addar tillbaka spelarna h�r i Menu.cpp f�r att bevara deras modeller om man 
				//	bara vill resetta med samma inst�llnignar
				for (int i = 0; i < 4; i++)
				{
					if (m_objHand->GetNumPlayers() > 0)
					{
						m_objHand->RemovePlayer(m_objHand->GetNumPlayers() - 1);
					}
				}
				if (m_objHand->GetNumPlayers() == 0)
				{
					m_objHand->AddPlayer(vec3(-10, 6, 3), 0, m_p1ModelId, m_p1Col, model);
				}
				if (m_p2Joined && m_objHand->GetNumPlayers() == 1)
				{
					m_objHand->AddPlayer(vec3(10, 6, 3), 1, m_p2ModelId, m_p2Col, model);
					if (m_p3Joined && m_objHand->GetNumPlayers() == 2)
					{
						m_objHand->AddPlayer(vec3(-7, 6, 15), 2, m_p3ModelId, m_p3Col, model);
						if (m_p4Joined)
						{
							m_objHand->AddPlayer(vec3(7, 6, 15), 3, m_p4ModelId, m_p4Col, model);
						}
					}
				}
			}
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3 - 100, 215));
			if (ImGui::Button("Main Menu", ImVec2(400, 75)))
			{
				// resettar banan m.m. och laddar start menyn
				if (m_soundEngine)
					m_soundEngine->play2D(m_menuSounds[1], false);
				m_menu = ActiveMenu::start;
				m_reset = true;
				m_p2Joined = false;
				m_p3Joined = false;
				m_p4Joined = false;
				m_p1ModelId = 0;
				m_p2ModelId = 0;
				m_p3ModelId = 0;
				m_p4ModelId = 0;
				for (int i = 0; i < 4; i++)
				{
					if (m_objHand->GetNumPlayers() > 0)
					{
						m_objHand->RemovePlayer(m_objHand->GetNumPlayers() - 1);
					}
				}
				/*if (m_objHand->GetNumPlayers() == 0)
				{
					m_objHand->AddPlayer(vec3(-10, 6, 3), 0, m_p1ModelId, m_p1Col, model);
					m_objHand->AddPlayer(vec3(10, 6, 3), 1, m_p2ModelId, m_p2Col, model);
				}*/
			}
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3, 315));
			if (ImGui::Button("Exit", ImVec2(200, 75)))
			{
				if (m_soundEngine)
					m_soundEngine->play2D(m_menuSounds[2], false);
				glfwSetWindowShouldClose(m_scene->GetWindow(), 1);
			}
			ImGui::PopStyleVar(1); // pop all the styles
			ImGui::PopFont();
			//ImGui::PopStyleColor();

		}
		ImGui::End();
		break;
	case ActiveMenu::win:
		m_scene->SetBloom(true);
		m_scene->SetOnlySky(false);
		ImGui::SetNextWindowPos(ImVec2(width / 2 - 150, 0));
		ImGui::SetNextWindowSize(ImVec2(300, 60));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
		if (m_winOrder[0] == 2)
		{
			borderCol = 2.5;
		}
		if (m_winOrder[0] == 2 || m_winOrder[0] == 3)
		{
			borderCol = 2.5;
		}
		else if (m_winOrder[0] == 1)
		{
			borderCol = 1.5;
		}
		else
		{
			borderCol = 0.5;
		}
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_playerColor[m_winOrder[0]].x - borderCol, m_playerColor[m_winOrder[0]].y - borderCol, m_playerColor[m_winOrder[0]].z - borderCol, 1));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_playerColor[m_winOrder[0]].x, m_playerColor[m_winOrder[0]].y, m_playerColor[m_winOrder[0]].z, 0.5));
		if (ImGui::Begin("##winner", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			
			//ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0, 0.0, 0.0, 1));
			ImGui::Text("\t\tPlayer  %d Has Won!", m_winOrder[0] + 1);
			/*
			
			if (time - m_p1Seconds >= 0.5 && (glfwGetGamepadState(0, &state)))
			{
				ImGui::Text("\tPress \"A\" To Continue!");

				if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
				{
					m_menu = ActiveMenu::stats;
					m_p1Seconds = time;
				}
				
			}*/
			ImGui::PopStyleColor(2);
			ImGui::PopStyleVar();
		}
		ImGui::End();

		if (time - m_p1Seconds >= 1.0 && (glfwGetGamepadState(0, &state)))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
			ImGui::SetNextWindowPos(ImVec2(width / 2 - 150, height - 60));
			ImGui::SetNextWindowSize(ImVec2(300, 60));
			if (ImGui::Begin("##winnerCont", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text(" Press  \"A \"  To Continue!");

				if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
				{
					m_menu = ActiveMenu::stats;
					m_p1Seconds = time;
				}
				//ImGui::PopStyleColor();
				ImGui::PopStyleVar();
			}
			ImGui::End();
		}
		break;
	case ActiveMenu::stats:
		m_scene->SetBloom(true);
		m_scene->SetOnlySky(false);
		// Winner
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(250, 80));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
		// borderCol = 0.5;
		if (m_winOrder[0] == 2 || m_winOrder[0] == 3)
		{
			borderCol = 2.5;
		}
		else if (m_winOrder[0] == 1)
		{
			borderCol = 1.5;
		}
		else
		{
			borderCol = 0.5;
		}

		if (m_winOrder[0] == 3)
		{
			p4FadeCol = 0.25;
		}
		else
		{
			p4FadeCol = 0;
		}
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_playerColor[m_winOrder[0]].x - borderCol, m_playerColor[m_winOrder[0]].y - borderCol, m_playerColor[m_winOrder[0]].z - borderCol, 1));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_playerColor[m_winOrder[0]].x / 3 - p4FadeCol, m_playerColor[m_winOrder[0]].y / 3 - p4FadeCol, m_playerColor[m_winOrder[0]].z / 3, 1));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
		if (ImGui::Begin("##statsP1.1", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("\t\t\t\t\t 1st \n\t\t\tPlayer\t%d ", m_winOrder[0] + 1);
			
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_playerColor[m_winOrder[0]].x - borderCol, m_playerColor[m_winOrder[0]].y - borderCol, m_playerColor[m_winOrder[0]].z - borderCol, 1));

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
		ImGui::SetNextWindowPos(ImVec2(0, height / 6 * 2));
		ImGui::SetNextWindowSize(ImVec2(250, 500));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_playerColor[m_winOrder[0]].x / 3 - p4FadeCol, m_playerColor[m_winOrder[0]].y / 3 - p4FadeCol, m_playerColor[m_winOrder[0]].z / 3, 1));
		if (ImGui::Begin("##statsP1.2", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Kills : ");
			int killCursorPosY = ImGui::GetCursorPosY();
			int suicides = 0;
			int j = 0;
			for (int i = 0; i < m_kills[m_winOrder[0]].size(); i++)
			{
				if (i % 4 != 0)
				{
					ImGui::SameLine();
				}
				ImGui::Text("\tP%d ", (m_kills[m_winOrder[0]]).at(i) + 1);

			}
			if (m_kills[m_winOrder[0]].size() == 0)
			{
				ImGui::SameLine();
				string temp = "0";
				ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
				ImGui::Text(temp.c_str());

			}
			if (ImGui::GetCursorPosY() < (killCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4))
			{
				ImGui::SetCursorPosY(killCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4);
			}
			ImGui::Separator();
			ImGui::Text("Deaths : ");//	TODO: FIxa så att antal suicides skrivs ut för sig själv 
			int deathCursorPosY = ImGui::GetCursorPosY();
			for (int i = 0; i < m_killers[m_winOrder[0]].size(); i++)
			{


				if ((m_killers[m_winOrder[0]].at(i)) != m_winOrder[0])
				{
					if (j % 4 != 0)
					{
						ImGui::SameLine();
					}
					ImGui::Text("\tP%d ", (m_killers[m_winOrder[0]]).at(i) + 1);
					j++;
				}
				else
				{
					suicides++;
				}


			}

			if (j == 0)
			{
				ImGui::SameLine();
				string temp = "0";
				ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
				ImGui::Text(temp.c_str());

			}

			if (ImGui::GetCursorPosY() < (deathCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4))
			{
				ImGui::SetCursorPosY(deathCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4);
			}
			ImGui::Text("Suicides:");
			int suicidesCursorPosY = ImGui::GetCursorPosY();
			ImGui::SameLine();
			string temp = to_string(suicides);
			ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
			ImGui::Text(temp.c_str());
			ImGui::SameLine();
			ImGui::Separator();
			if (ImGui::GetCursorPosY() < (suicidesCursorPosY + ImGui::GetTextLineHeightWithSpacing()))
			{
				ImGui::SetCursorPosY(suicidesCursorPosY + ImGui::GetTextLineHeightWithSpacing());
			}
			ImGui::Separator();
			ImGui::Text("Collisions:");
			ImGui::SameLine();
			temp = to_string(m_timesCollided[m_winOrder[0]]);
			ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
			ImGui::Text(temp.c_str());


			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
		}
		ImGui::End();



		// Second Place
		ImGui::SetNextWindowPos(ImVec2(width / 9 * 2, 0));
		ImGui::SetNextWindowSize(ImVec2(250, 80));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
		if (m_winOrder[1] == 2 || m_winOrder[1] == 3)
		{
			borderCol = 2.5;
		}
		else if (m_winOrder[1] == 1)
		{
			borderCol = 1.5;
		}
		else
		{
			borderCol = 0.5;
		}

		if (m_winOrder[1] == 3)
		{
			p4FadeCol = 0.25;
		}
		else
		{
			p4FadeCol = 0;
		}
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_playerColor[m_winOrder[1]].x - borderCol, m_playerColor[m_winOrder[1]].y - borderCol, m_playerColor[m_winOrder[1]].z - borderCol, 1));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_playerColor[m_winOrder[1]].x / 3 - p4FadeCol, m_playerColor[m_winOrder[1]].y / 3 - p4FadeCol, m_playerColor[m_winOrder[1]].z / 3, 1));
		if (ImGui::Begin("##statsP2.1", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("\t\t\t\t\t 2nd \n\t\t\tPlayer\t%d ", m_winOrder[1] + 1);
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_playerColor[m_winOrder[1]].x - borderCol, m_playerColor[m_winOrder[1]].y - borderCol, m_playerColor[m_winOrder[1]].z - borderCol, 1));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
		ImGui::SetNextWindowPos(ImVec2(width / 9 * 2, height / 6 * 2));
		ImGui::SetNextWindowSize(ImVec2(250, 500));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_playerColor[m_winOrder[1]].x / 3 - p4FadeCol, m_playerColor[m_winOrder[1]].y / 3 - p4FadeCol, m_playerColor[m_winOrder[1]].z / 3, 1));
		if (ImGui::Begin("##statsP2.2", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Kills : ");
			int killCursorPosY = ImGui::GetCursorPosY();
			int suicides = 0;
			int j = 0;
			for (int i = 0; i < m_kills[m_winOrder[1]].size(); i++)
			{
				if (i % 4 != 0)
				{
					ImGui::SameLine();
				}
				ImGui::Text("\tP%d ", (m_kills[m_winOrder[1]]).at(i) + 1);

			}
			if (m_kills[m_winOrder[1]].size() == 0)
			{
				ImGui::SameLine();
				string temp = "0";
				ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
				ImGui::Text(temp.c_str());

			}
			if (ImGui::GetCursorPosY() < (killCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4))
			{
				ImGui::SetCursorPosY(killCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4);
			}
			ImGui::Separator();
			ImGui::Text("Deaths : ");//	TODO: FIxa så att antal suicides skrivs ut för sig själv 
			int deathCursorPosY = ImGui::GetCursorPosY();
			for (int i = 0; i < m_killers[m_winOrder[1]].size(); i++)
			{


				if ((m_killers[m_winOrder[1]].at(i)) != m_winOrder[1])
				{
					if (j % 4 != 0)
					{
						ImGui::SameLine();
					}
					ImGui::Text("\tP%d ", (m_killers[m_winOrder[1]]).at(i) + 1);
					j++;
				}
				else
				{
					suicides++;
				}


			}

			if (j == 0)
			{
				ImGui::SameLine();
				string temp = "0";
				ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
				ImGui::Text(temp.c_str());

			}

			if (ImGui::GetCursorPosY() < (deathCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4))
			{
				ImGui::SetCursorPosY(deathCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4);
			}
			ImGui::Text("Suicides:");
			int suicidesCursorPosY = ImGui::GetCursorPosY();
			ImGui::SameLine();
			string temp = to_string(suicides);
			ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
			ImGui::Text(temp.c_str());
			ImGui::SameLine();
			ImGui::Separator();
			if (ImGui::GetCursorPosY() < (suicidesCursorPosY + ImGui::GetTextLineHeightWithSpacing()))
			{
				ImGui::SetCursorPosY(suicidesCursorPosY + ImGui::GetTextLineHeightWithSpacing());
			}
			ImGui::Separator();
			ImGui::Text("Collisions:");
			ImGui::SameLine();
			temp = to_string(m_timesCollided[m_winOrder[1]]);
			ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
			ImGui::Text(temp.c_str());
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
		}
		ImGui::End();

		// Third Place
		if (m_winOrder.size() >= 3)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
			if (m_winOrder[2] == 2 || m_winOrder[2] == 3)
			{
				borderCol = 2.5;
			}
			else if (m_winOrder[2] == 1)
			{
				borderCol = 1.5;
			}
			else
			{
				borderCol = 0.5;
			}

			if (m_winOrder[2] == 3)
			{
				p4FadeCol = 0.25;
			}
			else
			{
				p4FadeCol = 0;
			}
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_playerColor[m_winOrder[2]].x - borderCol, m_playerColor[m_winOrder[2]].y - borderCol, m_playerColor[m_winOrder[2]].z - borderCol, 1));
			ImGui::SetNextWindowPos(ImVec2(width / 9 * 6, 0));
			ImGui::SetNextWindowSize(ImVec2(250, 80));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_playerColor[m_winOrder[2]].x / 3 - p4FadeCol, m_playerColor[m_winOrder[2]].y / 3 - p4FadeCol, m_playerColor[m_winOrder[2]].z / 3, 1));
			if (ImGui::Begin("##statsP3.1", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("\t\t\t\t\t 3rd \n\t\t\tPlayer\t%d ", m_winOrder[2] + 1);

				ImGui::PopStyleColor(3);
				ImGui::PopStyleVar();
			}
			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_playerColor[m_winOrder[2]].x - borderCol, m_playerColor[m_winOrder[2]].y - borderCol, m_playerColor[m_winOrder[2]].z - borderCol, 1));
			ImGui::SetNextWindowPos(ImVec2(width / 9 * 6, height / 6 * 2));
			ImGui::SetNextWindowSize(ImVec2(250, 500));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_playerColor[m_winOrder[2]].x / 3 - p4FadeCol, m_playerColor[m_winOrder[2]].y / 3 - p4FadeCol, m_playerColor[m_winOrder[2]].z / 3, 1));
			if (ImGui::Begin("##statsP3.2", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Kills : ");
				int killCursorPosY = ImGui::GetCursorPosY();
				int suicides = 0;
				int j = 0;
				for (int i = 0; i < m_kills[m_winOrder[2]].size(); i++)
				{
					if (i % 4 != 0)
					{
						ImGui::SameLine();
					}
					ImGui::Text("\tP%d ", (m_kills[m_winOrder[2]]).at(i) + 1);

				}
				if (m_kills[m_winOrder[2]].size() == 0)
				{
					ImGui::SameLine();
					string temp = "0";
					ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
					ImGui::Text(temp.c_str());

				}
				if (ImGui::GetCursorPosY() < (killCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4))
				{
					ImGui::SetCursorPosY(killCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4);
				}
				ImGui::Separator();
				ImGui::Text("Deaths : ");//	TODO: FIxa så att antal suicides skrivs ut för sig själv 
				int deathCursorPosY = ImGui::GetCursorPosY();
				for (int i = 0; i < m_killers[m_winOrder[2]].size(); i++)
				{


					if ((m_killers[m_winOrder[2]].at(i)) != m_winOrder[2])
					{
						if (j % 4 != 0)
						{
							ImGui::SameLine();
						}
						ImGui::Text("\tP%d ", (m_killers[m_winOrder[2]]).at(i) + 1);
						j++;
					}
					else
					{
						suicides++;
					}


				}

				if (j == 0)
				{
					ImGui::SameLine();
					string temp = "0";
					ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
					ImGui::Text(temp.c_str());

				}

				if (ImGui::GetCursorPosY() < (deathCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4))
				{
					ImGui::SetCursorPosY(deathCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4);
				}
				ImGui::Text("Suicides:");
				int suicidesCursorPosY = ImGui::GetCursorPosY();
				ImGui::SameLine();
				string temp = to_string(suicides);
				ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
				ImGui::Text(temp.c_str());
				ImGui::SameLine();
				ImGui::Separator();
				if (ImGui::GetCursorPosY() < (suicidesCursorPosY + ImGui::GetTextLineHeightWithSpacing()))
				{
					ImGui::SetCursorPosY(suicidesCursorPosY + ImGui::GetTextLineHeightWithSpacing());
				}
				ImGui::Separator();
				ImGui::Text("Collisions:");
				ImGui::SameLine();
				temp = to_string(m_timesCollided[m_winOrder[2]]);
				ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
				ImGui::Text(temp.c_str());

				ImGui::PopStyleColor(3);
				ImGui::PopStyleVar();
			}
			ImGui::End();
		}

		// Fourth Place
		if (m_winOrder.size() >= 4)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
			if (m_winOrder[3] == 2 || m_winOrder[3] == 3)
			{
				borderCol = 2.5;
			}
			else if (m_winOrder[3] == 1)
			{
				borderCol = 1.5;
			}
			else
			{
				borderCol = 0.5;
			}

			if (m_winOrder[3] == 3)
			{
				p4FadeCol = 0.25;
			}
			else
			{
				p4FadeCol = 0;
			}
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_playerColor[m_winOrder[3]].x - borderCol, m_playerColor[m_winOrder[3]].y - borderCol, m_playerColor[m_winOrder[3]].z - borderCol, 1));
			ImGui::SetNextWindowPos(ImVec2(width / 9 * 8 - 32, 0));
			ImGui::SetNextWindowSize(ImVec2(250, 80));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_playerColor[m_winOrder[3]].x / 3 - p4FadeCol, m_playerColor[m_winOrder[3]].y / 3 - p4FadeCol, m_playerColor[m_winOrder[3]].z / 3, 1));
			if (ImGui::Begin("##statsP4.1", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("\t\t\t\t\t 4th \n\t\t\tPlayer\t%d ", m_winOrder[3] + 1);

				ImGui::PopStyleColor(3);
				ImGui::PopStyleVar();
			}
			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_playerColor[m_winOrder[3]].x - borderCol, m_playerColor[m_winOrder[3]].y - borderCol, m_playerColor[m_winOrder[3]].z - borderCol, 1));
			ImGui::SetNextWindowPos(ImVec2(width / 9 * 8 - 32, height / 6 * 2));
			ImGui::SetNextWindowSize(ImVec2(250, 500));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_playerColor[m_winOrder[3]].x / 3 - p4FadeCol, m_playerColor[m_winOrder[3]].y / 3 - p4FadeCol, m_playerColor[m_winOrder[3]].z / 3, 1));
			if (ImGui::Begin("##statsP4.2", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Kills : ");
				int killCursorPosY = ImGui::GetCursorPosY();
				int suicides = 0;
				int j = 0;
				for (int i = 0; i < m_kills[m_winOrder[3]].size(); i++)
				{
					if (i % 4 != 0)
					{
						ImGui::SameLine();
					}
					ImGui::Text("\tP%d ", (m_kills[m_winOrder[3]]).at(i) + 1);

				}
				if (m_kills[m_winOrder[3]].size() == 0)
				{
					ImGui::SameLine();
					string temp = "0";
					ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
					ImGui::Text(temp.c_str());

				}
				if (ImGui::GetCursorPosY() < (killCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4))
				{
					ImGui::SetCursorPosY(killCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4);
				}
				ImGui::Separator();
				ImGui::Text("Deaths : ");//	TODO: FIxa så att antal suicides skrivs ut för sig själv 
				int deathCursorPosY = ImGui::GetCursorPosY();
				for (int i = 0; i < m_killers[m_winOrder[3]].size(); i++)
				{


					if ((m_killers[m_winOrder[0]].at(i)) != m_winOrder[3])
					{
						if (j % 4 != 0)
						{
							ImGui::SameLine();
						}
						ImGui::Text("\tP%d ", (m_killers[m_winOrder[3]]).at(i) + 1);
						j++;
					}
					else
					{
						suicides++;
					}


				}

				if (j == 0)
				{
					ImGui::SameLine();
					string temp = "0";
					ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
					ImGui::Text(temp.c_str());

				}

				if (ImGui::GetCursorPosY() < (deathCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4))
				{
					ImGui::SetCursorPosY(deathCursorPosY + ImGui::GetTextLineHeightWithSpacing() * 4);
				}
				ImGui::Text("Suicides:");
				int suicidesCursorPosY = ImGui::GetCursorPosY();
				ImGui::SameLine();
				string temp = to_string(suicides);
				ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
				ImGui::Text(temp.c_str());
				ImGui::SameLine();
				ImGui::Separator();
				if (ImGui::GetCursorPosY() < (suicidesCursorPosY + ImGui::GetTextLineHeightWithSpacing()))
				{
					ImGui::SetCursorPosY(suicidesCursorPosY + ImGui::GetTextLineHeightWithSpacing());
				}
				ImGui::Separator();
				ImGui::Text("Collisions:");
				ImGui::SameLine();
				temp = to_string(m_timesCollided[m_winOrder[3]]);
				ImGui::SetCursorPosX(250 - ImGui::CalcTextSize(temp.c_str()).x - 10);
				ImGui::Text(temp.c_str());

				ImGui::PopStyleColor(3);
				ImGui::PopStyleVar();
			}
			ImGui::End();
		}
		// Continue Prompt
		if (time - m_p1Seconds >= 1.0 && (glfwGetGamepadState(0, &state)))
		{
			ImGui::SetNextWindowPos(ImVec2(width / 2 - 150, height - 60));
			ImGui::SetNextWindowSize(ImVec2(300, 60));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
			if (ImGui::Begin("##statsCont", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text(" Press  \"A \"  To Continue!");

				if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
				{
					m_menu = ActiveMenu::restart;
					m_p1Seconds = time;
				}
				ImGui::PopStyleVar();
			}
			ImGui::End();
		}
		break;
	case ActiveMenu::playerHud:
		m_scene->SetBloom(true);
		m_scene->SetOnlySky(false);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 1));
		ImGui::SetNextWindowPos(ImVec2(width / 2 - 125, 0));
		ImGui::SetNextWindowSize(ImVec2(250, 60));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5));

		if (ImGui::Begin("##timer", nullptr, ImGuiWindowFlags_NoTitleBar  | ImGuiWindowFlags_AlwaysAutoResize))
		{
			int timeLeft = (int)(m_maxTime - timer);
			int minutes = timeLeft / 60;
			int seconds = timeLeft % 60;
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6, 0.6, 0.6, 1));
			ImGui::Text("Time Left : %dm : %ds", minutes, seconds);
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
		}
		ImGui::End();


		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(170, 65));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_playerColor[0].x - borderCol, m_playerColor[0].y - borderCol, m_playerColor[0].z - borderCol, 1));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p1Col.x / 3, m_p1Col.y / 3, m_p1Col.z / 3, 1));
		if (ImGui::Begin("##player1Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			int index = -1;
			int lives = 0;
			for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
			{
				if (m_objHand->GetPlayerControllerIDBloo(i, 23) == 0)
				{
					index = i;
				}
			}
			if (index != -1)
			{
				lives = m_objHand->GetPlayerLives(index);
			}

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
			//ImGui::Text("P1 Lives: %d", lives);
			ImGui::Text("P1\t ");
			ImGui::SameLine();
			ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[4]);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
			ImGui::Text("T");
			ImGui::PopStyleColor(1);
			ImGui::PopFont();
			ImGui::SameLine();
			ImGui::Text("x %d", lives);

			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(width - 171, 0));
		ImGui::SetNextWindowSize(ImVec2(170, 65));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_playerColor[1].x - borderCol, m_playerColor[1].y - borderCol - 1, m_playerColor[1].z - borderCol, 1));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p2Col.x / 3, m_p2Col.y / 3, m_p2Col.z / 3, 1));
		if (ImGui::Begin("##player2Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			int index = -1;
			int lives = 0;
			for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
			{
				if (m_objHand->GetPlayerControllerIDBloo(i, 24) == 1)
				{
					index = i;
				}
			}
			if (index != -1)
			{
				lives = m_objHand->GetPlayerLives(index);
			}
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
			//ImGui::Text("Player Two Lives: %d", lives);
			ImGui::Text("P2\t ");
			ImGui::SameLine();
			ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[4]);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
			ImGui::Text("T");
			ImGui::PopStyleColor(1);
			ImGui::PopFont();
			ImGui::SameLine();
			ImGui::Text("x %d", lives);

			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
		}
		ImGui::End();
		// TODO: Kanske Fixa s� det f�ljer kontrollerId eller n�tt
		if (m_p3Joined)//m_objHand->GetNumPlayers() >= 3) 
		{
			ImGui::SetNextWindowPos(ImVec2(0, height - 66));
			ImGui::SetNextWindowSize(ImVec2(170, 65));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_playerColor[2].x - borderCol - 2, m_playerColor[2].y - borderCol - 1, m_playerColor[2].z - borderCol, 1));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p3Col.x / 3, m_p3Col.y / 3, m_p3Col.z / 3, 1));
			if (ImGui::Begin("##player3Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				int index = -1;
				int lives = 0;
				for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
				{
					if (m_objHand->GetPlayerControllerIDBloo(i, 25) == 2)
					{
						index = i;
					}
				}
				if (index != -1)
				{
					lives = m_objHand->GetPlayerLives(index);
				}
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				//ImGui::Text("Player Three Lives: %d", lives);
				ImGui::Text("P3\t ");
				ImGui::SameLine();
				ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[4]);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				ImGui::Text("T");
				ImGui::PopStyleColor(1);
				ImGui::PopFont();
				ImGui::SameLine();
				ImGui::Text("x %d", lives);

				ImGui::PopStyleColor(3);
				ImGui::PopStyleVar();
			}
			ImGui::End();
		}

		if (m_p4Joined)//m_objHand->GetNumPlayers() >= 4)
		{
			ImGui::SetNextWindowPos(ImVec2(width - 171, height - 66));
			ImGui::SetNextWindowSize(ImVec2(170, 65));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5);
			p4FadeCol = 0.25;
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(m_playerColor[3].x - borderCol - 2, m_playerColor[3].y - borderCol - 2, m_playerColor[3].z - borderCol, 1));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p4Col.x / 3 - p4FadeCol, m_p4Col.y / 3 - p4FadeCol, m_p4Col.z / 3 , 1));
			if (ImGui::Begin("##player4Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				int index = -1;
				int lives = 0;
				for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
				{
					if (m_objHand->GetPlayerControllerIDBloo(i, 26) == 3)
					{
						index = i;
					}
				}
				if (index != -1)
				{
					lives = m_objHand->GetPlayerLives(index);
				}
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				//ImGui::Text("Player Four lives: %d", lives);
				ImGui::Text("P4\t ");
				ImGui::SameLine();
				ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[4]);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				ImGui::Text("T");
				ImGui::PopStyleColor(1);
				ImGui::PopFont();
				ImGui::SameLine();
				ImGui::Text("x %d", lives);

				ImGui::PopStyleColor(3);
				ImGui::PopStyleVar();
			}
			ImGui::End();
		}
		break;
	default:
		break;
	}
}

void Menu::SetActiveMenu(ActiveMenu activeMenu)
{
	m_menu = activeMenu;
}

void Menu::SetWinner(int playerID)
{
	m_winnerID = playerID;
}

bool Menu::SelectMenuActive()
{
	if (m_menu == ActiveMenu::select)
		return true;
	else
		return false;
}

bool Menu::SelectLivesMenuActive()
{
	if (m_menu == ActiveMenu::selectLives)
		return true;
	else
		return false;
}

bool Menu::StartMenuActive()
{
	if (m_menu == ActiveMenu::start)
		return true;
	else
		return false;
}

bool Menu::RestartMenuActive()
{
	if (m_menu == ActiveMenu::restart)
		return true;
	else
		return false;
}

bool Menu::WinMenuActive()
{
	if (m_menu == ActiveMenu::win)
		return true;
	else
		return false;
}

bool Menu::StatsMenuActive()
{
	if (m_menu == ActiveMenu::stats)
		return true;
	else
		return false;
}

bool Menu::GameOn()
{
	if (m_menu == ActiveMenu::playerHud)
		return true;
	else
		return false;
}

int Menu::GetMaxTime()
{
	return m_maxTime;
}

void Menu::LoadMenuPic()
{

	// Main menu background 
	glGenTextures(0, &m_mainMenuPic);
	glBindTexture(GL_TEXTURE_2D, m_mainMenuPic);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	Window* w = m_scene->GetOurWindow();

	m_menuPicWidth = 0;
	m_menuPicHeight = 0;
	unsigned char* data = stbi_load("src/Textures/menu1.png", &m_menuPicWidth, &m_menuPicHeight, NULL, 4);
	if (data)
	{
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glTexImage2D(GL_TEXTURE_2D,
			0, GL_RGB, m_menuPicWidth, m_menuPicHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
		);
		stbi_image_free(data);

	}
	else
	{
		std::cout << "MainMenu texture failed to load at path: " << "src/Textures/menu1.png" << std::endl;
		stbi_image_free(data);
	}
}

bool Menu::Pause()
{
	if (m_menu != ActiveMenu::playerHud && m_menu != ActiveMenu::noMenu && m_menu != ActiveMenu::select)
		return true;
	return false;
}

bool Menu::Reset()
{
	return m_reset;
}

void Menu::ResetReset()
{
	m_reset = false;
	// reseta ocks� alla v�rden som m�ste resettas
	for (int i = 0; i < 4; i++)
	{
		m_selected[i] = 0;
	}
	m_objHand->SetNumberOfLives(m_maxLives);
	m_continue = 0;

	for (int i =  0; i < 4; i++)
	{
		m_killers[i].clear();
		m_kills[i].clear();

		m_lastCollied[i] = -1;
		m_timesCollided[i] = 0;
		m_points[i] = -1;
	}

	m_deathOrderID.clear();
	m_winOrder.clear();

}

void Menu::CollisionTracking()
{
 	m_objHand->SetCollisionHappened(false);
	//int oldAIndex = 
	int aId = m_objHand->GetACollisionId();
	int bId = m_objHand->GetBCollisionId();
	double time = glfwGetTime();

	m_timesCollided[aId] += 1;
	m_lastCollied[aId] = bId;
	m_lastCollisionTime[aId] = time;
	m_timesCollided[bId] += 1;
	m_lastCollied[bId] = aId;
	m_lastCollisionTime[bId] = time;

}

void Menu::KillCount()
{
	m_objHand->SetDeath(false);
	int deadId = m_objHand->GetDeadId();
	int killerId = m_lastCollied[deadId];
	double time = glfwGetTime();

	if (time - m_lastCollisionTime[deadId] <= 6)
	{
		// if killerId == -1 then you killed yourself
		if (killerId != -1)
		{
			m_killers[deadId].push_back(killerId);
			m_kills[killerId].push_back(deadId);
		}
		else
		{
			// if killerId == -1 then you killed yourself so set killerID to your ID
			m_killers[deadId].push_back(deadId);
		}
	}
	else
	{
		// you killed yourself so set killerID to your Id
		m_killers[deadId].push_back(deadId);
	}


	
}

void Menu::RankPlayers()
{
	int winnerIndex = 0;
	int winnerLife = 0;

	int last = -1;
	int secondLast = -1;
	int thirdLast = -1;
	int fourthLast = -1;
	vector<int> deathOrder = m_objHand->GetDeathOrder();
	//TODO: add shared placing if same amount of lifes and kills

	// getting the order players died in
	for (int i = deathOrder.size() - 1; i >= 0; i--)
	{
		if (i == 0)
		{
			m_deathOrderID.push_back(deathOrder[i]); // first to die fourth in list
		}
		else if (i == 1)
		{
			m_deathOrderID.push_back(deathOrder[i]); // second to die third in list
		}
		else if (i == 2)
		{
			m_deathOrderID.push_back(deathOrder[i]); // third to die second in list
		}
		else if (i == 3)
		{
			m_deathOrderID.push_back(deathOrder[i]); // fourth to die first in list
		}
	}

	// give points for kills and lives left
	for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
	{
		m_points[m_objHand->GetPlayerControllerID(i)] += m_kills[m_objHand->GetPlayerControllerID(i)].size() * 2;
		m_points[m_objHand->GetPlayerControllerID(i)] += m_objHand->GetPlayerLives(i);
	}

	
	int livesForPlayerWithIndex[4] = { -1, -1 , -1, -1 };
	int newLifeId[4] = { -1, -1 , -1, -1 };
	int tempLives = -1;
	int newI = -1;

	for (int j = 0; j < m_objHand->GetNumPlayers(); j++)
	{
		int tempLives = -1;
		for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
		{
			if (tempLives < m_points[m_objHand->GetPlayerControllerID(i)] && i != livesForPlayerWithIndex[0] && i != livesForPlayerWithIndex[1] && i != livesForPlayerWithIndex[2] && i != livesForPlayerWithIndex[3])
			{
				tempLives = m_points[m_objHand->GetPlayerControllerID(i)];
				newI = i;
				livesForPlayerWithIndex[j] = i;
			}
		}
		newLifeId[j] = m_objHand->GetPlayerControllerID(newI);
	}


	/*


	// getting the amount of lives the remaining players have
	if (m_deathOrderID.size() == 0)
	{
		for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
		{
			livesForPlayerWithIndex[i] = m_objHand->GetPlayerLives(i);
		}

		int newI[4] = { -1, -1 , -1, -1 };

		for (int j = 0; j < m_objHand->GetNumPlayers(); j++)
		{
			
			int tempLives = -1;

			for (int i = 0; i < 4; i++)
			{
				if (tempLives <= livesForPlayerWithIndex[i] && i != newI[0] && i != newI[1] && i != newI[2] && i != newI[3])
				{
					// if same amount of lives then check how many kills they've got
					if (tempLives == livesForPlayerWithIndex[i])
					{
						// should not give a out of bounds error since the first time it can never have the same amount of lives as -1
						if (m_kills[m_objHand->GetPlayerControllerID(newI[j])] < m_kills[m_objHand->GetPlayerControllerID(i)])
						{
							newI[j] = i;
						}
						// no need to change tempLives since they have the same amount of lives
					}
					else
					{
						tempLives = livesForPlayerWithIndex[i];
						newI[j] = i;
					}
				}
			}
			newLifeId[j] = m_objHand->GetPlayerControllerIDBloo(newI[j], 1);
		}

	}
	else if (m_deathOrderID.size() == 1)
	{
		for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
		{
			if (m_objHand->GetPlayerControllerIDBloo(i, 2) != m_deathOrderID[0])
			{
				livesForPlayerWithIndex[i] = m_objHand->GetPlayerLives(i);
			}
		}

		int newI[4] = { -1, -1 , -1, -1 };

		for (int j = 0; j < m_objHand->GetNumPlayers(); j++)
		{
			int tempLives = -1;

			for (int i = 0; i < 3; i++)
			{
				if (tempLives <= livesForPlayerWithIndex[i] && i != newI[0] && i != newI[1] && i != newI[2] && i != newI[3])
				{
					// if same amount of lives then check how many kills they've got
					if (tempLives == livesForPlayerWithIndex[i])
					{
						// should not give a out of bounds error since the first time it can never have the same amount of lives as -1
						if (m_kills[m_objHand->GetPlayerControllerID(newI[j])] < m_kills[m_objHand->GetPlayerControllerID(i)])
						{
							newI[j] = i;
						}
						// no need to change tempLives since they have the same amount of lives
					}
					else
					{
						tempLives = livesForPlayerWithIndex[i];
						newI[j] = i;
					}
				}
			}
			newLifeId[j] = m_objHand->GetPlayerControllerIDBloo(newI[j], 3);
		}
	}
	else if (m_deathOrderID.size() == 2)
	{
		for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
		{
			if (m_objHand->GetPlayerControllerIDBloo(i, 4) != m_deathOrderID[0] && m_objHand->GetPlayerControllerIDBloo(i, 5) != m_deathOrderID[1])
			{
				livesForPlayerWithIndex[i] = m_objHand->GetPlayerLives(i);
			}
		}

		int newI[4] = { -1, -1 , -1, -1 };

		for (int j = 0; j < m_objHand->GetNumPlayers(); j++)
		{
			int tempLives = -1;

			for (int i = 0; i < 2; i++)
			{
				if (tempLives <= livesForPlayerWithIndex[i] && i != newI[0] && i != newI[1] && i != newI[2] && i != newI[3])
				{
					// if same amount of lives then check how many kills they've got
					if (tempLives == livesForPlayerWithIndex[i])
					{
						// should not give a out of bounds error since the first time it can never have the same amount of lives as -1
						if (m_kills[m_objHand->GetPlayerControllerID(newI[j])] < m_kills[m_objHand->GetPlayerControllerID(i)])
						{
							newI[j] = i;
						}
						// no need to change tempLives since they have the same amount of lives
					}
					else
					{
						tempLives = livesForPlayerWithIndex[i];
						newI[j] = i;
					}
				}
			}
			newLifeId[j] = m_objHand->GetPlayerControllerIDBloo(newI[j], 6);
		}
	}// if deathOrder.size() == 3 then thers only one left alive so choose it
	else if (m_deathOrderID.size() == 3)
	{
			newLifeId[0] = m_objHand->GetPlayerControllerIDBloo(0, 10);
	}
	*/
	//	adding the deathOrder and remaning lives to the same list in order to get the final rankings
	for (int i = 0; i < 4; i++)
	{
		if (newLifeId[i] != -1)
		{
			m_winOrder.push_back(newLifeId[i]);
		}
	}
	for (int j = 0; j < m_deathOrderID.size(); j++)
	{
		m_winOrder.push_back(m_deathOrderID[j]);
	}

}

int Menu::GetWinnerIndex()
{
	int temp = 0;
	for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
	{
		if (m_objHand->GetPlayerControllerID(i) == m_winOrder[0])
		{
			temp = i;
			break;
		}
	}
	return temp;
}
bool Menu::GetMapUpdate()
{
	return m_updateMap;
}

void Menu::SetMapUpdate(bool map)
{
	m_updateMap = map;
}












/*
void Scene::renderMenu()
{
	// ## osynligtr id s� f�nster utan titel
	// id 'r vad imgui referera till s� h�ll koll p� dem
	//int width, height, nrChannels;
	//unsigned char* data = stbi_load("src/Textures/wow.png", &width, &height, &nrChannels, 0);
	//unsigned int wowTexture;
	//glGenTextures(1, &wowTexture);
	//glBindTexture(GL_TEXTURE_2D, wowTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//stbi_image_free(data);

	switch (m_menu)
	{
	case Menu::start:
		//ImGui::Image((void*)wowTexture, ImVec2(width, height));

		ImGui::SetNextWindowPos(ImVec2(-2, -2));
		ImGui::SetNextWindowSize(ImVec2(m_window->GetWidht() + 4, m_windoheight + 4));
		if (ImGui::Begin("##MainMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiNavInput_Activate))
		{

			float placement = (float)m_window->GetWidht() * 0.5f;
			//ImGui::SetWindowPos(ImVec2(placement, 50));
			//ImGui::Spacing();
			//ImGui::NextColumn();
			ImGui::SetCursorPos(ImVec2(placement - 100, 15));
			//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(placement, 0));
			ImGui::PushFont(m_window->m_fonts[1]);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5);
			ImGui::Text("KamiCarZe");
			ImGui::SetCursorPos(ImVec2(placement - 75, 300));
			if (ImGui::Button("Start", ImVec2(200, 75)))
			{
				m_menu = Menu::playerHud;
			}
			ImGui::SetCursorPos(ImVec2(placement - 75, 460));
			if (ImGui::Button("Exit", ImVec2(200, 75)))
			{
				glfwSetWindowShouldClose(GetWindow(), 1);
			}
			ImGui::PopStyleVar(1); // pop all the styles
			ImGui::PopFont();
			ImGui::PopStyleColor();

		}
		ImGui::End();
		break;
	case Menu::select:
		break;
	case Menu::playerHud:
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(200, 100));
		if (ImGui::Begin("##player1Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Player One lives: %d", 3);
		}
		ImGui::End();
		break;
	default:
		break;
	}

	/*
	if (ImGui::Begin("##MainMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize))
	{
		float placement = (float)m_window->GetWidht() * 0.5f - ImGui::GetWindowContentRegionWidth() * 0.5;
		ImGui::SetWindowPos(ImVec2(placement, 50));
		ImGui::PushFont(m_window->m_fonts[1]);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5);
		ImGui::Text("KamiCarZe");
		ImGui::PopStyleVar();
		ImGui::PopFont();
		ImGui::PopStyleColor();

	}
	ImGui::End();
}*/