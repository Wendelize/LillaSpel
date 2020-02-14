#include "Header Files/Scene.h"
#include <Map>
struct CollisionInfo
{
	btCollisionObject* object;
	btVector3 ptA;
	btVector3 ptB;
	btVector3 normalOnB;

	CollisionInfo(btCollisionObject *object = NULL, btVector3 ptA = btVector3(NULL, NULL, NULL), btVector3 ptB = btVector3(NULL, NULL, NULL), btVector3 normalOnB = btVector3(NULL,NULL,NULL))
	{
		this->object = object;
		this->ptA = ptA;
		this->ptB = ptB;
		this->normalOnB = normalOnB;
	}
};


Scene::Scene()
{
	m_window = new Window(1920, 1080);
	m_modelShader = new Shader("src/Shaders/SceneVS.glsl", "src/Shaders/SceneFS.glsl");
	m_skyboxShader = new Shader("src/Shaders/SkyboxVS.glsl", "src/Shaders/SkyboxFS.glsl");
	
	m_camera = new Camera({0, 10, 50});
	m_skybox = new Skybox();
	m_shadowMap = new ShadowMap();
	m_bloom = new Bloom();
	m_particles = new ParticleSystem(100);

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
	m_lights.clear();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext(); Causing breakpoint -> Might cause memory leaks if this one is removed. -> doesn't look like it :D

	delete m_modelShader;
	delete m_skyboxShader;
	delete m_camera;
	delete m_shadowMap;
	delete m_skybox;
	delete m_bloom;
	delete m_particles;

	delete m_window;

}

void Scene::Init()
{
	glEnable(GL_DEPTH_TEST);
	m_projMatrix = perspective(radians(45.0f), (float)m_window->GetWidht() / (float)m_window->GetHeight(), 0.1f, 100.0f);
	m_viewMatrix = m_camera->GetView();
	m_modelMatrix = mat4(1.0);


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


	// Lights
	AddDirLight(vec3(-1, -1, 0), { 1,1,1 });
	AddPointLight({ 2,2,2 }, {1, 1, 1});
	AddPointLight({ -2,2,-2 }, {1, 1, 1});
}

void Scene::LightToShader()
{
	m_modelShader->Uniform("u_NrOf", m_nrOfLights);

	for (uint i = m_nrOfLights; i < m_lights.size(); i++)
	{
		string _nr = to_string(i);
		switch (m_lights.at(i).GetType())
		{
		case 0:
			m_modelShader->Uniform("u_Lights[" + _nr + "].type", m_lights.at(i).GetType());
			m_modelShader->Uniform("u_Lights[" + _nr + "].dir", m_lights.at(i).GetDirection());
			m_modelShader->Uniform("u_Lights[" + _nr + "].color", m_lights.at(i).GetColor());
			break;
		case 1:
			m_modelShader->Uniform("u_Lights[" + _nr + "].type", m_lights.at(i).GetType());
			m_modelShader->Uniform("u_Lights[" + _nr + "].pos", m_lights.at(i).GetPos());
			m_modelShader->Uniform("u_Lights[" + _nr + "].color", m_lights.at(i).GetColor());
			break;
		case 2:
			m_modelShader->Uniform("u_Lights[" + _nr + "].type", m_lights.at(i).GetType());
			m_modelShader->Uniform("u_Lights[" + _nr + "].pos", m_lights.at(i).GetPos());
			m_modelShader->Uniform("u_Lights[" + _nr + "].dir", m_lights.at(i).GetDirection());
			m_modelShader->Uniform("u_Lights[" + _nr + "].color", m_lights.at(i).GetColor());
			break;
		default:
			break;
		}

		m_modelShader->Uniform("u_Lights[" + _nr + "].ambient", m_lights.at(i).GetAmbient());
		m_modelShader->Uniform("u_Lights[" + _nr + "].diffuse", m_lights.at(i).GetDiffuse());
		m_modelShader->Uniform("u_Lights[" + _nr + "].specular", m_lights.at(i).GetSpecular());
		m_nrOfLights += 1;
	}


}

void Scene::Render(vector<ObjectInfo*> objects, btDiscreteDynamicsWorld* world)
{
	/* Render here */
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
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


		std::map<btCollisionObject*, CollisionInfo> newContacts;

		/* Browse all collision pairs */
		int numManifolds = world->getDispatcher()->getNumManifolds();
		if (numManifolds > 3)
		{
			//RenderParticles(0.03, objects[0]);
		}

		//for (int i = 0; i < numManifolds; i++)
		//{
		//	btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		//	btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
		//	btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

		//	/* Check all contacts points */
		//	int numContacts = contactManifold->getNumContacts();
		//	cout <<"Number of contacts for manifol#"<<i << ": " <<  numContacts << endl;

		//	if (numContacts > 3)
		//	{
		//		if(objects[i]->typeId == 0)
		//		RenderParticles(0.03, objects[i]);

		//	}
			//for (int j = 0; j < numContacts; j++)
			//{
			//	btManifoldPoint& pt = contactManifold->getContactPoint(j);
			//	
			//	if (pt.getDistance() < 0.f)
			//	{
			//		//std::cout << "Contact found" << std::endl;

			//		const btVector3& ptA = pt.getPositionWorldOnA();
			//		const btVector3& ptB = pt.getPositionWorldOnB();
			//		const btVector3& normalOnB = pt.m_normalWorldOnB;

			//		if (newContacts.find(obB) == newContacts.end())
			//		{
			//			RenderParticles(0.03, objects[i]);
			//			newContacts[obB] = CollisionInfo(obA, ptA, ptB, normalOnB);
			//		}
			//	}
			//}
		//}

		//vector<CollisionInfo>::iterator m_contacts;
		///* Check for added contacts ... */
		//map<btCollisionObject*, CollisionInfo>::iterator it;
		//if (!newContacts.empty())
		//{
		//	for (it = newContacts.begin(); it != newContacts.end(); it++)
		//	{
		//		if (m_contacts.find((*it).first) == m_contacts.end())
		//		{
		//			std::cout << "Collision detected" << std::endl;
		//			// TODO: signal
		//		}
		//		else
		//		{
		//			// Remove to filter no more active contacts
		//			m_contacts.erase((*it).first);
		//		}
		//	}
		//}

		///* ... and removed contacts */
		//if (!m_contacts.empty())
		//{
		//	for (it = m_contacts.begin(); it != m_contacts.end(); it++)
		//	{
		//		std::cout << "End of collision detected" << std::endl;
		//		// TODO: signal
		//	}
		//	m_contacts.clear();
		//}

		//m_contacts = newContacts;
	
	//for (size_t i = 0; i < world->getNumCollisionObjects(); i++)
	//{
	//
	//	if (objects[i]->typeId == 0) // Is veichle
	//	{
	//		
	//		btCollisionObject* car = world->getCollisionObjectArray()[i];
	//		//RenderParticles(0.03, objects[i]);

	//		for (int j = 0; j < world->getNumCollisionObjects(); j++)
	//		{
	//			if (objects[j]->typeId == 1 )
	//			{
	//				btCollisionObject* obj = world->getCollisionObjectArray()[j];
	//				if ( car != obj)
	//				{
	//					world->performDiscreteCollisionDetection();


	//					
	//					RenderParticles(0.03, objects[i]);
	//				}
	//			}

	//		}
	//			
	//		
	//	}
	//}
	// Render Skybox
	RenderSkybox();


	// Add glow
	m_bloom->PingPongRender();

	m_bloom->RenderBloom();



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
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);

	glCullFace(GL_FRONT);
	RenderSceneInfo(m_shadowMap->GetShader(), objects);
	glCullFace(GL_BACK);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_window->GetWidht(), m_window->GetHeight());
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

void Scene::RenderParticles(float dt, ObjectInfo* object)
{
	mat4 temp = m_viewMatrix;
	m_particles->GetShader()->UseShader();
	m_particles->GetShader()->SetUniform("u_View", temp);
	m_particles->GetShader()->SetUniform("u_Proj", m_projMatrix);

	//m_particles->GenerateParticles(dt);
	m_particles->SimulateParticles(dt, vec3(object->modelMatrix[3][0], object->modelMatrix[3][1], object->modelMatrix[3][2]));
	m_particles->Draw();
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

vector<Light> Scene::GetLight()
{
	return m_lights;
}

void Scene::AddPointLight(vec3 pos, vec3 color)
{
	Light _temp = Light(1, pos, pos, color);
	m_lights.push_back(_temp);
}

void Scene::AddDirLight(vec3 dir, vec3 color)
{
	Light _temp = Light(0, dir, dir, color);
	m_lights.push_back(_temp);
}

void Scene::AddSpotLight(vec3 pos, vec3 dir, vec3 color, float cutOff)
{
	Light _temp = Light(2, dir, pos, color, cutOff);
	m_lights.push_back(_temp);
}