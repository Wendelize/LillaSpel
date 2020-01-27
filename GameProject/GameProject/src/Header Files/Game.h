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
	float m_deadZone; //How much a controller "stick" can move before the game registers movement.
public:
	
};