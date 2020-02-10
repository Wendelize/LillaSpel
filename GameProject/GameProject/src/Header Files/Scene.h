#pragma once
#include "Include.h"

class Scene
{
private:

	Window * m_window;
	Shader* m_modelShader,
		* m_skyboxShader;
	Camera * m_camera;
	Skybox * m_skybox;
	ShadowMap * m_shadowMap;
	Bloom* m_bloom;

	bool m_debug = false;
	bool m_toggle = false;

	mat4 m_projMatrix,
		m_viewMatrix,
		m_modelMatrix;

	vector<Model*> m_vehicles, m_platform, m_power;
	
	vector <Light> m_lights;
	int m_nrOfLights = 0;

public:

	Scene();
	~Scene();

	void Init();
	void LightToShader();
	void Render(vector<ObjectInfo*> objects, btDiscreteDynamicsWorld* world);
	void SwapBuffer();
	void RetardRender(Shader * shader, vector<ObjectInfo*> objects);
	void REEEE( vector<ObjectInfo*> objects);
	void SetWindowSize(int width, int height);
	GLFWwindow* GetWindow();
	int GetNumPlayerModels();
	int GetNumPlatformModels();
	int GetNumPowerUpModels();
	vector<Model*> GetModels(int index);
	void AddPointLight(vec3 pos = { 2,2,2 }, vec3 color = { 1,0,0 });
	void AddDirLight(vec3 dir = { 0,-1,0 }, vec3 color = { 0,1,0 });
	void AddSpotLight(vec3 pos = { 0,10,0 }, vec3 dir = { 0,-1,0 }, vec3 color = {0,0,1}, float cutOff = 12.0);
};
