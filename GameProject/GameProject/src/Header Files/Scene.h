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
	ShadowMap * m_shadowMap;

	bool m_debug = true;

	mat4 m_projMatrix,
		m_viewMatrix,
		m_modelMatrix;

	vector<Model*> m_vehicles, m_platform, m_power;
	//vector<Skybox*> m_skyBox;
	
	//Light m_lights;
	vector <Light> m_lights;
	int m_nrOfLights = 0;

public:

	Scene();
	~Scene();

	void Init();
	void LightToShader();
	void Render(vector<ObjectInfo*> objects);
	void RetardRender(Shader * shader, vector<ObjectInfo*> objects);
	void SwapBuffer();
	void SetWindowSize(int width, int height);
	GLFWwindow* GetWindow();
	void AddPointLight(vec3 pos = { 2,2,2 }, vec3 color = { 1,0,0 });
	void AddDirLight(vec3 dir = { 0,-1,0 }, vec3 color = { 0,1,0 });
	void AddSpotLight(vec3 pos = { 0,10,0 }, vec3 dir = { 0,-1,0 }, vec3 color = {0,0,1}, float cutOff = 12.0);
};
