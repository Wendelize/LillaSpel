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
		selectLevel = 4,
		pause = 5,
		stats = 6,
		win = 7,
		restart = 8,
		playerHud = 9
	};

	void RenderMenu(bool gameOver, float time, Model* model);
	void SetActiveMenu(ActiveMenu activeMenu);
	void SetWinner(int playerNum); // old might not be needed anymore
	bool SelectMenuActive();
	bool RestartMenuActive();
	bool WinMenuActive();
	bool StatsMenuActive();
	int GetMaxTime();
	void LoadMenuPic();
	bool Pause();
	bool Reset();
	void ResetReset();
	void SetMapUpdate(bool map);
	ISoundEngine* m_soundEngine;
	vector<ISoundSource*> m_menuSounds;
	// for statsMenu 
	void CollisionTracking();
	void KillCount();
	void RankPlayers();
	int GetWinnerIndex();
	bool GetMapUpdate();

private:
	Scene* m_scene;
	ObjectHandler* m_objHand;
	GLuint m_mainMenuPic;
	int m_menuPicWidth, m_menuPicHeight;
	bool m_reset = false;
	bool m_updateMap = false;
	int m_maxLives = 3;
	int m_maxTime = 60.0f;
	int m_winnerID = 0;
	int m_mapID = 0;

	int m_lastCollied[4] = { -1, -1, -1, -1 }; 	// TODO: resetta det jag laggt till efter jag gjorde resett funktionen
	int m_lastCollisionTime[4] = { 0, 0, 0, 0 };
	int m_timesCollided[4] = { 0 };
	vector<vector<int>> m_kills;		// who you've killed
	vector<vector<int>> m_killers;	// who killed you
	vector<int> m_deathOrderID;		// ID for order players died in 
	vector<int> m_winOrder;			// ID for the final ranking
	int m_points[4] = { 0, 0, 0, 0 }; // points 2 per kill and 1 per life left
	vector<vec3> m_playerColor;

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

	double m_inputSeconds = 1;
	
	ActiveMenu m_menu = ActiveMenu::start;



};
