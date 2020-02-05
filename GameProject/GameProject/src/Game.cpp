#include "Header Files/Game.h"

Game::Game()
{
	m_objectHandler = new ObjectHandler();
	m_scene = new Scene();
	m_scene->Init();
	m_time = 0;

	m_objectHandler->AddPlayer(vec3(2, 0, 5), 0, 2, vec3(1, 2 ,0));
	m_objectHandler->SetScale(0, vec3(0.3));
	m_objectHandler->AddPlayer(vec3(2, 0, 0), 1, 1, vec3(0, 1, 0));
	m_objectHandler->SetScale(1, vec3(0.08));
	m_objectHandler->AddPlayer(vec3(-2, 0, 0), 2, 3, vec3(0, 0, 1));
	m_objectHandler->SetScale(2, vec3(0.01));
	m_objectHandler->AddPlayer(vec3(-2, 0, 5), 3, 0, vec3(1, 0, 1.5));
	

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

	Render();
}

void Game::Render()
{
	m_objects = m_objectHandler->GetObjects();
	m_scene->Render(m_objects);

	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000 || true)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Debug");
		
		ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);

		ImGui::PushItemWidth(80.f);
		ImGui::InputFloat3("Pos: ", pos, 0, 0);

		if (ImGui::Button("Add Car"))
		{
			std::cout << "Car Added! " << std::endl;

			m_objectHandler->AddPlayer(vec3(pos[0], pos[1], pos[2]), 0, 2, vec3(1, 2, 0));
			//m_objectHandler->SetScale(0, vec3(0.3));
		}

		for (int i = 0; i < m_objects.size(); i++)
		{
			if (m_objects[i]->typeId == 0)
			{
				ImGui::Text("Player %i:", i + 1);
				ImGui::Text("Position: x:%.2f , y:%.2f , z:%.2F", m_objects[i]->modelMatrix[3][0], m_objects[i]->modelMatrix[3][1], m_objects[i]->modelMatrix[3][2]);
				ImGui::Text("Dir: xyz(%.2f, y:%.2f, z:%.2f)", m_objectHandler->GetPlayerDirection(i).x, m_objectHandler->GetPlayerDirection(i).y, m_objectHandler->GetPlayerDirection(i).z);
			}
		}
		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(m_scene->GetWindow(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	m_scene->SwapBuffer();
}

GLFWwindow* Game::GetWindow()
{
	return m_scene->GetWindow();
}

