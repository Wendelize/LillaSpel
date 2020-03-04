#include "Header Files/Game.h"
#define STB_IMAGE_IMPLEMENTATION    
#include "Header Files/stb_image.h"
Game::Game()
{
	m_mapUpdateReady.store(false);
	m_updateMap.store(false);

	m_objectHandler = new ObjectHandler();
	m_scene = new Scene();
	m_scene->Init(); // H�r skapas modellerna
	m_time = 0;
	m_menu = new Menu(m_scene, m_objectHandler);
	// noMenu, start, select, pause, stats, restart, playerHud; // stats finns inte än
	// väl playerHud om ni vill spela utan start menu
	// välj noMenu om ni vill spela utan HUD 
	//	Pause meny bör fortfarande fungera med noMenu
	m_menu->SetActiveMenu(Menu::ActiveMenu::start);
	m_menu->LoadMenuPic();

	m_maxTime = 120.f;
	m_debug = false;
	m_toggle = false;
	m_platforms = m_scene->GetModels(0);
	m_cars = m_scene->GetModels(1);
	m_objectModels = m_scene->GetModels(3);
	m_cube = new MarchingCubes();
	m_objectHandler->AddDynamicPlatformMesh(m_cube);

	m_timeSinceSpawn = 0;

	m_objectHandler->AddPlayer(vec3(-10, 6, 3), 0, 0, vec3(0.5, 1, 9), m_cars[0]); // Passa modell
	m_objectHandler->AddPlayer(vec3(10, 6, 3), 1, 0, vec3(0, 2, 0), m_cars[2]); // Passa modell

	m_scene->SetCameraPos(CAMERAPOS_GAME);


	m_soundEngine = createIrrKlangDevice();

	if (m_soundEngine)
	{
		int randomNumber = rand () % 5;
		m_songs.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Music - 16bit Sea Shanty 2.mp3"));
		m_songs.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Music - 16bit Deja Vu.mp3"));
		m_songs.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Music - Main Game.mp3"));
		m_songs.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Music - Main Game 2 Players Left.mp3"));
		m_songs.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Music - Menu.mp3"));

		m_music = m_soundEngine->play2D(m_songs[randomNumber], true, true);

		if (m_music)
		{
			m_music->setVolume(0.9f);
			if (randomNumber == 1)
				m_music->setVolume(0.6f);
		}

		m_soundEngine->setListenerPosition(vec3df(0, 18, 33), vec3df(0, -4, 3)); // Listener position, view direction
		m_music->setIsPaused(false);
	}
}

Game::~Game()
{
	delete m_objectHandler;
	delete m_scene;
	delete m_cube;
	delete m_menu;
	if (m_soundEngine)
	{
		for (int i = 0; i < m_songs.size(); i++)
		{
			m_songs[i]->drop();

		}
		if (m_music)
			m_music->drop();

		//m_soundEngine->drop();
	}
}

void Game::Update(float dt)
{
	float dtUnchanged = dt;

	if (m_slowmoCooldown > 0)
	{
		m_slowmoCooldown -= dt;
		dt /= (m_slowmoCooldown * 1.5) + 1;
	}

	//Exhaust particles
	for (int i = 0; i < m_objectHandler->GetNumPlayers(); i++)
	{
		float speed = m_objectHandler->GetPlayerSpeed(i);
		if (speed > 15)
		{
			vec3 dir = m_objectHandler->GetPlayerDirection(i);
			vec3 pos = m_objectHandler->GetPlayerPos(i) + dir * 1.f + vec3(0, -0.45, 0);
			vec3 right = cross(dir, vec3(0, 1, 0)) * 0.5f;

			m_scene->AddParticleEffect(pos, vec3(0.6, 0.6, 0.6), vec3(0), 5, 0.005, dir, 4, 0.3, 0.3, 1);
		}
	}

	//Explosion particles and screen shake
	if (m_objectHandler->GetExplosion())
	{
		vec3 pos = m_objectHandler->GetExplosionPosition();
		m_scene->ShakeCamera(0.4f, 1);
		m_scene->AddParticleEffect(pos, vec3(0.01, 0, 0), vec3(1, 0, 0), 1, 6, vec3(0, 1, 0), 50, 1.2, 0.5, -9.82);
		m_objectHandler->SetExplosion(false);
	}

	// ska banan resettas?
	if (m_menu->Reset())
	{
		Reset();
		m_scene->TranslateCameraPos(vec3(CAMERAPOS_GAME), 1.f);
		m_objectHandler->ClearHoles();
		m_updateMap.store(true);
		m_timeSwapTrack = 0;
		m_menu->SetMapUpdate(false);
	}


	// är select menyn aktiverad? ändra kameran till inzoomad
	if (m_menu->SelectMenuActive())
	{
		SelectionMenu();
		m_scene->TranslateCameraPos(vec3(CAMERAPOS_SELECT), 1.f);
		m_maxTime = m_menu->GetMaxTime();
	}
	else if (m_menu->SelectMenuActive() == false && m_wasSelect == true)
	{
		m_wasSelect = false;
		// TODO: fixa snyggare kamera transition?
		//m_scene->SetCameraPos(CAMERAPOS_GAME);
		m_scene->TranslateCameraPos(CAMERAPOS_GAME, 1.f);
	}
	// är vi på en meny som ska pausa spelet? sluta då updatera deltaTime
	if (m_menu->Pause())
	{
		dt = 0;
		if (m_menu->GetMapUpdate()) {
			m_updateMap.store(true);
			m_menu->SetMapUpdate(false);
		}
		if (m_mapUpdateReady.load() == true && m_updateMap.load() == false)
		{
			m_objectHandler->RemoveDynamicPlatformMesh(m_cube);
			m_cube->MapUpdate();
			m_objectHandler->AddDynamicPlatformMesh(m_cube);
			m_timeSwapTrack = 0.f;
			m_mapUpdateReady.store(false);
			m_objectHandler->ClearBombs();
			if (m_cube->GetCurrentLevel() == 5) {
				m_objectHandler->RemoveAllObjects();
				m_objectHandler->AddObject(vec3(0, 2, 0), 0, m_objectModels[0]);
			}
			else {
				m_objectHandler->RemoveAllObjects();
			}
		}
	}
	if ((!m_menu->Pause() && !m_wasSelect)) // Vet inte om det kan göras snyggare?
	{
		m_time += dt;
		m_timeSinceSpawn += dt;
		m_timeSwapTrack += dt;
		if ((m_timeSwapTrack > 2.f && m_updateMap.load() == false && m_mapUpdateReady.load() == false) || m_menu->GetMapUpdate())
		{
			m_updateMap.store(true);
		}

		if (m_mapUpdateReady.load() == true && m_updateMap.load() == false)
		{
			m_objectHandler->RemoveDynamicPlatformMesh(m_cube);
			m_cube->MapUpdate();
			m_objectHandler->AddDynamicPlatformMesh(m_cube);
			m_timeSwapTrack = 0.f;
			m_mapUpdateReady.store(false);
			m_objectHandler->ClearBombs();
		}
		if (m_timeSinceSpawn > 5 && !m_gameOver)
		{
			m_objectHandler->AddPowerUp();
			m_timeSinceSpawn = 0;
		}
		if (!m_gameOver){
			m_objectHandler->Update(dt);
			if (m_objectHandler->GetSpawnBall()) {
				m_objectHandler->AddObject(vec3(0, 20, 0), 1, m_objectModels[1]);
				m_objectHandler->SetSpawnBall(false);
			}
		}

		if (m_objectHandler->GetNumPlayers() == 1 && !m_gameOver)
		{
			m_winner = 0;
			m_menu->RankPlayers();
			m_menu->SetActiveMenu(Menu::ActiveMenu::win);
			m_gameOver = true;

			if (m_soundEngine)
			{
				m_soundEngine->stopAllSounds();
				m_soundEngine->play2D("src/Audio/Music - Win.mp3", true);
				m_objectHandler->StopAllSound();
			}
		}
		if (m_maxTime - m_time <= 30.f && !m_fastMusic)
		{
			m_objectHandler->AddObject(vec3(rand() % 10 - 5, 10, rand() & 10 - 5), 1, m_objectModels[1]);
			if (m_soundEngine)
			{
				m_music->setPlaybackSpeed(1.2);
			}
			m_fastMusic = true;
		}
		if (m_time > m_maxTime && !m_gameOver)
		{
			m_menu->RankPlayers();
			m_winner = m_menu->GetWinner();//m_objectHandler->GetWinnerID();
			//m_menu->SetWinner(m_winner);
			m_menu->SetActiveMenu(Menu::ActiveMenu::win);
			m_gameOver = true;
			if (m_soundEngine)
			{
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

		if (m_objectHandler->GetCollisionHappened())
		{
			m_menu->CollisionTracking();
			int aId = m_objectHandler->GetACollisionId();
			int bId = m_objectHandler->GetBCollisionId();
			vec3 pos = m_objectHandler->GetPlayerPos(m_objectHandler->GetIndexByControllerId(aId));
			pos += m_objectHandler->GetPlayerPos(m_objectHandler->GetIndexByControllerId(bId));

			m_scene ->AddParticleEffect(pos / 2.f, vec3(1, 0, 0), vec3(0, 1, 0), 1, 6, vec3(0, 1, 0), 200, 0.5, 0.15, -9.82);

			if (m_objectHandler->GetNumPlayers() == 2)
			{
				if (m_objectHandler->GetPlayerLives(0) == 1 || m_objectHandler->GetPlayerLives(1) == 1)
				{
					m_slowmoCooldown = 1.f;
				}
			}

		}

		if (m_objectHandler->GetDeath())
		{
			m_menu->KillCount();
		}
	}

	DynamicCamera(dtUnchanged);
	m_scene->UpdateCamera(dtUnchanged);
	m_scene->UpdateSky(dtUnchanged);
	m_scene->UpdateParticles(dtUnchanged);
}

void Game::DynamicCamera(float dt)
{
	vec3 focusPoint = vec3(0);
	vec3 offset = vec3(0, -6, 0);
	int numPlayers = m_objectHandler->GetNumPlayers();

	if (m_menu->WinMenuActive() || m_menu->RestartMenuActive() || m_menu->StatsMenuActive())
	{
		focusPoint = m_objectHandler->GetPlayerPos(m_winner) + vec3(0, 0.5, 0);
	}
	else
	{
		for (int i = 0; i < numPlayers; i++)
		{
			if (m_objectHandler->GetPlayerPos(i).y > 0)
			{
				focusPoint += m_objectHandler->GetPlayerPos(i);
			}
		}
		focusPoint /= vec3(numPlayers);
	}

	if (dt < 1.f)
		m_scene->SetCameraFocus(focusPoint);

	if (m_menu->WinMenuActive() || m_menu->RestartMenuActive() || m_menu->StatsMenuActive())
	{
		vec3 infront = normalize(m_objectHandler->GetPlayerPos(m_winner) - (m_objectHandler->GetPlayerPos(m_winner) - m_objectHandler->GetPlayerDirection(m_winner)));
		vec3 right = cross(infront, vec3(0, 1, 0));

		vec3 vec = m_objectHandler->GetPlayerPos(m_winner) + -infront * 3.f + right * 3.f + vec3(0, -0.6, 0);

		m_scene->TranslateCameraPos(vec, 1.f);

		right = cross(normalize(m_objectHandler->GetPlayerPos(m_winner) - vec), vec3(0, 1, 0));

		vec3 in = m_objectHandler->GetPlayerPos(m_winner) - (m_objectHandler->GetPlayerPos(m_winner) + right * 2.0f);


		m_scene->AddParticleEffect(m_objectHandler->GetPlayerPos(m_winner) + right * 2.0f + vec3(0, -1, 0), vec3(NULL), vec3(NULL), 1, 0.9, vec3(0, 7, 0) + in + vec3(0, -1, 0), 10, 0.9, 0.04, -9.82);
		m_scene->AddParticleEffect(m_objectHandler->GetPlayerPos(m_winner) - right * 2.0f + vec3(0, -1, 0), vec3(NULL), vec3(NULL), 1, 0.9, vec3(0, 7, 0) - in + vec3(0, -1, 0), 10, 0.9, 0.04, -9.82);

		m_fireworkCooldown += dt;

		if (m_fireworkCooldown >= 0.3)
		{
			vec3 behind = m_objectHandler->GetPlayerPos(m_winner) + (m_objectHandler->GetPlayerPos(m_winner) - vec) * 13.f + vec3(0, 26, 0);

			vec3 x = normalize(in) * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 60.f;
			x = x * 2.f - normalize(in) * 60.f;

			vec3 y = vec3(0, 1, 0) * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 13.f;
			y = y * 2.f - vec3(0, 1, 0) * 13.f;

			vec3 randPos = x + y;

			vec3 randCol;
			randCol.x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
			randCol.y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
			randCol.z = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

			m_scene->AddParticleEffect(behind + randPos, randCol, vec3(0, 0, 0), 2, 6, vec3(1, 0, 0), 50, 0.8, 0.4, -9.82);
			m_fireworkCooldown = 0;
		}
	}
	else
	{
		mat4 matrix = m_scene->GetProjMatrix() * m_scene->GetCameraView();

		int innerFrustum = 0;
		int outerFrustum = 0;
		int outside = 0;
		for (int i = 0; i < numPlayers; i++)
		{
			vec4 modelSpace = matrix * vec4(m_objectHandler->GetPlayerPos(i), 1);

			if (-modelSpace.w < modelSpace.x && modelSpace.x < modelSpace.w && -modelSpace.w < modelSpace.y && modelSpace.y < modelSpace.w)
			{
				if (-modelSpace.w < modelSpace.x - 20 && modelSpace.x + 20 < modelSpace.w && -modelSpace.w < modelSpace.y - 20 && modelSpace.y + 20 < modelSpace.w)
				{
					innerFrustum++;
				}
				else if (-modelSpace.w > modelSpace.x - 19 || modelSpace.x + 19 > modelSpace.w || -modelSpace.w > modelSpace.y - 19 || modelSpace.y + 19 > modelSpace.w)
				{
					outerFrustum++;
				}
			}
			else
			{
				outside++;
			}
		}

		if (innerFrustum == numPlayers && dt < 1.f)
		{
			m_scene->ZoomIn(dt * 5);
		}
		else if ((outerFrustum != 0 || outside != 0) && dt < 1.f)
		{
			m_scene->ZoomOut(dt * 6);
		}
	}
}

void Game::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow();

	m_menu->RenderMenu(m_gameOver, m_time, m_cars[0]);

	
	m_objects = m_objectHandler->GetObjects();
	m_carLight = m_objectHandler->GetLights();
	m_objectHandler->RenderParticles();
	m_scene->RenderLights(m_carLight);
	m_scene->Render(m_objects, m_objectHandler->GetWorld(), m_cube, m_gameOver, m_winner, m_objectHandler->GetLightsOut());
//
//	m_scene->Render(m_objects, m_objectHandler->GetWorld(), m_cube);

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
	glfwPollEvents();
}

void Game::Reset()
{
	m_menu->ResetReset();
	m_gameOver = false;
	m_time = 0;
	m_objectHandler->SetLightsOut(false);
	//m_maxTime = 240.f;
	m_timeSinceSpawn = 0;
	// delete remaning players so we can spawn them back att spawn positions
	int nrOfPup = m_objectHandler->GetNumPowerUps();
	for (int i = 0; i < nrOfPup; i++)
	{
		m_objectHandler->RemovePowerUp(0);
	}
	m_objectHandler->RemoveAllObjects();
	if (m_cube->GetCurrentLevel() == 5) {
		m_objectHandler->AddObject(vec3(0, 2, 0), 0, m_objectModels[0]);
	}
	if (m_soundEngine)
	{
		srand(time(NULL));
		int randomNumber = rand() % 5;
		m_soundEngine->stopAllSounds();
		m_music = m_soundEngine->play2D(m_songs[randomNumber], true, true);
		m_music->setIsPaused(false);
	}
	
}

GLFWwindow* Game::GetWindow()
{
	return m_scene->GetWindow();
}

void Game::MutliThread(GLFWwindow* window)
{
	while (!glfwWindowShouldClose(window)) {
		if (m_updateMap.load()) {
			m_cube->Update(window, m_objectHandler->GetBomb());
			m_updateMap.store(false);// = false;
			m_mapUpdateReady.store(true);
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}

void Game::Debug()
{
	// TODO: BUG: fixa så debug inte crashar om det är uppe och nån spelare deletas (förlorar alla liv)

	/*ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();*/
	bool temp = false;
	ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[2]);
	ImGui::Begin("Stats", &temp, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Display Size: W:%.0f, H:%.0f", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
	ImGui::Text("Time: %.2fs", m_time);
	ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
	ImGui::Text("PlayerModels: %i", m_scene->GetNumPlayerModels());
	ImGui::Text("PlatformModels: %i", m_scene->GetNumPlatformModels());
	ImGui::Text("PowerUpModels: %i", m_scene->GetNumPowerUpModels());
	ImGui::PopFont();

	ImGui::End();

	ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[2]);
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
		ImGui::PopFont();
		ImGui::End();
		ImGui::PushFont(m_scene->GetOurWindow()->m_fonts[2]);
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
	ImGui::PopFont();

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

void Game::SelectionMenu()
{
	if (m_wasSelect == false)
	{
		m_scene->SetCameraPos(vec3(CAMERAPOS_SELECT));
		m_wasSelect = true;
	}
	
	
}


