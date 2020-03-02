#pragma once
#include "Include.h"
#include "MarchingCubes.h"


class Scene
{
private:

	Window * m_window;
	Shader * m_modelShader,
		   * m_skyboxShader,
		   * m_skyDomeShader,
		   * m_skyPlaneShader;
	Camera * m_camera;
	Skybox * m_skybox;
	Sky	   * m_sky;
	Bloom  * m_bloom;
	vector<ParticleSystem*> m_particles;
	ShadowMap * m_shadowMap;

	bool m_debug = false;
	bool m_toggle = false;

	mat4 m_projMatrix,
		m_viewMatrix,
		m_modelMatrix;

	vector<Model*> m_vehicles, m_platform, m_power;
	
	vector <Light*> m_lights;
	vector <Light*> m_carLights;
	int m_nrOfLights = 0, m_nrOfCarLights = 0;

	int m_screenWidth;
	int m_screenHeight;
	int m_shadowMapWidth;
	int m_shadowMapHeight;
	float m_bloomTextureScale;
	int m_bloomNrOfGaussianBlur;
	float m_fov;

public:

	Scene();
	~Scene();

	void Init();
	void LightToShader(bool lightsOut);
	//void Render(vector<ObjectInfo*> objects, btDiscreteDynamicsWorld* world, MarchingCubes* cube);
	void Render(vector<ObjectInfo*> objects, btDiscreteDynamicsWorld* world, MarchingCubes* cube, bool gameOver, int winner, float dt, bool lightsOut);
	void RenderSceneInfo(Shader * shader, vector<ObjectInfo*> objects);
	void RenderSkybox();
	void RenderSky();
	void RenderLights(vector<Light*> light);
	void RenderShadows(vector<ObjectInfo*> objects);
	void RenderImGui(btDiscreteDynamicsWorld* world);
	void RenderParticles(float dt);
	void SwapBuffer();
	void ZoomIn(float dt);
	void ZoomOut(float dt);

	void SetCameraPos(vec3 pos);
	void TranslateCameraPos(vec3 pos);
	void SetCameraFocus(vec3 pos);
	void ShakeCamera(float intensity, float duration);

	void AddParticleEffect(vec3 pos, vec3 color1, vec3 color2, float speed, float spread, vec3 dir, int nr, float duration, float size);

	void SetWindowSize(int width, int height);
	GLFWwindow* GetWindow();
	Window* GetOurWindow();
	int GetNumPlayerModels();
	int GetNumPlatformModels();
	int GetNumPowerUpModels();
	mat4 GetProjMatrix();
	mat4 GetCameraView();
	vector<Model*> GetModels(int index);

	vector<Light*> GetLight();
	void AddPointLight(vec3 pos = { 2,2,2 }, vec3 color = { 1,0,0 });
	void AddDirLight(vec3 dir = { 0,-1,0 }, vec3 color = { 0,1,0 });
	void AddSpotLight(vec3 pos = { 0,10,0 }, vec3 dir = { 0,-1,0 }, vec3 color = {0,0,1}, float cutOff = 12.0);
	//void CameraFollowCar(ObjectInfo* objects);
};
