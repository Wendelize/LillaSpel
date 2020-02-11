#pragma once
#include"Scene.h"
#include "ObjectHandler.h"
class Menu
{
private:
	Scene* m_scene;
	ObjectHandler* m_objHand;
	enum struct ActiveMenu
	{
		noMenu = 0,
		start = 1,
		select = 2,
		pause = 3,
		restart = 4,
		playerHud = 5
	};


	ActiveMenu m_menu = ActiveMenu::start;

public:

	Menu(Scene* scene, ObjectHandler* objHand);
	~Menu();

	void RenderMenu();
	void SetActiveMenu(ActiveMenu activeMenu);
	bool Pause();
	


};
