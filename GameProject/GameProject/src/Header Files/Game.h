#pragma once
#include "Include.h"
#include "ObjectHandler.h"
#include "Scene.h"
#include "Menu.h"

class Game
{
private:
	ObjectHandler* m_objectHandler;
	Scene* m_scene;
	Menu* m_menu;

	vector<ObjectInfo*> m_objects;
	vector<Light*> m_carLight;
	vector<Model*> m_platforms;
	vector<Model*> m_cars;
	ISoundEngine* m_soundEngine;
	ISound* m_music;
	vector<ISoundSource*> m_songs;
	MarchingCubes* m_cube;

	//Debug variables
	bool m_debug, m_toggle;
	int m_controllerID;
	int m_modelId;
	float m_pos[3];
	float m_time;
	float m_maxTime;
	float m_timeSinceSpawn;
	float m_timeSwapTrack;
	bool m_gameOver = false;
	bool m_fastMusic = false;
	std::atomic<bool> m_mapUpdateReady;
	std::atomic<bool> m_updateMap;

	int m_winner = 0;
	bool m_wasSelect = false; // used to see if we were in select-menu last time or not
	void Debug();
	void SelectionMenu();	// settup for select-menu
public:
	Game();
	~Game();
	void Update(float dt);
	void UpdateParticles(float dt);
	void DynamicCamera(float dt);
	void PlayWithLights(float dt);
	void Render(float dt);
	void Reset();
	GLFWwindow* GetWindow();
	void MutliThread(GLFWwindow* window);
};