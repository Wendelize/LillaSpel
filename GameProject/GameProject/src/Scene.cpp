#include "Header Files/Scene.h"
Scene::Scene()
{
/*	m_screenWidth = 1280;
	m_screenHeight = 720;
*/
	m_screenWidth = 1920;
	m_screenHeight = 1080;
	m_shadowMapWidth = 1200;
	m_shadowMapHeight = 1200;
	m_bloomTextureScale = 0.2f;
	m_bloomNrOfGaussianBlur = 6;

	m_window = new Window(m_screenWidth, m_screenHeight);

	m_camera = new Camera(CAMERAPOS_SELECT);
	m_modelShader = new Shader("src/Shaders/SceneVS.glsl", "src/Shaders/SceneFS.glsl");
	m_skyboxShader = new Shader("src/Shaders/SkyboxVS.glsl", "src/Shaders/SkyboxFS.glsl");
	m_skyDomeShader = new Shader("src/Shaders/SkyDomeVS.glsl", "src/Shaders/SkyDomeFS.glsl");
	m_skyPlaneShader = new Shader("src/Shaders/SkyPlaneVS.glsl", "src/Shaders/SkyPlaneFS.glsl");
	m_skybox = new Skybox();
	m_sky = new Sky();
	m_shadowMap = new ShadowMap(m_shadowMapWidth, m_shadowMapHeight);
	m_bloom = new Bloom(m_screenWidth, m_screenHeight, m_bloomTextureScale);

	m_modelMatrix = mat4(1.0);
	m_projMatrix = mat4(1.0);
	m_viewMatrix = mat4(1.0);

}

Scene::~Scene()
{
	for (uint i = 0; i < m_vehicles.size(); i++)
	{
		delete m_vehicles.at(i);
	}
	m_vehicles.clear();

	for (uint i = 0; i < m_platform.size(); i++)
	{
		delete m_platform.at(i);
	}
	m_platform.clear();

	for (uint i = 0; i < m_objects.size(); i++)
	{
		delete m_objects.at(i);
	}
	m_objects.clear();
	for (uint i = 0; i < m_power.size(); i++)
	{
		delete m_power.at(i);
	}
	m_power.clear();

	for (uint i = 0; i < m_lights.size(); i++)
	{
		delete m_lights.at(i);
	}
	m_lights.clear();

	m_carLights.clear();

	for (uint i = 0; i < m_particles.size(); i++)
	{
		delete m_particles.at(i);
	}
	m_particles.clear();

	delete m_winnerIsland;


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext(); Causing breakpoint -> Might cause memory leaks if this one is removed. -> doesn't look like it :D

	delete m_modelShader;
	delete m_skyboxShader;
	delete m_skyDomeShader;
	delete m_skyPlaneShader;
	delete m_camera;
	delete m_shadowMap;
	delete m_skybox;
	delete m_sky;
	delete m_bloom;

	
	delete m_window;
}

void Scene::Init()
{
	glEnable(GL_DEPTH_TEST);
	m_fov = 60.0f;
	m_projMatrix = perspective(radians(m_fov), (float)m_window->GetWidht() / (float)m_window->GetHeight(), 0.1f, 500.0f);
	m_viewMatrix = m_camera->GetView();
	m_modelMatrix = mat4(1.0);

	m_platform.push_back(new Model("src/Models/platform2.obj"));
	// Veichles

	// racingcar scale 0.5 
	m_vehicles.push_back(new Model("src/Models/Low-Poly-Racing-Car-Grey.obj"));
	// snowcat scale 0.08 
	m_vehicles.push_back(new Model("src/Models/Lowpoly-Snowcat2.obj"));
	// cybertruck scale 0.3 
	m_vehicles.push_back(new Model("src/Models/Cybertruck.obj"));
	// shoppingcart scale 0.01 
	m_vehicles.push_back(new Model("src/Models/shoppingcart.obj"));


	// Platforms
	m_platform.push_back(new Model("src/Models/platform2.obj"));

	// Powers
	m_power.push_back(new Model("src/Models/CartoonBomb.obj"));			//0
	m_power.push_back(new Model("src/Models/PowerUpCar.obj"));			//1
	m_power.push_back(new Model("src/Models/LowPolyController.obj"));	//2
	m_power.push_back(new Model("src/Models/Question.obj"));			//3
	m_power.push_back(new Model("src/Models/Exclamation.obj"));			//4
	m_power.push_back(new Model("src/Models/SizeDown2.0.obj"));			//5
	m_power.push_back(new Model("src/Models/SizeUP2.0.obj"));			//6
	m_power.push_back(new Model("src/Models/Bulb.obj"));				//7
	m_power.push_back(new Model("src/Models/Love.obj"));				//8
	m_power.push_back(new Model("src/Models/Rocket.obj"));				//9


	m_objects.push_back(new Model("src/Models/Log.obj"));  //M�STE VARA F�RSTA SOM LADDAS IN
	m_objects.push_back(new Model("src/Models/Ball.obj"));

	m_winnerIsland = new Model("src/Models/Island.obj");

	//m_power.push_back(new Model("src/Models/PowerUp.obj"));

	// Lights
	
	AddDirLight(vec3(-1, -1, 0), { 1,1,1 });
	/*
	AddPointLight({ 0,2,10 }, { 1, 0, 0 });
	AddPointLight({ 10,2,10 }, { 0, 1, 0 });
	AddPointLight({ 10,2,0 }, { 0, 0, 1 });
	AddPointLight({ 10,2,-10 }, { 0, 1, 1 });
	AddPointLight({ 0,2,-10 }, { 0, 1, 1 });
	AddPointLight({ -10,2,-10 }, { 1, 0, 1 });
	AddPointLight({ -10,2,0 }, { 0, 1, 0 });
	AddPointLight({ 0,0,0 }, { 1, 1, 1});
	 */
	// pls do not add spotlights thanks you ^^
	//AddSpotLight({ 0, 2, 0 }, vec3(vec3(0) - vec3(0, 2, 0)), {1, 1, 1}, 12.5);

}

void Scene::LightToShader(bool lightsOut)
{
	m_nrOfLights = m_lights.size();
	m_nrOfCarLights = m_carLights.size();

	vector<Light*> temp;
	for (int i = 0; i < m_nrOfLights; i++)
	{
		temp.push_back(m_lights.at(i));
	}
	for (int i = 0; i < m_nrOfCarLights; i++)
	{
		temp.push_back(m_carLights.at(i));
	}

	int nrOf = temp.size();
	m_modelShader->Uniform("u_NrOf", nrOf);
	m_modelShader->Uniform("u_LightsOut", lightsOut);


	for (uint i = 0; i < temp.size(); i++)
	{
		string _nr = to_string(i);
		switch (temp[i]->GetType())
		{
		case 0:
			m_modelShader->Uniform("u_Lights[" + _nr + "].type", temp[i]->GetType());
			m_modelShader->Uniform("u_Lights[" + _nr + "].dir", temp[i]->GetDirection());
			m_modelShader->Uniform("u_Lights[" + _nr + "].color", temp[i]->GetColor());
			break;
		case 1:
			m_modelShader->Uniform("u_Lights[" + _nr + "].type", temp[i]->GetType());
			m_modelShader->Uniform("u_Lights[" + _nr + "].pos", temp[i]->GetPos());
			m_modelShader->Uniform("u_Lights[" + _nr + "].color", temp[i]->GetColor());
			break;
		case 2:
			m_modelShader->Uniform("u_Lights[" + _nr + "].type", temp[i]->GetType());
			m_modelShader->Uniform("u_Lights[" + _nr + "].pos", temp[i]->GetPos());
			m_modelShader->Uniform("u_Lights[" + _nr + "].dir", temp[i]->GetDirection());
			m_modelShader->Uniform("u_Lights[" + _nr + "].color", temp[i]->GetColor());
			m_modelShader->Uniform("u_Lights[" + _nr + "].cutOff", temp[i]->GetCutOff());
			break;
		default:
			break;
		}

		m_modelShader->Uniform("u_Lights[" + _nr + "].ambient", temp[i]->GetAmbient());
		m_modelShader->Uniform("u_Lights[" + _nr + "].diffuse", temp[i]->GetDiffuse());
		m_modelShader->Uniform("u_Lights[" + _nr + "].specular", temp[i]->GetSpecular());
	}
}

void Scene::Render(vector<ObjectInfo*> objects, btDiscreteDynamicsWorld* world, MarchingCubes* cube, bool gameOver, int winner, bool lightsOut, bool terrain)
{
	m_viewMatrix = m_camera->GetView();
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, m_window->GetWidht(), m_window->GetHeight());

	// Render shadows
	RenderShadows(objects);

	glBindFramebuffer(GL_FRAMEBUFFER, m_bloom->getFBO());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render sky dome and clouds
	RenderSky();
	
	
	// Matrix uniforms
	m_modelShader->UseShader();
	m_modelShader->SetUniform("u_ViewPos", m_camera->GetPos());
	m_modelShader->SetUniform("u_View", m_viewMatrix);
	m_modelShader->SetUniform("u_Projection", m_projMatrix);
	m_modelShader->SetUniform("u_LSP", m_shadowMap->GetLSP());
	m_modelShader->SetTexture2D(0, "u_ShadowMap", m_shadowMap->GetTexture());

	// Texture(shadowmap)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap->GetTexture());

	// Terrain
	if (terrain == true)
	cube->Draw(m_modelShader);

	// Light uniforms
	LightToShader(lightsOut);
	
	// Draw all objects
	RenderSceneInfo(m_modelShader, objects);

	// Render Imgui
	RenderImGui(world);

	// Render Particles
	RenderParticles();
	
	// Render Skybox
	//RenderSkybox();

	// Add glow
	m_bloom->PingPongRender(3);

	m_bloom->RenderBloom(m_window->m_window);
}

void Scene::RenderSceneInfo(Shader* shader, vector<ObjectInfo*> objects)
{
	// Draw all objects
	shader->UseShader();
	for (uint i = 0; i < objects.size(); i++)
	{
		shader->SetUniform("u_Model", objects[i]->modelMatrix);
		shader->SetUniform("u_PlayerColor", objects[i]->hue);
		shader->SetUniform("u_Glow", objects[i]->glow);
		switch (objects[i]->typeId)
		{
		case 0:
			m_vehicles.at(objects[i]->modelId)->Draw(shader);
			break;

		case 1:
			m_platform.at(objects[i]->modelId)->Draw(shader);
			break;

		case 2:
			m_power.at(objects[i]->modelId)->Draw(shader);
			break;
		case 3:
			m_objects.at(objects[i]->modelId)->Draw(shader);
		default:
			break;
		}
	}
	shader->SetUniform("u_Model", glm::scale(translate(mat4(1.f), vec3(0, 9.65, 30)),vec3(0.25, 0.25, 0.25)));
	shader->SetUniform("u_PlayerColor", vec3(1, 0, 0));
	shader->SetUniform("u_Glow", false);
	m_winnerIsland->Draw(shader);
}

void Scene::RenderSkybox()
{
	m_skyboxShader->UseShader();
	m_skyboxShader->SetUniform("u_View", mat4(mat3(m_camera->GetView())));
	m_skyboxShader->SetUniform("u_Projection", m_projMatrix);
	m_skybox->DrawSkybox(m_skyboxShader);
}

void Scene::RenderSky()
{
	glCullFace(GL_FRONT);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	m_sky->RenderSkyDome(m_skyDomeShader, translate(mat4(1.0f), m_camera->GetPos()), m_camera->GetView(), m_projMatrix);

	glCullFace(GL_BACK);

	m_sky->RenderSkyPlane(m_skyPlaneShader, scale(translate(mat4(1.0f), vec3(0, -120, -150)), vec3(200)), m_camera->GetView(), m_projMatrix);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

void Scene::RenderLights(vector<Light*> light)
{
	m_carLights = light;
}

void Scene::RenderShadows(vector<ObjectInfo*> objects)
{
	m_shadowMap->CalcLightSpaceMatrix(m_lights);

	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMap->GetFBO());
	glViewport(0, 0, m_shadowMapWidth, m_shadowMapHeight);
	glClear(GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);

	glCullFace(GL_FRONT);
	RenderSceneInfo(m_shadowMap->GetShader(), objects);
	glCullFace(GL_BACK);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_screenWidth, m_screenHeight);
}

void Scene::RenderImGui(btDiscreteDynamicsWorld* world)
{
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

	if (m_debug) {
		m_modelShader->SetUniform("u_Model", mat4(1));
		m_modelShader->SetUniform("u_PlayerColor", vec3(1, 0, 0));
		world->debugDrawWorld();

	}
}

void Scene::RenderParticles()
{
	m_viewMatrix = m_camera->GetView();
	for (int i = 0; i < m_particles.size(); i++)
	{
		m_particles[i]->GetShader()->UseShader();
		m_particles[i]->GetShader()->SetUniform("u_View", m_viewMatrix);
		m_particles[i]->GetShader()->SetUniform("u_Proj", m_projMatrix);
		m_particles[i]->Draw();
	}
}

void Scene::UpdateParticles(float dt)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		m_particles[i]->Simulate(dt);
	}

	int k = 0;
	for (int i = 0; i < m_particles.size(); i++)
	{
		if (!m_particles[k]->GetActive())
		{
			delete m_particles[k];
			m_particles.erase(m_particles.begin() + k);
		}
		else
		{
			k++;
		}
	}
}

void Scene::UpdateCamera(float dt)
{
	if (dt < 1)
		m_camera->UpdateMovement(dt);
}

void Scene::UpdateSky(float dt)
{
	m_sky->Update(dt);
}

void Scene::SwapBuffer()
{
	glfwSwapBuffers(m_window->m_window);
}

void Scene::ZoomIn(float dt)
{
	if (m_fov > 40)
	{
		m_fov -= dt * (m_fov - 40) * 0.1f;
		m_projMatrix = perspective(radians(m_fov), (float)m_window->GetWidht() / (float)m_window->GetHeight(), 0.1f, 500.0f);
	}
}

void Scene::ZoomOut(float dt)
{
	if (m_fov < 60)
	{
		m_fov += dt * (60 - m_fov) * 0.1f;
		m_projMatrix = perspective(radians(m_fov), (float)m_window->GetWidht() / (float)m_window->GetHeight(), 0.1f, 500.0f);
	}
}

void Scene::SetCameraPos(vec3 pos)
{
	m_camera->ChangePos(pos);
}

void Scene::TranslateCameraPos(vec3 pos, float speed)
{
	m_camera->TranslatePos(pos, speed);
}

void Scene::SetCameraFocus(vec3 pos)
{
	//m_camera->SetFocusPoint(pos);
	m_camera->TranslateFocusPoint(pos);
}

void Scene::ShakeCamera(float intensity, float duration)
{
	m_camera->Shake(intensity, duration);
}

void Scene::AddParticleEffect(vec3 pos, vec3 color1, vec3 color2, float speed, float spread, vec3 dir, int nr, float duration, float size, float gravity)
{
	m_particles.push_back(new ParticleSystem(nr));
	m_particles.back()->SetActive();
	m_particles.back()->GenerateParticles(pos, speed, spread, duration, color1, color2, size, dir, gravity);
}


void Scene::SetWindowSize(int width, int height)
{
	m_window->SetWidht(width);
	m_window->SetHeight(height);
}

GLFWwindow* Scene::GetWindow()
{
	return m_window->m_window;
}

Window* Scene::GetOurWindow()
{
	return m_window;
}

int Scene::GetNumPlayerModels()
{
	return m_vehicles.size();
}

int Scene::GetNumPlatformModels()
{
	return m_platform.size();
}

int Scene::GetNumPowerUpModels()
{
	return m_power.size();
}

mat4 Scene::GetProjMatrix()
{
	return m_projMatrix;;
}

mat4 Scene::GetCameraView()
{
	return m_camera->GetView();
}

vector<Model*> Scene::GetModels(int index)
{
	if (index == 0)
		return m_platform;
	else if (index == 1)
		return m_vehicles;
	else if (index == 2)
		return m_power;
	else if (index == 3)
		return m_objects;
}

vector<Light*> Scene::GetLight()
{
	return m_lights;
}

void Scene::AddPointLight(vec3 pos, vec3 color)
{
	m_lights.push_back(new Light(1, pos, pos, color, 50));
}

void Scene::AddDirLight(vec3 dir, vec3 color)
{
	m_lights.push_back(new Light(0, dir, dir, color));
}

void Scene::AddSpotLight(vec3 pos, vec3 dir, vec3 color, float cutOff)
{
	m_lights.push_back(new Light(2, dir, pos, color, cutOff));
}