#include "Header Files/Game.h"
#define STB_IMAGE_IMPLEMENTATION    
#include "Header Files/stb_image.h"
Game::Game()
{
	m_objectHandler = new ObjectHandler();
	m_scene = new Scene();
	m_scene->Init(); // H�r skapas modellerna
	m_time = 0;
	m_maxTime = 240.f;
	m_menu = new Menu(m_scene, m_objectHandler);
	// noMenu, start, select, pause, stats, restart, playerHud;
	m_menu->SetActiveMenu(Menu::ActiveMenu::start);
	m_menu->LoadMenuPic();

	m_debug = false;
	m_toggle = false;
	m_platforms = m_scene->GetModels(0);
	m_cars = m_scene->GetModels(1);
	m_timeSinceSpawn = 0;
	m_objectHandler->AddPlatform(0, m_platforms[0]); // Passa modell
	srand(time(NULL));
	cout << rand() % 4 << endl;
	m_objectHandler->AddPlayer(vec3(4, 7, 4), 0, rand() % 4, vec3(0, 0, 1), m_cars[0]); // Passa modell
	m_objectHandler->AddPlayer(vec3(-4, 7, -4), 1, rand() % 4, vec3(0, 1, 0), m_cars[2]); // Passa modell
	//m_objectHandler->AddPlayer(vec3(4, 7, -4), 2, rand() % 4, vec3(1, 1, 0), m_cars[1]); // Passa modell
	//m_objectHandler->AddPlayer(vec3(-4, 7, -4), 3, rand() % 4, vec3(1, 1, 0), m_cars[3]); // Passa modell
	
	m_soundEngine = createIrrKlangDevice();

	if (m_soundEngine)
	{
		/*
			m_soundEngine->play2D("src/Audio/Music - 16bit Deja Vu.mp3", GL_TRUE);
			m_soundEngine->play2D("src/Audio/Music - Main Game 2 Players Left.mp3", GL_TRUE);
			m_soundEngine->play2D("src/Audio/Music - Main Game.mp3", GL_TRUE);
			m_soundEngine->play2D("src/Audio/Music - Menu.mp3", GL_TRUE);
			m_soundEngine->play2D("src/Audio/Music - Win.mp3", GL_TRUE);
			m_soundEngine->play2D("src/Audio/Music - 16bit Sea Shanty 2.mp3", GL_TRUE);
		*/
		int randomNumber = 4;
		m_songs.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Music - 16bit Sea Shanty 2.mp3"));
		m_songs.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Music - 16bit Deja Vu.mp3"));
		m_songs.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Music - Main Game.mp3"));
		m_songs.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Music - Main Game 2 Players Left.mp3"));
		m_songs.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Music - Menu.mp3"));

		m_music = m_soundEngine->play2D(m_songs[randomNumber], true, true);

		if (m_music)
		{
			m_music->setVolume(0.9f);
		}

		m_soundEngine->setListenerPosition(vec3df(0, 18, 33), vec3df(0, -4, 3)); // Listener position, view direction
		m_music->setIsPaused(false);
	}
}

Game::~Game()
{
	delete m_objectHandler;
	delete m_scene;

	if (m_soundEngine)
	{
		if (m_music)
			m_music->drop();

		m_soundEngine->drop();
	}
}

void Game::Update(float dt)
{
	m_time += dt;
	m_timeSinceSpawn += dt;
	if (m_timeSinceSpawn > 5 && !m_gameOver) {
		m_objectHandler->AddPowerUp();
		m_timeSinceSpawn = 0;
	}
	if (!m_menu->Pause())
	{
		m_time += dt;

	if(!m_gameOver)
		m_objectHandler->Update(dt);
	
	if (m_objectHandler->GetNumPlayers() == 1 && !m_gameOver) {
		m_gameOver = true;
		if (m_soundEngine) {
			m_soundEngine->stopAllSounds();
			m_soundEngine->play2D("src/Audio/Music - Win.mp3", true);
			m_objectHandler->StopAllSound();
		}
	}
	if (m_maxTime - m_time <= 30.f && !m_fastMusic) {
		if (m_soundEngine) {
			m_music->setPlaybackSpeed(1.4);
		}
		m_fastMusic = true;
	}
	if (m_time > m_maxTime && !m_gameOver) {
		m_gameOver = true;
		if (m_soundEngine) {
			m_soundEngine->stopAllSounds();
			m_soundEngine->play2D("src/Audio/Music - Win.mp3", true);
			m_objectHandler->StopAllSound();
		}
	}
	// Toggle debug window
	SHORT keyState = GetAsyncKeyState(VK_LCONTROL);
	if (keyState < 0)
	{
		if (!m_toggle)
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
	}
	else
	{
		m_toggle = false;
	}

	Render();
}

}

void Game::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow();

	m_menu->RenderMenu(m_gameOver);
	if(!m_menu->Pause())
	{ 
		m_objects = m_objectHandler->GetObjects();
		m_scene->Render(m_objects, m_objectHandler->GetWorld());

	}
	
	if (m_debug)
	{
		Debug();
	}

	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(GetWindow(), &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	m_scene->SwapBuffer();
}

GLFWwindow* Game::GetWindow()
{
	return m_scene->GetWindow();
}

void Game::Debug()
{
	/*ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();*/
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
	if (ImGui::Button("Add PUP"))
	{
		m_objectHandler->AddPowerUp();
	}
		if (ImGui::Button("Add Car"))
	{
		if (m_objectHandler->GetNumPlayers() < 40)
		{
			if(m_modelId < m_scene->GetNumPlayerModels())
				m_objectHandler->AddPlayer(vec3(m_pos[0], m_pos[1], m_pos[2]), m_controllerID, m_modelId, vec3(0.5, 0.5, 0.5),m_cars[0]);
			else
				m_objectHandler->AddPlayer(vec3(m_pos[0], m_pos[1], m_pos[2]), m_controllerID, 0, vec3(0.5, 0.5, 0.5),m_cars[0]);
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
	//ImGui::Render();
	//int display_w, display_h;
	//glfwGetFramebufferSize(m_scene->GetWindow(), &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

