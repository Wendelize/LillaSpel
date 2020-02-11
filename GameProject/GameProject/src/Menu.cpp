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
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5);
			ImGui::Text("KamiCarZe");
			ImGui::SetCursorPos(ImVec2(middle - 75, 300));
			if (ImGui::Button("Start", ImVec2(200, 75)))
			{
				m_menu = ActiveMenu::playerHud;
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
			ImGui::SetCursorPos(ImVec2(((float)w->GetWidht() / 3)/3 - 25, 15));
			//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(placement, 0));
			ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[1]);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5);
			ImGui::Text("KamiCarZe");
			ImGui::SetCursorPos(ImVec2(((float)w->GetWidht() / 3) /3, 115));
			if (ImGui::Button("Start", ImVec2(200, 75)))
			{
				m_menu = ActiveMenu::playerHud;
			}
			ImGui::SetCursorPos(ImVec2(((float)w->GetWidht() / 3)/3-25, 215));
			if (ImGui::Button("Main Menu", ImVec2(250, 75)))
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

		break;
	case ActiveMenu::playerHud:
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(200, 100));
		if (ImGui::Begin("##player1Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Player One lives: %d", 3);
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(w->GetWidht()-200, 0));
		ImGui::SetNextWindowSize(ImVec2(200, 100));
		if (ImGui::Begin("##player2Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Player Two lives: %d", 3);
		}
		ImGui::End();
		// TODO: Kanske Fixa så det följer kontrollerId eller nått
		if (m_objHand->GetNumPlayers() >= 3) 
		{
			ImGui::SetNextWindowPos(ImVec2(0, w->GetHeight() - 155));
			ImGui::SetNextWindowSize(ImVec2(200, 100));
			if (ImGui::Begin("##player3Hud", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Player Three lives: %d", 3);
			}
			ImGui::End();
		}
		
		if (m_objHand->GetNumPlayers() >= 4)
		{
			ImGui::SetNextWindowPos(ImVec2(w->GetWidht() - 200, w->GetHeight() - 155));
			ImGui::SetNextWindowSize(ImVec2(200, 100));
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
	if (m_menu == ActiveMenu::pause)
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