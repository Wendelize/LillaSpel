#pragma once
#include "Include.h"

class Scene
{
private:
	enum LightType : int {
		DirLight = 0,
		PointLight = 1,
		SpotLight = 2};

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
	
	Light m_lights;

public:
	Scene();
	~Scene();

	void Init();
	void UseShader(Shader shader);
	void LightToShader();
	void Render(vector<ObjectInfo*> objects, btDiscreteDynamicsWorld* world);
	void SetWindowSize(int width, int height);
	GLFWwindow* GetWindow();
	void AddLights(LightType type, vec3 a, vec3 b);
};
