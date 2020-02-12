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
		stats = 4,
		restart = 5,
		playerHud = 6
	};
	int m_selected[4] = { 0, 0, 0, 0 };

	int m_p1ModelId = 0;
	double m_p1Seconds = 1;
	
	int m_p2ModelId = 0;
	double m_p2Seconds = 1;

	int m_p3ModelId = 0;
	double m_p3Seconds = 1;

	int m_p4ModelId = 0;
	double m_p4Seconds = 1;

	ActiveMenu m_menu = ActiveMenu::start;

public:

	Menu(Scene* scene, ObjectHandler* objHand);
	~Menu();

	void RenderMenu();
	void SetActiveMenu(ActiveMenu activeMenu);
	bool Pause();
	


};
