#pragma once
#include "Include.h"
#include "ObjectHandler.h"
#include "Scene.h"

class Game
{
private:
	ObjectHandler* m_objectHandler;
	Scene* m_scene;

	vector<ObjectInfo*> m_objects;
	vector<Model*> m_platforms;
	vector<Model*> m_cars;
	//Debug variables
	bool m_debug, m_toggle;
	int m_controllerID;
	int m_modelId;
	float m_pos[3];
	float m_time;
	float m_timeSinceSpawn;
	void Debug();
public:
	Game();
	~Game();
	void Update(float dt);
	void PlayWithLights(float dt);
	void Render();
	GLFWwindow* GetWindow();
};