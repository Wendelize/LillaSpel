#include "Header Files/Scene.h"

Scene::Scene()
{
	m_window = new Window(1200, 840);
	m_modelShader = new Shader("src/Shaders/VertexShader.glsl", "src/Shaders/FragmentShader.glsl");
	m_skyboxShader = new Shader("src/Shaders/VertexSkyboxShader.glsl", "src/Shaders/FragmentSkyboxShader.glsl");
	m_camera = new Camera(m_window->m_window, {0, 5, -50});
	m_skybox = new Skybox();

	m_modelMatrix = mat4(1.0);
	m_projMatrix = mat4(1.0);
	m_viewMatrix = mat4(1.0);
}
//
//Scene::Scene()
//{
//	m_window = new Window(1200, 840);
//	//m_shader = new Shader(VertexShaderFile, GeoShaderFile, FragmentShaderFile);
//	m_camera = new Camera(m_window->m_window);
//}

Scene::~Scene()
{
	delete m_window;
	delete m_modelShader;
	delete m_skyboxShader;
	delete m_camera;
	delete m_skybox;
}

void Scene::Init()
{
	glEnable(GL_DEPTH_TEST);
	m_projMatrix = perspective(radians(45.0f), (float)m_window->GetWidht() / (float)m_window->GetHeight(), 0.1f, 100.0f);
	m_viewMatrix = m_camera->GetView();
	m_modelMatrix = mat4(1.0);

	// Veichles
	m_vehicles.push_back(new Model("src/Models/Low-Poly-Racing-Car1.obj"));
	m_vehicles.push_back(new Model("src/Models/ape.obj"));

	// Platforms
	//m_platform.push_back(new Model("src/Models/Platform2.obj"));

	// Powers
}

void Scene::UseShader(Shader shader)
{

}

void Scene::Render(vector<ObjectInfo*> objects)
{
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.3f, 0.3f, 1.0f);

	m_modelShader->UseShader();
	// Matrix uniforms
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
