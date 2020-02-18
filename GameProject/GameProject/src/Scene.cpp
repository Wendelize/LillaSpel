#include "Header Files/Scene.h"

Scene::Scene()
{
	m_screenWidth = 1920;
	m_screenHeight = 1080;
	m_shadowMapWidth = 1500;
	m_shadowMapHeight = 1500;
	m_bloomTextureScale = 0.4f;
	m_bloomNrOfGaussianBlur = 3;

	m_window = new Window(m_screenWidth, m_screenHeight);

	m_camera = new Camera({ 0, 16, 25 });
	m_modelShader = new Shader("src/Shaders/SceneVS.glsl", "src/Shaders/SceneFS.glsl");
	m_skyboxShader = new Shader("src/Shaders/SkyboxVS.glsl", "src/Shaders/SkyboxFS.glsl");
	m_skybox = new Skybox();
	m_shadowMap = new ShadowMap(m_shadowMapWidth, m_shadowMapHeight);
	m_bloom = new Bloom(m_screenWidth, m_screenHeight, m_bloomTextureScale);
	for(int i =0; i< 4 ; i++)
	m_particles.push_back(new ParticleSystem(50));

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

	for (uint i = 0; i < m_particles.size(); i++)
	{
		delete m_particles.at(i);
	}
	m_particles.clear();


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext(); Causing breakpoint -> Might cause memory leaks if this one is removed. -> doesn't look like it :D

	delete m_modelShader;
	delete m_skyboxShader;
	delete m_camera;
	delete m_shadowMap;
	delete m_skybox;
	delete m_bloom;

	
	delete m_window;

}

void Scene::Init()
{
	glEnable(GL_DEPTH_TEST);
	m_projMatrix = perspective(radians(60.0f), (float)m_window->GetWidht() / (float)m_window->GetHeight(), 0.1f, 100.0f);
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
	m_vehicles.push_back(new Model("src/Models/ape.obj"));
	m_vehicles.push_back(new Model("src/Models/CAT.obj"));

	// Platforms
	m_platform.push_back(new Model("src/Models/platform2.obj"));

	// Powers
	m_power.push_back(new Model("src/Models/PowerUp.obj"));
	m_power.push_back(new Model("src/Models/PowerUpCar.obj"));
	//m_power.push_back(new Model("src/Models/PowerUp.obj"));

	// Lights
	AddDirLight(vec3(-1, -1, 0), { 1,1,1 });
	AddPointLight({ 0,2,10 }, { 1, 0, 0 });
	AddPointLight({ 10,2,10 }, { 0, 1, 0 });
	AddPointLight({ 10,2,0 }, { 0, 0, 1 });
	AddPointLight({ 10,2,-10 }, { 0, 1, 1 });
	AddPointLight({ 0,2,-10 }, { 0, 1, 1 });
	AddPointLight({ -10,2,-10 }, { 1, 0, 1 });
	AddPointLight({ -10,2,0 }, { 0, 1, 0 });
	AddPointLight({ -10,2,10 }, { 1, 1, 0 });
	// pls do not add spotlights thanks you ^^
	//AddSpotLight({ 0, 2, 0 }, vec3(vec3(0) - vec3(0, 2, 0)), {1, 1, 1}, 12.5);

}

void Scene::LightToShader()
{
	m_modelShader->Uniform("u_NrOf", m_nrOfLights);
	m_nrOfLights = m_lights.size();

	for (int i = 0; i < m_lights.size(); i++)
	{
		string _nr = to_string(i);
		switch (m_lights[i]->GetType())
		{
		case 0:
			m_modelShader->Uniform("u_Lights[" + _nr + "].type", m_lights[i]->GetType());
			m_modelShader->Uniform("u_Lights[" + _nr + "].dir", m_lights[i]->GetDirection());
			m_modelShader->Uniform("u_Lights[" + _nr + "].color", m_lights[i]->GetColor());
			break;
		case 1:
			m_modelShader->Uniform("u_Lights[" + _nr + "].type", m_lights[i]->GetType());
			m_modelShader->Uniform("u_Lights[" + _nr + "].pos", m_lights[i]->GetPos());
			m_modelShader->Uniform("u_Lights[" + _nr + "].color", m_lights[i]->GetColor());
			break;
		case 2:
			m_modelShader->Uniform("u_Lights[" + _nr + "].type", m_lights[i]->GetType());
			m_modelShader->Uniform("u_Lights[" + _nr + "].pos", m_lights[i]->GetPos());
			m_modelShader->Uniform("u_Lights[" + _nr + "].dir", m_lights[i]->GetDirection());
			m_modelShader->Uniform("u_Lights[" + _nr + "].color", m_lights[i]->GetColor());
			break;
		default:
			break;
		}

		m_modelShader->Uniform("u_Lights[" + _nr + "].ambient", m_lights[i]->GetAmbient());
		m_modelShader->Uniform("u_Lights[" + _nr + "].diffuse", m_lights[i]->GetDiffuse());
		m_modelShader->Uniform("u_Lights[" + _nr + "].specular", m_lights[i]->GetSpecular());
		m_nrOfLights++;
	}
}

void Scene::Render(vector<ObjectInfo*> objects, btDiscreteDynamicsWorld* world, bool gameOver, int winner, float dt)
{
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, m_window->GetWidht(), m_window->GetHeight());

	// Render shadows
	RenderShadows(objects);

	glBindFramebuffer(GL_FRAMEBUFFER, m_bloom->getFBO());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	// Light uniforms
	LightToShader();

	// Draw all objects
	RenderSceneInfo(m_modelShader, objects);

	// Render Imgui
	RenderImGui(world);

	//// Render Particles
	RenderParticlesCollision(0.03, world);
	if (gameOver == true)
	{
		RenderParticlesVictory(objects[winner], dt);
	}
	// Render Skybox
	RenderSkybox();

	// Add glow
	m_bloom->PingPongRender(3);

	m_bloom->RenderBloom(m_window->m_window);

	/* Poll for and process events */
	//glfwPollEvents();
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

		default:
			break;
		}
	}
}

void Scene::RenderSkybox()
{
	m_skyboxShader->UseShader();
	m_skyboxShader->SetUniform("u_View", mat4(mat3(m_camera->GetView())));
	m_skyboxShader->SetUniform("u_Projection", m_projMatrix);
	m_skybox->DrawSkybox(m_skyboxShader);
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
		world->debugDrawWorld();

	}
}

void Scene::RenderParticlesCollision(float dt, btDiscreteDynamicsWorld* world)
{
	// Browse all collision pairs 
	int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; ++i)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

		btCollisionShape* shapeA = obA->getCollisionShape();
		btCollisionShape* shapeB = obB->getCollisionShape();

		//Collision between spheres(cars)
		if (shapeA->getShapeType() == 8 && shapeB->getShapeType() == 8)
		{
			bool A = false;
			bool B = false;
			vec3 particlePos;

			//Check for available particlesystem
			for (auto ps : m_particles)
			{
				//Car A
				if (ps->getActive() == false && A == false)
				{

					btTransform matA = obA->getWorldTransform();
					btVector3 vecA = matA.getOrigin();

					particlePos = vec3(vecA.x(), vecA.y(), vecA.z());

					btVector3 spread = obA->getInterpolationLinearVelocity();

					float fspread = (spread.x() + spread.y() + spread.z()) / 3.f;
					if (fspread < 1.f)
					{
						fspread = 6.f;
					}

					//Set of particlesystem for collider A
					ps->setActive();
					ps->GenerateParticlesForCollision(particlePos, fspread);
					A = true;	
				}
				//Car B
				if (ps->getActive() == false && B == false)
				{
					btTransform matB = obB->getWorldTransform();
					btVector3 vecB = matB.getOrigin();
					particlePos = vec3(vecB.x(), vecB.y(), vecB.z());

					btVector3 spread = obB->getInterpolationLinearVelocity();

					float fspread = (spread.x() + spread.y() + spread.z()) / 3.f;
					if (fspread < 1.f)
					{
						fspread = 6.f;
					}

					//Set of particlesystem for collider B
					ps->setActive();
					ps->GenerateParticlesForCollision(particlePos, fspread);
					B = true;
					//Both particlesystem are set of, no need to search futher
					break;
				}
			}
		}
	}
		
	// Render the active particlesystems
	for (auto ps : m_particles)
	{
		if (ps->getActive() == true)
		{
			mat4 temp = m_viewMatrix;
			ps->GetShader()->UseShader();
			ps->GetShader()->SetUniform("u_View", temp);
			ps->GetShader()->SetUniform("u_Proj", m_projMatrix);

			ps->Collision(dt);
			ps->Draw();
		}

	}
}

void Scene::RenderParticlesVictory(ObjectInfo* object, float dt)
{
	mat4 temp = m_viewMatrix;
	m_particles.back()->GetShader()->UseShader();
	m_particles.back()->GetShader()->SetUniform("u_View", temp);
	m_particles.back()->GetShader()->SetUniform("u_Proj", m_projMatrix);
	
	for (auto ps : m_particles)
	{
		if (ps->getActive() == false)
		{
			ps->setActive();
			ps->GenerateParticles(dt, vec3(object->modelMatrix[3][0], object->modelMatrix[3][1], object->modelMatrix[3][2]), 5.0f);
		}
		//ps->GenerateParticlesForVictory(dt, vec3(object->modelMatrix[3][0], object->modelMatrix[3][1], object->modelMatrix[3][2]));
		ps->Victory(dt, vec3(object->modelMatrix[3][0], object->modelMatrix[3][1], object->modelMatrix[3][2]));
		ps->Draw();

	}
}

void Scene::SwapBuffer()
{
	glfwSwapBuffers(m_window->m_window);
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

vector<Model*> Scene::GetModels(int index)
{
	if (index == 0)
		return m_platform;
	else if (index == 1)
		return m_vehicles;
	else if (index == 2)
		return m_power;
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