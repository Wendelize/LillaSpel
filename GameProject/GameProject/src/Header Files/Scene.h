#pragma once
#include "Include.h"
#include "MarchingCubes.h"


class Scene
{
private:

	Window* m_window;
	Shader *m_modelShader,
	       *m_skyboxShader,
	       *m_skyDomeShader,
	       *m_skyPlaneShader;
	Camera* m_camera;
	Skybox* m_skybox;
	Sky* m_sky;
	Bloom* m_bloom;
	vector<ParticleSystem*> m_particles;
	ShadowMap* m_shadowMap;

	bool m_debug = false;
	bool m_toggle = false;
	bool m_onlySky = false;
	bool m_enableBloom = true;

	mat4 m_projMatrix,
	     m_viewMatrix,
	     m_modelMatrix;

	vector<Model*> m_vehicles, m_platform, m_power, m_objects;

	Model* m_winnerIsland;

	vector <Light*> m_lights;
	vector <Light*> m_carLights;
	vector <Light*> m_objLights;
	int m_nrOfLights = 0, m_nrOfCarLights = 0, m_nrOfObjLights = 0;

	int m_screenWidth;
	int m_screenHeight;
	int m_shadowMapWidth;
	int m_shadowMapHeight;
	float m_bloomTextureScale;
	int m_bloomNrOfGaussianBlur;
	float m_fov;
	float m_terrainAlpha;
	bool m_lightsOut;


public:

	Scene();
	~Scene();

	void Init();
	void LightToShader();
	void Render(vector<ObjectInfo*> objects, btDiscreteDynamicsWorld* world, MarchingCubes* cube, bool gameOver,
	            int winner, bool terrain);
	void RenderSceneInfo(Shader* shader, vector<ObjectInfo*> objects);
	void RenderSkybox();
	void RenderSky();
	void RenderLights(vector<Light*> carlight, vector<Light*> objlight);
	void RenderShadows(vector<ObjectInfo*> objects);
	void RenderImGui(btDiscreteDynamicsWorld* world);
	void RenderParticles();
	void UpdateParticles(float dt);
	void UpdateCamera(float dt);
	void UpdateSky(float dt);
	void SwapBuffer();
	void ResetCameraFOV();
	void ZoomIn(float dt);
	void ZoomOut(float dt);

	void SetCameraPos(vec3 pos);
	void TranslateCameraPos(vec3 pos, float speed);
	void SetCameraFocus(vec3 pos);
	void SetInstantCameraFocus(vec3 pos);
	void ShakeCamera(float intensity, float duration);

	void SetBloom(bool b);

	void AddParticleEffect(vec3 pos, vec3 color1, vec3 color2, float speed, float spread, vec3 dir, int nr,
	                       float duration, float size, float gravity);

	void SetWindowSize(int width, int height);
	GLFWwindow* GetWindow();
	Window* GetOurWindow();
	int GetNumPlayerModels();
	int GetNumPlatformModels();
	int GetNumPowerUpModels();
	void SetOnlySky(bool b);
	mat4 GetProjMatrix();
	mat4 GetCameraView();
	vector<Model*> GetModels(int index);

	vector<Light*> GetLight();
	void AddPointLight(vec3 pos = {2, 2, 2}, vec3 color = {1, 0, 0});
	void AddDirLight(vec3 dir = {0, -1, 0}, vec3 color = {0, 1, 0});
	void AddSpotLight(vec3 pos = {0, 10, 0}, vec3 dir = {0, -1, 0}, vec3 color = {0, 0, 1}, float cutOff = 12.0);
	void UpdateTerrainAlpha(float dt, bool terrain);
	void UpdateLightsOut(bool lightsOut);
};
