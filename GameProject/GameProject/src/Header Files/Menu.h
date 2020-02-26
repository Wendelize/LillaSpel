#pragma once
#include"Scene.h"
#include "ObjectHandler.h"
class Menu
{
public:
	Menu(Scene* scene, ObjectHandler* objHand);
	~Menu();

	enum struct ActiveMenu
	{
		noMenu = 0,
		start = 1,
		select = 2,
		selectLives = 3,
		pause = 4,
		stats = 5,
		restart = 6,
		playerHud = 7
	};

	void RenderMenu(bool gameOver, float time, Model* model);
	void SetActiveMenu(ActiveMenu activeMenu);
	bool SelectMenuActive();
	int GetMaxTime();
	void LoadMenuPic();
	bool Pause();
	bool Reset();
	void ResetReset();


private:
	Scene* m_scene;
	ObjectHandler* m_objHand;
	GLuint m_mainMenuPic;
	int m_menuPicWidth, m_menuPicHeight;
	bool m_reset = false;
	int m_maxLives = 3;
	int m_maxTime = 180.0f;

	int m_selected[4] = { 0, 0, 0, 0 };
	int m_continue = 0;

	int m_p1ModelId = 0;
	double m_p1Seconds = 1;
	vec3 m_p1Col = vec3(0.5, 1, 9);
	
	int m_p2ModelId = 0;
	double m_p2Seconds = 1;
	vec3 m_p2Col = vec3(0, 1, 0);
	
	int m_p3ModelId = -1;
	double m_p3Seconds = 1;
	bool m_p3Joined = false;
	vec3 m_p3Col = vec3(3, 0, 0);


	int m_p4ModelId = -1;
	double m_p4Seconds = 1;
	bool m_p4Joined = false;
	vec3 m_p4Col = vec3(1, 1, 0);
	
	ActiveMenu m_menu = ActiveMenu::start;



};
