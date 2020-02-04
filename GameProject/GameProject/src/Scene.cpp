#include "Header Files/Scene.h"

Scene::Scene()
{
	m_window = new Window(1500, 900);
	m_modelShader = new Shader("src/Shaders/VertexShader.glsl", "src/Shaders/FragmentShader.glsl");
	m_skyboxShader = new Shader("src/Shaders/VertexSkyboxShader.glsl", "src/Shaders/FragmentSkyboxShader.glsl");
	m_camera = new Camera({0, 12, -8});
	m_skybox = new Skybox();

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

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext(); Causing breakpoint -> Might cause memory leaks if this one is removed. -> doesn't look like it :D

	delete m_modelShader;
	delete m_skyboxShader;
	delete m_camera;
	delete m_skybox;
	delete m_window;
}

void Scene::Init()
{
	glEnable(GL_DEPTH_TEST);
	m_projMatrix = perspective(radians(45.0f), (float)m_window->GetWidht() / (float)m_window->GetHeight(), 0.1f, 100.0f);
	m_viewMatrix = m_camera->GetView();
	m_modelMatrix = mat4(1.0);

	/*ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();*/

	// Veichles
	m_vehicles.push_back(new Model("src/Models/Low-Poly-Racing-Car-Grey.obj"));
	m_vehicles.push_back(new Model("src/Models/Lowpoly-Snowcat.obj"));
	m_vehicles.push_back(new Model("src/Models/Cybertruck.obj"));
	m_vehicles.push_back(new Model("src/Models/ape.obj"));
	m_vehicles.push_back(new Model("src/Models/CAT.obj"));

	// Platforms
	m_platform.push_back(new Model("src/Models/Platform3.obj"));

	// Powers

	// Lights
	AddDirLight({ 0,-1,0 }, {1,1,1});
	AddPointLight({ 2,2,2 }, {0.6, 0, 0.9});
	AddPointLight({ -2,2,-2 }, {1, 0.8, 0});
	AddSpotLight({ 0, 5, -6 }, vec3(vec3(0) - vec3(0, 5, -5)), {0, 0, 1});

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

void Scene::Render(vector<ObjectInfo*> objects)
{
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.3f, 0.3f, 1.0f);

	m_modelShader->UseShader();
	// Matrix uniforms
	LightToShader();
	m_modelShader->SetUniform("u_ViewPos", m_camera->GetPos());
	m_modelShader->SetUniform("u_View", m_camera->GetView());
	m_modelShader->SetUniform("u_Projection", m_projMatrix);
	
	// Draw all objects
	for (uint i = 0; i < objects.size(); i++)
	{
		m_modelShader->SetUniform("u_Model", objects[i]->modelMatrix);
		m_modelShader->SetUniform("u_PlayerColor", objects[i]->hue);
		switch (objects[i]->typeId)
		{
		case 0:
			m_vehicles.at(objects[i]->modelId)->Draw(m_modelShader);
			break;

		case 1:
			m_platform.at(objects[i]->modelId)->Draw(m_modelShader);
			break;

		case 2: 
			m_power.at(objects[i]->modelId)->Draw(m_modelShader);
			break;

		default:
			break;
		}
	}

	m_skyboxShader->UseShader();
	m_skyboxShader->SetUniform("u_View", mat4(mat3(m_camera->GetView())));
	m_skyboxShader->SetUniform("u_Projection", m_projMatrix);
	m_skybox->DrawSkybox(m_skyboxShader);

	/* Poll for and process events */
	glfwPollEvents();

	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Debug");
		//ImGui::Text("FPS:	OVER 9000");
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->typeId == 0)
			{
				char* temp;
				//sprintf(temp, "%.0f", objects[i]->modelMatrix[0][3]);
				ImGui::Text("Player %i Position: x:%.2f , z:%.2F", i + 1, objects[i]->modelMatrix[3][0], objects[i]->modelMatrix[3][2]);
			}
		}
		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(m_window->m_window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

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
