#include "Header Files/Menu.h"
#include "Header Files/stb_image.h"


Menu::Menu(Scene* scene, ObjectHandler* objHand)
{
	m_scene = scene;
	m_objHand = objHand;

	
	
}

Menu::~Menu()
{
}


void Menu::RenderMenu(bool gameOver, float timer,Model* model)
{
	GLFWgamepadstate state;
	Window* w = m_scene->GetOurWindow();
	if (gameOver)
	{
		m_menu = ActiveMenu::restart;
	}
	else if ((m_menu == ActiveMenu::playerHud || m_menu == ActiveMenu::select) && glfwGetGamepadState(0, &state))
	{
		// TODO: maybe one should be able to access the pause menu from more places?
		if (state.buttons[GLFW_GAMEPAD_BUTTON_START])
		{
			m_menu = ActiveMenu::pause;
		}
	}
	double time = glfwGetTime();

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
			float middle = (float)width * 0.5f;
			ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[1]);

			//	NavHighlight is the border around the button
			ImGui::PushStyleColor(ImGuiCol_NavHighlight, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2, 0.2, 0.2, 1));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7, 0.1, 0, 0.3));
			ImGui::SetCursorPos(ImVec2(middle - 75, 500 - mainMenuButonHeight));
			if (ImGui::Button("Start", ImVec2(200, 75)))
			{
				m_menu = ActiveMenu::select;
				m_p1Seconds = time;
				m_p2Seconds = time;
				m_p3Seconds = time;
				m_p4Seconds = time;
			}
			ImGui::SetCursorPos(ImVec2(middle - 75, 600 - mainMenuButonHeight));
			if (ImGui::Button("Exit", ImVec2(200, 75)))
			{
				glfwSetWindowShouldClose(m_scene->GetWindow(), 1);
			}
			//ImGui::PopStyleVar(1); // pop all the styles
			ImGui::PopFont();
			ImGui::PopStyleColor(4);

		}
		ImGui::End();
		break;
	case ActiveMenu::select:
		ImGui::SetNextWindowPos(ImVec2(50, 25));
		ImGui::SetNextWindowSize(ImVec2(350, 100));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p1Col.x, m_p1Col.y, m_p1Col.z, 1));
		if (ImGui::Begin("##player1Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs))
		{
			//GLFWgamepadstate state;
			// letar upp rätt index för modellen för de deletas och addas om och om ien så de behåller ej standard
			// just nu sätts färgera en gång och ändras aldrig men man kanske vill ändra dem senare
			// så därför finns detta (for-loopen som letar upp rätt index) inbyggt
			int index = 0;
			for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
			{
				if (m_objHand->GetPlayerControllerID(i) == 0)
				{
					index = i;
				}
			}
			m_p1Col = m_objHand->GetPlayerColor(index);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
			if (m_selected[0] == 0)
			{
				ImGui::Text("\t\t\t<  Vehicle Model : %d  > ", m_p1ModelId);
			}
			else
			{
				ImGui::Text("\t\t\tVehicle Selected! \n\t\tPress \"B\" to Unselect!");
			}

			if (time - m_p1Seconds >= 0.5 && (glfwGetGamepadState(0, &state)))
			{
				if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] && m_selected[0] == 0)
				{
					if ((m_p1ModelId + 1) <= m_scene->GetNumPlayerModels() - 1)
					{
						m_p1ModelId += 1;
					}
					else
					{
						m_p1ModelId = 0;
					}

					m_objHand->RemovePlayer(index);
					m_objHand->AddPlayer(vec3(-10, 2, 3), 0, m_p1ModelId, m_p1Col, model);

					m_p1Seconds = time;
				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] && m_selected[0] == 0)
				{

					if ((m_p1ModelId - 1) >= 0)
					{
						m_p1ModelId -= 1;
					}
					else
					{
						m_p1ModelId = m_scene->GetNumPlayerModels() - 1;
					}

					m_objHand->RemovePlayer(index);
					m_objHand->AddPlayer(vec3(-10, 2, 3), 0, m_p1ModelId, m_p1Col, model);

					m_p1Seconds = time;
				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
				{
					m_selected[0] = 1;
					m_p1Seconds = time;
				}
				else if (m_selected[0] == 1 && state.buttons[GLFW_GAMEPAD_BUTTON_B])
				{
					m_selected[0] = 0;
				}

			}
			ImGui::PopStyleColor(2);
		}
		ImGui::End();

		if (m_objHand->GetNumPlayers() >= 2)
		{
			ImGui::SetNextWindowPos(ImVec2(width - 400, 25));
			ImGui::SetNextWindowSize(ImVec2(350, 100));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p2Col.x, m_p2Col.y, m_p2Col.z, 1));
			if (ImGui::Begin("##player2Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs))
			{
				//GLFWgamepadstate state;
				int index = 1;
				for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
				{
					if (m_objHand->GetPlayerControllerID(i) == 1)
					{
						index = i;
					}
				}
				m_p2Col = m_objHand->GetPlayerColor(index);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				if (m_selected[1] == 0)
				{
					ImGui::Text("\t\t\t<  Vehicle Model : %d  > ", m_p2ModelId);
				}
				else
				{
					ImGui::Text("\t\t\tVehicle Selected! \n\t\tPress \"B\" to Unselect!");

				}

				if (time - m_p2Seconds >= 0.5 && (glfwGetGamepadState(1, &state)))
				{
					if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] && m_selected[1] == 0)
					{
						if ((m_p2ModelId + 1) <= m_scene->GetNumPlayerModels() - 1)
						{
							m_p2ModelId += 1;
						}
						else
						{
							m_p2ModelId = 0;
						}
						m_objHand->RemovePlayer(index);
						m_objHand->AddPlayer(vec3(10, 2, 3), 1, m_p2ModelId, m_p2Col, model);
						m_p2Seconds = time;
					}
					else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] && m_selected[1] == 0)
					{

						if ((m_p2ModelId - 1) >= 0)
						{
							m_p2ModelId -= 1;
						}
						else
						{
							m_p2ModelId = m_scene->GetNumPlayerModels() - 1;
						}

						m_objHand->RemovePlayer(index);
						m_objHand->AddPlayer(vec3(10, 2, 3), 1, m_p2ModelId, m_p2Col, model);
						m_p2Seconds = time;
					}
					else if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
					{
						m_selected[1] = 1;
					}
					else if (m_selected[1] == 1 && state.buttons[GLFW_GAMEPAD_BUTTON_B])
					{
						m_selected[1] = 0;
					}

				}
				ImGui::PopStyleColor(2);
			}
			ImGui::End();
		}
		// TODO: Kanske Fixa så det följer kontrollerId eller nått
		if(true)//glfwGetGamepadState(2, &state))//m_objHand->GetNumPlayers() >= 3)
		{
			ImGui::SetNextWindowPos(ImVec2(50, height - 200));
			ImGui::SetNextWindowSize(ImVec2(350, 100));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p3Col.x, m_p3Col.y, m_p3Col.z, 1));
			if (ImGui::Begin("##player3Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs))
			{
				//GLFWgamepadstate state;
				if (m_p3Joined == false)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
					if (time - m_p3Seconds >= 0.5 && (glfwGetGamepadState(2, &state)))
					{
						ImGui::Text(" Join? Press \"A\"! ");
						if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
						{
							m_p3Joined = true;
							m_objHand->AddPlayer(vec3(-7, 2, 15), 2, 0, vec3(3, 0, 0), model);
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
						if (m_objHand->GetPlayerControllerID(i) == 2)
						{
							index = i;
						}
					}
					m_p3Col = m_objHand->GetPlayerColor(index);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
					if (m_selected[2] == 0)
					{
						ImGui::Text("\t\t\t<  Vehicle Model : %d  > ", m_p3ModelId);
					}
					else
					{
						ImGui::Text("\t\t\tVehicle Selected! \n\t\tPress \"B\" to Unselect!");
					}

					if (time - m_p3Seconds >= 0.5 && (glfwGetGamepadState(2, &state)))
					{
						if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] && m_selected[2] == 0)
						{
							if ((m_p3ModelId + 1) <= m_scene->GetNumPlayerModels() - 1)
							{
								m_p3ModelId += 1;
							}
							else
							{
								m_p3ModelId = 0;
							}

							m_objHand->RemovePlayer(index);
							m_objHand->AddPlayer(vec3(-7, 2, 15), 2, m_p3ModelId, m_p3Col, model);
							m_p3Seconds = time;
						}
						else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] && m_selected[2] == 0)
						{

							if ((m_p3ModelId - 1) >= 0)
							{
								m_p3ModelId -= 1;
							}
							else
							{
								m_p3ModelId = m_scene->GetNumPlayerModels() - 1;
							}

							m_objHand->RemovePlayer(index);
							m_objHand->AddPlayer(vec3(-7, 2, 15), 2, m_p3ModelId, m_p3Col, model);
							m_p3Seconds = time;
						}
						else if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
						{
							m_selected[2] = 1;
						}
						else if (m_selected[2] == 1 && state.buttons[GLFW_GAMEPAD_BUTTON_B])
						{
							m_selected[2] = 0;
						}

					}
				}

				ImGui::PopStyleColor(2);
			}
			ImGui::End();
		}

		if (m_p3Joined && glfwGetGamepadState(3, &state))//m_objHand->GetNumPlayers() >= 4)
		{
			ImGui::SetNextWindowPos(ImVec2(width - 400, height - 200));
			ImGui::SetNextWindowSize(ImVec2(350, 100));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p4Col.x, m_p4Col.y, m_p4Col.z, 1));
			if (ImGui::Begin("##player4Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs))
			{
				
				if (m_p4Joined == false)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
					if (time - m_p4Seconds >= 0.5 && (glfwGetGamepadState(3, &state)))
					{
						ImGui::Text(" Join? Press \"A\"! ");
						if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
						{
							m_p4Joined = true;
							m_objHand->AddPlayer(vec3(7, 2, 15), 3, 0, vec3(1, 1, 0), model);
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
						if (m_objHand->GetPlayerControllerID(i) == 3)
						{
							index = i;
						}
					}
					m_p4Col = m_objHand->GetPlayerColor(index);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
					if (m_selected[3] == 0)
					{
						ImGui::Text("\t\t\t<  Vehicle Model : %d  > ", m_p3ModelId);
					}
					else
					{
						ImGui::Text("\t\t\tVehicle Selected! \n\t\tPress \"B\" to Unselect!");
					}

					if (time - m_p4Seconds >= 0.5 && (glfwGetGamepadState(3, &state)))
					{
						if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] && m_selected[3] == 0)
						{
							if ((m_p4ModelId + 1) <= m_scene->GetNumPlayerModels() - 1)
							{
								m_p4ModelId += 1;
							}
							else
							{
								m_p4ModelId = 0;
							}

							m_objHand->RemovePlayer(index);
							m_objHand->AddPlayer(vec3(7, 2, 15), 3, m_p4ModelId, m_p4Col, model);
							m_p4Seconds = time;
						}
						else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] && m_selected[3] == 0)
						{

							if ((m_p4ModelId - 1) >= 0)
							{
								m_p4ModelId -= 1;
							}
							else
							{
								m_p4ModelId = m_scene->GetNumPlayerModels() - 1;
							}

							m_objHand->RemovePlayer(index);
							m_objHand->AddPlayer(vec3(7, 2, 15), 3, m_p4ModelId, m_p4Col, model);
							m_p4Seconds = time;
						}
						else if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
						{
							m_selected[3] = 1;
						}
						else if (m_selected[3] == 1 && state.buttons[GLFW_GAMEPAD_BUTTON_B])
						{
							m_selected[3] = 0;
						}

					}
				}

				ImGui::PopStyleColor(2);
			}
			ImGui::End();
		}
		for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
		{

			if (m_selected[i] == 1)
			{
				m_continue += 1;
			}
		}

		if (m_continue == m_objHand->GetNumPlayers())
		{
			m_menu = ActiveMenu::selectLives;

			for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
			{
				m_selected[i] = 0;
			}
		}
		m_continue = 0;
		break;
	case ActiveMenu::selectLives:
		//GLFWgamepadstate state;
	////	int fillerSize = 300;
	//	if (height <= 720)
	//	{
	//		fillerSize = 200;
	//	}
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 10);
		ImGui::SetNextWindowPos(ImVec2((float)width / 3 - (slWidth / 2), height / 4));
		ImGui::SetNextWindowSize(ImVec2((float)width / 3 + slWidth, height / 4 - 300));
		if (ImGui::Begin("##selectLives", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiNavInput_Activate))
		{
			if (m_selected[0] == 0)
			{
				ImGui::Text("\t\t\t <  How Many Lives Should Players Have? : %d  > ", m_maxLives);
				ImGui::Text("\t\t\t\t Press \"B\" to Select Vehicles Again.. ");
			}
			else if (m_selected[0] == 1)
			{
				int minutes = m_maxTime / 60;
				int seconds = m_maxTime % 60;
				ImGui::Text("\t\t\t <  How Long should the Match be? : %dm : %ds > ", minutes, seconds);
				ImGui::Text("\t\t\t\t Press \"B\" to Select Lives Again.. ");
			}
			else if (m_selected[0] == 2)
			{
				m_menu = ActiveMenu::playerHud;
				m_selected[0] = 0;
				m_objHand->SetNumberOfLives(m_maxLives);
			}
			else // bör bara kunna vara -1
			{
				m_menu = ActiveMenu::select;
				m_selected[0] = 0;
			}

			if (time - m_p1Seconds >= 0.5 && (glfwGetGamepadState(0, &state)))
			{
				if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
				{
					if (m_selected[0] == 0)
					{
						if (m_maxLives < 10)
						{
							m_maxLives += 1;
						}
						else
						{
							m_maxLives = 0;
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

					if (m_selected[0] == 0)
					{
						if (m_maxLives <= 10 && m_maxLives > 0)
						{
							m_maxLives -= 1;
						}
						else
						{
							m_maxLives = 0;
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
						else
						{
							m_maxTime = 15;
						}
					}
					m_p1Seconds = time;
				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_A] && m_selected[0] <=2)
				{
					m_selected[0] += 1;
					m_p1Seconds = time;
				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_B] && m_selected[0] > -1)
				{
					m_selected[0] -= 1;
					m_p1Seconds = time;
				}

			}
			//ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}
		ImGui::End();
		break;
	case ActiveMenu::pause:
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 15);
		ImGui::SetNextWindowPos(ImVec2((float)width / 3 - (pauseMenuWidth / 2), height / 4 - (pauseMenuHeight / 2)));
		ImGui::SetNextWindowSize(ImVec2((float)width / 3 + pauseMenuWidth, height / 4 + 150 + pauseMenuHeight));
		if (ImGui::Begin("##PauseMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiNavInput_Activate))
		{

			float middle = (float)width * 0.5f;
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3 - 25, 15));
			ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[1]);
			ImGui::Text("Pause Menu");
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3, 115));
			if (ImGui::Button("Resume", ImVec2(200, 75)))
			{
				m_menu = ActiveMenu::playerHud;
			}
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3 - 100, 215));
			if (ImGui::Button("Main Menu", ImVec2(400, 75)))
			{
				// resettar banan m.m. och laddar start menyn
				m_menu = ActiveMenu::start;
				m_reset = true;
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
				if (m_objHand->GetNumPlayers() == 0)
				{
					m_objHand->AddPlayer(vec3(-10, 2, 3), 0, m_p1ModelId, m_p1Col, model);
					m_objHand->AddPlayer(vec3(10, 2, 3), 1, m_p2ModelId, m_p2Col, model);
				}
			}
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3, 315));
			if (ImGui::Button("Exit", ImVec2(200, 75)))
			{
				glfwSetWindowShouldClose(m_scene->GetWindow(), 1);
			}
			ImGui::PopStyleVar(1); // pop all the styles
			ImGui::PopFont();
			//ImGui::PopStyleColor();

		}
		ImGui::End();
		break;
	case ActiveMenu::restart:
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 15);
		ImGui::SetNextWindowPos(ImVec2((float)width / 3 - (pauseMenuWidth / 2), height / 4 - (pauseMenuHeight / 2)));
		ImGui::SetNextWindowSize(ImVec2((float)width / 3 + pauseMenuWidth, height / 4 + 150 + pauseMenuHeight));
		if (ImGui::Begin("##RestartMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiNavInput_Activate))
		{

			float middle = (float)width * 0.5f;
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3 - 10, 15));
			ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[1]);
			ImGui::Text("Start Menu");
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3 - 25, 115));
			if (ImGui::Button("Restart", ImVec2(250, 75)))
			{
				m_menu = ActiveMenu::playerHud;

				m_reset = true;
				// addar tillbaka spelarna här i Menu.cpp för att bevara deras modeller om man 
				//	bara vill resetta med samma inställnignar
				for (int i = 0; i < 4; i++)
				{
					if (m_objHand->GetNumPlayers() > 0)
					{
						m_objHand->RemovePlayer(m_objHand->GetNumPlayers() - 1);
					}
				}
				if (m_objHand->GetNumPlayers() == 0)
				{
					m_objHand->AddPlayer(vec3(-10, 2, 3), 0, m_p1ModelId, m_p1Col, model);
					m_objHand->AddPlayer(vec3(10, 2, 3), 1, m_p2ModelId, m_p2Col, model);
				}
				if (m_p3Joined && m_objHand->GetNumPlayers() == 2)
				{
					m_objHand->AddPlayer(vec3(-7, 2, 15), 2, m_p3ModelId, m_p3Col, model);
					if (m_p4Joined)
					{
						m_objHand->AddPlayer(vec3(7, 2, 15), 3, m_p4ModelId, m_p4Col, model);
					}
				}


			}
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3 - 100, 215));
			if (ImGui::Button("Main Menu", ImVec2(400, 75)))
			{
				// resettar banan m.m. och laddar start menyn
				m_menu = ActiveMenu::start;
				m_reset = true;
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
				if (m_objHand->GetNumPlayers() == 0)
				{
					m_objHand->AddPlayer(vec3(-10, 2, 3), 0, m_p1ModelId, m_p1Col, model);
					m_objHand->AddPlayer(vec3(10, 2, 3), 1, m_p2ModelId, m_p2Col, model);
				}
			}
			ImGui::SetCursorPos(ImVec2(((float)width / 3) / 3, 315));
			if (ImGui::Button("Exit", ImVec2(200, 75)))
			{
				glfwSetWindowShouldClose(m_scene->GetWindow(), 1);
			}
			ImGui::PopStyleVar(1); // pop all the styles
			ImGui::PopFont();
			//ImGui::PopStyleColor();

		}
		ImGui::End();
		break;
	case ActiveMenu::playerHud:
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
			ImGui::PopStyleColor(2);
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(250, 65));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p1Col.x, m_p1Col.y, m_p1Col.z, 1));
		if (ImGui::Begin("##player1Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			int index = -1;
			int lives = 0;
			for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
			{
				if (m_objHand->GetPlayerControllerID(i) == 0)
				{
					index = i;
				}
			}
			if (index != -1)
			{
				lives = m_objHand->GetPlayerLives(index);
			}

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
			ImGui::Text("Player One Lives: %d", lives);
			ImGui::PopStyleColor(2);
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(width - 258, 0));
		ImGui::SetNextWindowSize(ImVec2(250, 65));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p2Col.x, m_p2Col.y, m_p2Col.z, 1));
		if (ImGui::Begin("##player2Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			int index = -1;
			int lives = 0;
			for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
			{
				if (m_objHand->GetPlayerControllerID(i) == 1)
				{
					index = i;
				}
			}
			if (index != -1)
			{
				lives = m_objHand->GetPlayerLives(index);
			}
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
			ImGui::Text("Player Two Lives: %d", lives);
			ImGui::PopStyleColor(2);
		}
		ImGui::End();
		// TODO: Kanske Fixa så det följer kontrollerId eller nått
		if (m_p3Joined)//m_objHand->GetNumPlayers() >= 3) 
		{
			ImGui::SetNextWindowPos(ImVec2(0, height - 155));
			ImGui::SetNextWindowSize(ImVec2(265, 65));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p3Col.x, m_p3Col.y, m_p3Col.z, 1));
			if (ImGui::Begin("##player3Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				int index = -1;
				int lives = 0;
				for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
				{
					if (m_objHand->GetPlayerControllerID(i) == 2)
					{
						index = i;
					}
				}
				if (index != -1)
				{
					lives = m_objHand->GetPlayerLives(index);
				}
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				ImGui::Text("Player Three Lives: %d", lives);
				ImGui::PopStyleColor(2);
			}
			ImGui::End();
		}

		if (m_p4Joined)//m_objHand->GetNumPlayers() >= 4)
		{
			ImGui::SetNextWindowPos(ImVec2(width - 258, height - 155));
			ImGui::SetNextWindowSize(ImVec2(265, 65));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(m_p4Col.x, m_p4Col.y, m_p4Col.z, 1));
			if (ImGui::Begin("##player4Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				int index = -1;
				int lives = 0;
				for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
				{
					if (m_objHand->GetPlayerControllerID(i) == 3)
					{
						index = i;
					}
				}
				if (index != -1)
				{
					lives = m_objHand->GetPlayerLives(index);
				}
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
				ImGui::Text("Player Four lives: %d", lives);
				ImGui::PopStyleColor(2);
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

bool Menu::SelectMenuActive()
{
	if (m_menu == ActiveMenu::select || m_menu == ActiveMenu::selectLives)
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
	unsigned char* data = stbi_load("src/Textures/wow3.png", &m_menuPicWidth, &m_menuPicHeight, NULL, 4);
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
		std::cout << "MainMenu texture failed to load at path: " << "src/Textures/wow3.png" << std::endl;
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
	// reseta också alla värden som måste resettas
	for (int i = 0; i < 4; i++)
	{
		m_selected[i] = 0;
	}
	m_objHand->SetNumberOfLives(m_maxLives);
	m_continue = 0;
}













/*
void Scene::renderMenu()
{
	// ## osynligtr id så fönster utan titel
	// id 'r vad imgui referera till så håll koll på dem
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