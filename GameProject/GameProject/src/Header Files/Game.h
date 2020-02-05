#pragma once
#include "Include.h"
#include "ObjectHandler.h"
#include "Scene.h"

class Game
{
private:
	ObjectHandler* m_objectHandler;
	Scene* m_scene;
	float m_time;

	vector<ObjectInfo*> m_objects;

	//Debug variables
	float pos[3];
public:
	Game();
	~Game();
	void Update(float dt);
	void Render();
	GLFWwindow* GetWindow();
};