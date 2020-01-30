#pragma once
#include "Include.h"

class Scene
{
private:
	Window * m_window;
	Shader * m_modelShader;
	Shader * m_skyboxShader;
	Camera * m_camera;
	Skybox * m_skybox;

	mat4 m_projMatrix,
		m_viewMatrix,
		m_modelMatrix;

	vector<Model*> m_vehicles, m_platform, m_power;
	//vector<Skybox*> m_skyBox;
	

public:
	Scene();
	~Scene();

	void Init();
	void UseShader(Shader shader);
	void Render(vector<ObjectInfo*> objects);
	void SetWindowSize(int width, int height);
	GLFWwindow* GetWindow();
};
