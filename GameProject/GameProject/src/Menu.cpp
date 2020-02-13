#include "Header Files/Menu.h"


Menu::Menu(Scene* scene, ObjectHandler* objHand)
{
	m_scene = scene;
	m_objHand = objHand;
}

Menu::~Menu()
{
}


void Menu::RenderMenu()
{
	GLFWgamepadstate state;
	Window* w = m_scene->GetOurWindow();
	if (m_menu == ActiveMenu::playerHud && glfwGetGamepadState(0, &state))
	{
		if (state.buttons[GLFW_GAMEPAD_BUTTON_START])
		{
			m_menu = ActiveMenu::pause;
		}
	}
	double time = glfwGetTime();
	switch (m_menu)
	{
	case ActiveMenu::start:
		//ImGui::Image((void*)wowTexture, ImVec2(width, height));

		ImGui::SetNextWindowPos(ImVec2(-2, -2));
		ImGui::SetNextWindowSize(ImVec2((float)w->GetWidht() + 4, w->GetHeight() + 4));
		if (ImGui::Begin("##MainMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiNavInput_Activate))
		{

			float middle = (float)w->GetWidht() * 0.5f;
			//ImGui::SetWindowPos(ImVec2(placement, 50));
			//ImGui::Spacing();
			//ImGui::NextColumn();
			ImGui::SetCursorPos(ImVec2(middle - 100, 15));
			//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(placement, 0));
			ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[1]);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0.6, 0.8, 1));
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5);
			ImGui::Text("KamiCarZe");
			ImGui::SetCursorPos(ImVec2(middle - 75, 300));
			if (ImGui::Button("Start", ImVec2(200, 75)))
			{
				m_menu = ActiveMenu::select;
				m_p1Seconds = time;
				m_p2Seconds = time;
				m_p3Seconds = time;
				m_p4Seconds = time;
			}
			ImGui::SetCursorPos(ImVec2(middle - 75, 460));
			if (ImGui::Button("Exit", ImVec2(200, 75)))
			{
				glfwSetWindowShouldClose(m_scene->GetWindow(), 1);
			}
			ImGui::PopStyleVar(1); // pop all the styles
			ImGui::PopFont();
			ImGui::PopStyleColor();

		}
		ImGui::End();
		break;
	case ActiveMenu::select:
		ImGui::SetNextWindowPos(ImVec2(50, 25));
		ImGui::SetNextWindowSize(ImVec2(250, 100));
		if (ImGui::Begin("##player1Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs))
		{
			GLFWgamepadstate state;
			//TODO: Get player color on HUD and Select vehicle model
			vec3 p1Col  = m_objHand->GetPlayerColor(0);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(p1Col.x, p1Col.y, p1Col.z, 1));
			ImGui::Text("<  Vehicle Model: %d  > ", m_p1ModelId);
			
			if ( time - m_p1Seconds >= 0.5 && (glfwGetGamepadState(0, &state)))
			{
				if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
				{
					if ((m_p1ModelId + 1) <= m_scene->GetNumPlayerModels() - 1)
					{
						m_p1ModelId += 1;
					}
					else
					{
						m_p1ModelId = 0;
					}
					m_p1Seconds = time;
				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
				{

					if ((m_p1ModelId - 1) >= 0)
					{
						m_p1ModelId -= 1;
					}
					else
					{
						m_p1ModelId = m_scene->GetNumPlayerModels() - 1;
					}
					m_p1Seconds = time;
				}
				else if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
				{
					m_selected[0] = 1;
				}
				else if (m_selected[0] == 1 && state.buttons[GLFW_GAMEPAD_BUTTON_B])
				{
					m_selected[0] = 0;
				}
				
			}
			ImGui::PopStyleColor();
		}
		ImGui::End();

		if (m_objHand->GetNumPlayers() >= 2)
		{
			ImGui::SetNextWindowPos(ImVec2(w->GetWidht() - 300, 25));
			ImGui::SetNextWindowSize(ImVec2(250, 100));
			if (ImGui::Begin("##player2Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs))
			{
				GLFWgamepadstate state;
				vec3 p2Col = m_objHand->GetPlayerColor(1);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(p2Col.x, p2Col.y, p2Col.z, 1));
				ImGui::Text("<  Vehicle Model: %d  > ", m_p2ModelId);

				if (time - m_p2Seconds >= 0.5 && (glfwGetGamepadState(1, &state)))
				{
					if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
					{
						if ((m_p2ModelId + 1) <= m_scene->GetNumPlayerModels() - 1)
						{
							m_p2ModelId += 1;
						}
						else
						{
							m_p2ModelId = 0;
						}
						m_p2Seconds = time;
					}
					else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
					{

						if ((m_p2ModelId - 1) >= 0)
						{
							m_p2ModelId -= 1;
						}
						else
						{
							m_p2ModelId = m_scene->GetNumPlayerModels() - 1;
						}
						m_p2Seconds = time;
					}
					else if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
					{
						m_selected[1] = 1;
					}
					else if (m_selected[0] == 1 && state.buttons[GLFW_GAMEPAD_BUTTON_B])
					{
						m_selected[1] = 0;
					}

				}
				ImGui::PopStyleColor();
			}
			ImGui::End();
		}
		// TODO: Kanske Fixa så det följer kontrollerId eller nått
		if (m_objHand->GetNumPlayers() >= 3)
		{
			ImGui::SetNextWindowPos(ImVec2(50, w->GetHeight() - 155));
			ImGui::SetNextWindowSize(ImVec2(250, 100));
			if (ImGui::Begin("##player3Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs))
			{
				GLFWgamepadstate state;
				vec3 p3Col = m_objHand->GetPlayerColor(2);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(p3Col.x, p3Col.y, p3Col.z, 1));
				if (m_p3Joined == false)
				{
					if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
					{
						ImGui::Text(" Press \"A\" Button to Join! ");
					}
					if (time - m_p3Seconds >= 0.5 && (glfwGetGamepadState(2, &state)))
					{
						m_p3Joined = true;
					}
				}
				else 
				{
					ImGui::Text("<  Vehicle Model: %d  > ", m_p3ModelId);

					if (time - m_p3Seconds >= 0.5 && (glfwGetGamepadState(2, &state)))
					{
						if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
						{
							if ((m_p3ModelId + 1) <= m_scene->GetNumPlayerModels() - 1)
							{
								m_p3ModelId += 1;
							}
							else
							{
								m_p3ModelId = 0;
							}
							m_p3Seconds = time;
						}
						else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
						{

							if ((m_p3ModelId - 1) >= 0)
							{
								m_p3ModelId -= 1;
							}
							else
							{
								m_p3ModelId = m_scene->GetNumPlayerModels() - 1;
							}
							m_p3Seconds = time;
						}
						else if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
						{
							m_selected[2] = 1;
						}
						else if (m_selected[0] == 1 && state.buttons[GLFW_GAMEPAD_BUTTON_B])
						{
							m_selected[2] = 0;
						}

					}
				}
				
				ImGui::PopStyleColor();
			}
			ImGui::End();
		}

		if (m_objHand->GetNumPlayers() >= 4)
		{
			ImGui::SetNextWindowPos(ImVec2(w->GetWidht() - 300, w->GetHeight() - 155));
			ImGui::SetNextWindowSize(ImVec2(250, 100));
			if (ImGui::Begin("##player4Select", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNavInputs))
			{
				GLFWgamepadstate state;
				vec3 p4Col = m_objHand->GetPlayerColor(3);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(p4Col.x, p4Col.y, p4Col.z, 1));
				ImGui::Text("<  Vehicle Model: %d  > ", m_p4ModelId);

				if (time - m_p4Seconds >= 0.5 && (glfwGetGamepadState(2, &state)))
				{
					if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
					{
						if ((m_p4ModelId + 1) <= m_scene->GetNumPlayerModels() - 1)
						{
							m_p4ModelId += 1;
						}
						else
						{
							m_p4ModelId = 0;
						}
						m_p4Seconds = time;
					}
					else if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
					{

						if ((m_p4ModelId - 1) >= 0)
						{
							m_p4ModelId -= 1;
						}
						else
						{
							m_p4ModelId = m_scene->GetNumPlayerModels() - 1;
						}
						m_p4Seconds = time;
					}
					else if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
					{
						m_selected[3] = 1;
					}
					else if (m_selected[0] == 1 && state.buttons[GLFW_GAMEPAD_BUTTON_B])
					{
						m_selected[3] = 0;
					}

				}
				ImGui::PopStyleColor();
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
			m_menu = ActiveMenu::playerHud;

			for (int i = 0; i < m_objHand->GetNumPlayers(); i++)
			{
				m_selected[i] = 0;
			}
		}
		m_continue = 0;
		break;
	case ActiveMenu::pause:
		ImGui::SetNextWindowPos(ImVec2((float)w->GetWidht() / 3, w->GetHeight() / 4 ));
		ImGui::SetNextWindowSize(ImVec2((float)w->GetWidht()/3 /*- 150*/, w->GetHeight()/4 + 150));
		if (ImGui::Begin("##PauseMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiNavInput_Activate))
		{

			float middle = (float)w->GetWidht() * 0.5f;
			//ImGui::SetWindowPos(ImVec2(placement, 50));
			//ImGui::Spacing();
			//ImGui::NextColumn();
			ImGui::SetCursorPos(ImVec2(((float)w->GetWidht() / 3)/3 - 50, 15));
			//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(placement, 0));
			ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[1]);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0.6, 0.8, 1));
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5);
			ImGui::Text("KamiCarZe");
			ImGui::SetCursorPos(ImVec2(((float)w->GetWidht() / 3) /3, 115));
			if (ImGui::Button("Start", ImVec2(200, 75)))
			{
				m_menu = ActiveMenu::playerHud;
			}
			ImGui::SetCursorPos(ImVec2(((float)w->GetWidht() / 3)/3-100, 215));
			if (ImGui::Button("Main Menu", ImVec2(400, 75)))
			{
				m_menu = ActiveMenu::start;
			}
			ImGui::SetCursorPos(ImVec2(((float)w->GetWidht() / 3) / 3, 315));
			if (ImGui::Button("Exit", ImVec2(200, 75)))
			{
				glfwSetWindowShouldClose(m_scene->GetWindow(), 1);
			}
			ImGui::PopStyleVar(1); // pop all the styles
			ImGui::PopFont();
			ImGui::PopStyleColor();

		}
		ImGui::End();
		break;
	case ActiveMenu::restart:
		ImGui::SetNextWindowPos(ImVec2((float)w->GetWidht() / 3, w->GetHeight() / 4));
		ImGui::SetNextWindowSize(ImVec2((float)w->GetWidht() / 3 /*- 150*/, w->GetHeight() / 4 + 150));
		if (ImGui::Begin("##RestartMenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiNavInput_Activate))
		{

			float middle = (float)w->GetWidht() * 0.5f;
			//ImGui::SetWindowPos(ImVec2(placement, 50));
			//ImGui::Spacing();
			//ImGui::NextColumn();
			ImGui::SetCursorPos(ImVec2(((float)w->GetWidht() / 3) / 3 - 50, 15));
			//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(placement, 0));
			ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[1]);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0.6, 0.8, 1));
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5);
			ImGui::Text("KamiCarZe");
			ImGui::SetCursorPos(ImVec2(((float)w->GetWidht() / 3) / 3, 115));
			if (ImGui::Button("Restart", ImVec2(200, 75)))
			{
				m_menu = ActiveMenu::playerHud;
			}
			ImGui::SetCursorPos(ImVec2(((float)w->GetWidht() / 3) / 3 - 100, 215));
			if (ImGui::Button("Main Menu", ImVec2(400, 75)))
			{
				m_menu = ActiveMenu::start;
			}
			ImGui::SetCursorPos(ImVec2(((float)w->GetWidht() / 3) / 3, 315));
			if (ImGui::Button("Exit", ImVec2(200, 75)))
			{
				glfwSetWindowShouldClose(m_scene->GetWindow(), 1);
			}
			ImGui::PopStyleVar(1); // pop all the styles
			ImGui::PopFont();
			ImGui::PopStyleColor();

		}
		ImGui::End();
		break;
	case ActiveMenu::playerHud:
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(210, 100));
		if (ImGui::Begin("##player1Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			//TODO: get this from player maybe pull main to get their function?
			ImGui::Text("Player One lives: %d", 3);
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(w->GetWidht()-218, 0));
		ImGui::SetNextWindowSize(ImVec2(210, 100));
		if (ImGui::Begin("##player2Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Player Two lives: %d", 3);
		}
		ImGui::End();
		// TODO: Kanske Fixa så det följer kontrollerId eller nått
		if (m_objHand->GetNumPlayers() >= 3) 
		{
			ImGui::SetNextWindowPos(ImVec2(0, w->GetHeight() - 155));
			ImGui::SetNextWindowSize(ImVec2(210, 100));
			if (ImGui::Begin("##player3Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Player Three lives: %d", 3);
			}
			ImGui::End();
		}
		
		if (m_objHand->GetNumPlayers() >= 4)
		{
			ImGui::SetNextWindowPos(ImVec2(w->GetWidht() - 218, w->GetHeight() - 155));
			ImGui::SetNextWindowSize(ImVec2(210, 100));
			if (ImGui::Begin("##player4Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Player Four lives: %d", 3);
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

bool Menu::Pause()
{
	if (m_menu != ActiveMenu::playerHud && m_menu != ActiveMenu::noMenu && m_menu != ActiveMenu::select)
		return true;
	return false;
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
		ImGui::SetNextWindowSize(ImVec2(m_window->GetWidht() + 4, m_window->GetHeight() + 4));
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