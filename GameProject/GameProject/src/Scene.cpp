#include "Header Files/Scene.h"

Scene::Scene()
{
	m_window = new Window(1200, 840);
	m_modelShader = new Shader("src/Shaders/VertexShader.glsl", "src/Shaders/FragmentShader.glsl");
	m_skyboxShader = new Shader("src/Shaders/VertexSkyboxShader.glsl", "src/Shaders/FragmentSkyboxShader.glsl");
	m_camera = new Camera(*m_window->m_window, {0, 5, -10});
	m_skybox = new Skybox();

	m_modelMatrix = mat4(1.0);
	m_projMatrix = mat4(1.0);
	m_viewMatrix = mat4(1.0);
}

Scene::~Scene()
{
	for (int i = 0; i < m_vehicles.size(); i++)
	{
		delete m_vehicles.at(i);
	}
	m_vehicles.clear();

	for (int i = 0; i < m_platform.size(); i++)
	{
		delete m_platform.at(i);
	}
	m_platform.clear();

	for (int i = 0; i < m_power.size(); i++)
	{
		delete m_power.at(i);
	}
	m_power.clear();

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

	// Veichles
	m_vehicles.push_back(new Model("src/Models/Low-Poly-Racing-Car.obj"));
	m_vehicles.push_back(new Model("src/Models/ape.obj"));

	// Platforms
	m_platform.push_back(new Model("src/Models/Platform2.obj"));

	// Powers

	// Lights
	AddLights(DirLight, vec3(0, -1, 0), vec3(1));
	AddLights(SpotLight, vec3(0, 4, 0), vec3(0.6, 0, 0.3));
	AddLights(PointLight, vec3(5, 5, -3), vec3(0, 1, 0));
	AddLights(PointLight, vec3(-7, 3, -3), vec3(1, 0.0, 1));
}

void Scene::UseShader(Shader shader)
{

}

void Scene::LightToShader()
{
	m_modelShader->UseShader();
	m_modelShader->SetUniform("u_DirLight.dir", m_lights.m_dirLight.dir);
	m_modelShader->SetUniform("u_DirLight.color", m_lights.m_dirLight.color);
	m_modelShader->SetUniform("u_DirLight.ambient", m_lights.m_dirLight.ambient);
	m_modelShader->SetUniform("u_DirLight.diffuse", m_lights.m_dirLight.diffuse);
	m_modelShader->SetUniform("u_DirLight.specular", m_lights.m_dirLight.specular);

	m_modelShader->SetUniform("u_SpotLight.pos", m_lights.m_spotLights.at(0).pos);
	m_modelShader->SetUniform("u_SpotLight.color", m_lights.m_spotLights.at(0).color);
	m_modelShader->SetUniform("u_SpotLight.ambient", m_lights.m_spotLights.at(0).ambient);
	m_modelShader->SetUniform("u_SpotLight.diffuse", m_lights.m_spotLights.at(0).diffuse);
	m_modelShader->SetUniform("u_SpotLight.specular", m_lights.m_spotLights.at(0).specular);
	m_modelShader->SetUniform("u_SpotLight.cutOff", m_lights.m_spotLights.at(0).cutOff);
	m_modelShader->SetUniform("u_SpotLight.outerCutOff", cos(radians(30.0f)));



	for (int i = 0; i < m_lights.m_pointLights.size(); i++)
	{
		//	here to get correct numbering in the char-array 
		//	e.g. ""u_PointLight[0].pos" "u_PointLight[1].pos" etc..
		string _temp = "u_PointLight[" + to_string(i) + "].pos";
		const GLchar* _charPos = _temp.c_str();
		string _temp1 = "u_PointLight[" + to_string(i) + "].color";
		const GLchar* _charColor = _temp1.c_str();
		string _temp2 = "u_PointLight[" + to_string(i) + "].ambient";
		const GLchar* _charAmbient = _temp2.c_str();
		string _temp3 = "u_PointLight[" + to_string(i) + "].diffuse";
		const GLchar* _charDiffuse = _temp3.c_str();
		string _temp4 = "u_PointLight[" + to_string(i) + "].specular";
		const GLchar* _charSpecular = _temp4.c_str();

		m_modelShader->SetUniform(_charPos, m_lights.m_pointLights.at(i).pos);
		m_modelShader->SetUniform(_charColor, m_lights.m_pointLights.at(i).color);
		m_modelShader->SetUniform(_charAmbient, m_lights.m_pointLights.at(i).ambient);
		m_modelShader->SetUniform(_charDiffuse, m_lights.m_pointLights.at(i).diffuse);
		m_modelShader->SetUniform(_charSpecular, m_lights.m_pointLights.at(i).specular);
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
	m_modelShader->SetUniform("u_View", m_camera->GetView());
	m_modelShader->SetUniform("u_Projection", m_projMatrix);
	
	// Draw all objects
	for (uint i = 0; i < objects.size(); i++)
	{
		m_modelShader->SetUniform("u_Model", objects[i]->modelMatrix);
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

	/* Swap front and back buffers */
	glfwSwapBuffers(m_window->m_window);

	/* Poll for and process events */
	glfwPollEvents();
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

// a : a direction if u want a dirlight, else position
// b : color of light

void Scene::AddLights(LightType type, vec3 a, vec3 b)
{
	switch (type)
	{
	case Scene::DirLight:
		m_lights.AddDirLight(a, b);
		break;
	case Scene::PointLight:
		m_lights.AddPointLight(a, b);
		break;
	case Scene::SpotLight:
		m_lights.AddSpotLight(a, b);
		break;
	default:
		break;
	}
}


