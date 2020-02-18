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
	vector<Model*> m_platforms;
	vector<Model*> m_cars;
	ISoundEngine* m_soundEngine;
	ISound* m_music;
	vector<ISoundSource*> m_songs;

	//Debug variables
	bool m_debug, m_toggle;
	int m_controllerID;
	int m_modelId;
	float m_pos[3];
	float m_time;
	float m_maxTime;
	float m_timeSinceSpawn;
	bool m_gameOver = false;
	bool m_fastMusic = false;
	bool m_wasSelect = false; // used to see if we were in select-menu last time or not
	void Debug();
	void SelectionMenu();	// settup for select-menu
public:
	Game();
	~Game();
	void Update(float dt);
	void Render();
	void Reset();
	GLFWwindow* GetWindow();
	
};