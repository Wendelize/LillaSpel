#include "Header Files/Include.h"
#include "Header Files/Transform.h"
#include "Header Files/Game.h"
#include "Header Files/Scene.h"
#include "Header Files/Transform.h"


Game* GAME;
Transform* tran;
//Scene* SCENE;

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	GAME = new Game();
	delete tran;

	float _deltaTime = 0.0, _curTime = 0.0, _lastTime = 0.0;
	GLFWwindow* wndw = GAME->GetWindow();
	while (!glfwWindowShouldClose(wndw))
	{	
		_curTime = (float)glfwGetTime();
		_deltaTime = _curTime - _lastTime;
		_lastTime = _curTime;
		GAME->Update(_deltaTime);
		GAME->Render();
	}

	glfwTerminate();

	//delete wndw;
	delete GAME;

	_CrtDumpMemoryLeaks();

	return 0;
}
