#pragma once

#include "Include.h"

class Scene
{
private:
	Window m_window;
	Shader m_shader;
	Camera m_camera;

public:
	Scene();
	~Scene();

	void Init();
	void UseShader(Shader shader);
	void Render(vector<Object*> objects);
};

Scene::Scene()
{
}

Scene::~Scene()
{
}

inline void Scene::Init()
{
}

inline void Scene::UseShader(Shader shader)
{

}

inline void Scene::Render(vector<Object*> objects)
{
}
