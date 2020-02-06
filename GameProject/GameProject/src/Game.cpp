#include "Header Files/Game.h"

Game::Game()
{
	m_objectHandler = new ObjectHandler();
	m_scene = new Scene();
	m_scene->Init();
	m_time = 0;

	m_debug = false;
	m_toggle = false;

	m_objectHandler->AddPlayer(vec3(2, 0, 5), 0, 2, vec3(1, 1 ,0));
	m_objectHandler->SetScale(0, vec3(0.3));
	m_objectHandler->AddPlayer(vec3(2, 0, 0), 1, 1, vec3(0, 1, 0));
	m_objectHandler->SetScale(1, vec3(0.08));
	m_objectHandler->AddPlayer(vec3(-2, 0, 0), 2, 3, vec3(0, 0, 1));
	m_objectHandler->SetScale(2, vec3(0.01));
	m_objectHandler->AddPlayer(vec3(-2, 0, 5), 3, 0, vec3(1, 0, 1));
	m_objectHandler->SetScale(3, vec3(1));
	

	m_objectHandler->AddPlatform(0);
}

Game::~Game()
{
	delete m_objectHandler;
	delete m_scene;
}

void Game::Update(float dt)
{
	m_time += dt;

	m_objectHandler->Update(dt);

	// Toggle debug window
	SHORT keyState = GetAsyncKeyState(VK_LCONTROL);
	if (keyState < 0)
	{
		if (!m_toggle)
		{
			if (m_debug)
				m_debug = false;
			else
				m_debug = true;
		}
		m_toggle = true;
	}
	else
	{
		m_toggle = false;
	}

	Render();
}

void Game::Render()
{
	m_objects = m_objectHandler->GetObjects();
	m_scene->Render(m_objects);

	if (m_debug)
	{
		Debug();
	}

	m_scene->SwapBuffer();
}

GLFWwindow* Game::GetWindow()
{
	return m_scene->GetWindow();
}

void Game::Debug()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	bool temp = false;
	ImGui::Begin("Stats", &temp, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Display Size: W:%.0f, H:%.0f", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
	ImGui::Text("Time: %.2fs", m_time);
	ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
	ImGui::Text("PlayerModels: %i", m_scene->GetNumPlayerModels());
	ImGui::Text("PlatformModels: %i", m_scene->GetNumPlatformModels());
	ImGui::Text("PowerUpModels: %i", m_scene->GetNumPowerUpModels());

	ImGui::End();

	ImGui::Begin("Settings", &temp, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::BeginChild(1, ImVec2(205, 35), true);
	ImGui::Text("Controller ID: ");
	ImGui::PushItemWidth(80.f);
	ImGui::SameLine(0, 1);
	ImGui::InputInt("", &m_controllerID);
	ImGui::EndChild();

	ImGui::BeginChild(2, ImVec2(205, 35), true);
	ImGui::Text("Model ID:      ");
	ImGui::PushItemWidth(80.f);
	ImGui::SameLine(0, 1);
	ImGui::InputInt("", &m_modelId);
	ImGui::EndChild();

	ImGui::BeginChild(3, ImVec2(205, 35), true);
	ImGui::Text("Position:      ");
	ImGui::PushItemWidth(80.f);
	ImGui::SameLine(0, 1);
	ImGui::InputFloat3("", m_pos, 0, 0);
	ImGui::EndChild();

	if (ImGui::Button("Add Car"))
	{
		if (m_objectHandler->GetNumPlayers() < 4)
		{
			if(m_modelId < m_scene->GetNumPlayerModels())
				m_objectHandler->AddPlayer(vec3(m_pos[0], m_pos[1], m_pos[2]), m_controllerID, m_modelId, vec3(0.5, 0.5, 0.5));
			else
				m_objectHandler->AddPlayer(vec3(m_pos[0], m_pos[1], m_pos[2]), m_controllerID, 0, vec3(0.5, 0.5, 0.5));
		}
	}
	ImGui::SameLine(0, 2);
	if (ImGui::Button("Remove Last Car"))
	{
		if (m_objectHandler->GetNumPlayers() > 0)
		{
			m_objectHandler->RemovePlayer(m_objectHandler->GetNumPlayers() - 1);

		}
	}
	else
	{
		ImGui::End();
		ImGui::Begin("Player List", &temp, ImGuiWindowFlags_AlwaysAutoResize);
		for (int i = 0; i < m_objects.size(); i++)
		{
			if (m_objects[i]->typeId == 0)
			{
				ImGui::BeginChild(to_string(i).c_str(), ImVec2(290, 175), true);

				ImGui::Text("Vector Index: %i, Controller ID: %i", i, m_objectHandler->GetPlayerControllerID(i));
				ImGui::Text("Position: x:%.2f , y:%.2f , z:%.2F", m_objects[i]->modelMatrix[3][0], m_objects[i]->modelMatrix[3][1], m_objects[i]->modelMatrix[3][2]);
				ImGui::Text("Dir: xyz(%.2f, y:%.2f, z:%.2f)", m_objectHandler->GetPlayerDirection(i).x, m_objectHandler->GetPlayerDirection(i).y, m_objectHandler->GetPlayerDirection(i).z);

				ImGui::Text("Color (RGB): ");
				vec3 col = m_objectHandler->GetPlayerColor(i);
				
				ImGui::BeginChild(1, ImVec2(90, 35), true);
				ImGui::PushItemWidth(75.f);
				ImGui::SliderFloat(".", &col.x, 0, 1);
				ImGui::EndChild();
				ImGui::SameLine(0, 2);
				ImGui::BeginChild(2, ImVec2(90, 35), true);
				ImGui::PushItemWidth(75.f);
				ImGui::SliderFloat("", &col.y, 0, 1);
				ImGui::EndChild();
				ImGui::SameLine(0, 2);
				ImGui::BeginChild(3, ImVec2(90, 35), true);
				ImGui::PushItemWidth(75.f);
				ImGui::SliderFloat("", &col.z, 0, 1);
				ImGui::EndChild();
				m_objectHandler->SetPlayerColor(i, col);

				ImGui::Text("Scale (XYZ): ");
				vec3 scale = m_objectHandler->GetPlayerScale(i);
				ImGui::BeginChild(4, ImVec2(90, 35), true);
				ImGui::PushItemWidth(75.f);
				ImGui::SliderFloat("", &scale.x, 0, 2);
				ImGui::EndChild();
				ImGui::SameLine(0, 2);
				ImGui::BeginChild(5, ImVec2(90, 35), true);
				ImGui::PushItemWidth(75.f);
				ImGui::SliderFloat("", &scale.y, 0, 2);
				ImGui::EndChild();
				ImGui::SameLine(0, 2);
				ImGui::BeginChild(6, ImVec2(90, 35), true);
				ImGui::PushItemWidth(75.f);
				ImGui::SliderFloat("", &scale.z, 0, 2);
				ImGui::EndChild();
				m_objectHandler->SetPlayerScale(i, scale);

				ImGui::EndChild();
			}
		}
	}
	ImGui::End();

	//vec3 col = vec3((cos(m_time) + 1)/2, (cos(m_time * 0.7f) + 1)/2, (cos(m_time * 0.4f) + 1)/2);
	//m_objectHandler->SetPlayerColor(0, col);


	// Rendering
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(m_scene->GetWindow(), &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

