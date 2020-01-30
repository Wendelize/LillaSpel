#include "Header Files/Include.h"
#include "Header Files/Transform.h"
#include "Header Files/Game.h"
#include "Header Files/Scene.h"


Game* GAME;
Scene* SCENE;

int main(void)
{
	SCENE = new Scene();
	SCENE->Init();
	float _deltaTime = 0.0, _curTime = 0.0, _lastTime = 0.0;

	vector<ObjectInfo*> temp;
	
	temp.push_back(new ObjectInfo(mat4(1.0), 0, 0));

	while (!glfwWindowShouldClose(SCENE->GetWindow()))
	{	
		SCENE->Render(temp);
		_curTime = (float)glfwGetTime();
		_deltaTime = _curTime - _lastTime;
		_lastTime = _curTime;
	}

	glfwTerminate();

	return 0;
}