#include "Header Files/Scene.h"

Scene::Scene(const GLchar* VertexShaderFile, const GLchar* FragmentShaderFile)
{
	m_window = new Window(1200, 840);
	m_shader = new Shader(VertexShaderFile, FragmentShaderFile);
	m_camera = new Camera(m_window->m_window);
}

Scene::Scene(const GLchar* VertexShaderFile, const GLchar* GeoShaderFile, const GLchar* FragmentShaderFile)
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
}

void Scene::UseShader(Shader shader)
{

}

void Scene::Render(vector<Object*> objects)
{
}
