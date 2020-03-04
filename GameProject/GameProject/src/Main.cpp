#include "Header Files/Include.h"
#include "Header Files/Transform.h"
#include "Header Files/Game.h"
#include "Header Files/Scene.h"
#define STB_IMAGE_IMPLEMENTATION

Game* GAME;


int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	srand(time(NULL));
	GAME = new Game();

	float _deltaTime = 0.0, _curTime = 0.0, _lastTime = 0.0;
	GLFWwindow* _window = GAME->GetWindow();

	std::thread t(&Game::MutliThread, GAME, _window);
	while (!glfwWindowShouldClose(_window))
	{	

		_curTime = (float)glfwGetTime();
		_deltaTime = _curTime - _lastTime;
		_lastTime = _curTime;
		GAME->Update(_deltaTime);
		GAME->Render();
	}

	t.join();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	
	delete GAME;

	exit(0); //Used to be called in window destructor -> caused memory leaks. This fixes it.

	//_CrtDumpMemoryLeaks();
	
	return 0;
}

