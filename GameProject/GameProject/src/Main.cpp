//INCLUDES 3D
#include <GL/glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

//INCLUDES C++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

//CLASSES
#include "Header Files/Window.h"
#include "Header Files/Shader.h"
#include "Header Files/Camera.h"
#include "Header Files/Keys.h"
#include "Header Files/ObjectData.h"

//NAMESPACES
using namespace std;
using namespace glm;

Window * _gameWindow = new Window(1200, 840);
Shader * _shader; 
Object * _object;
Camera * _camera;
mat4 _viewMatrix, _projectionMatrix;

void createStuff()
{
	glEnable(GL_DEPTH_TEST);

	_camera = new Camera(_gameWindow->m_window, { 0,0, -10 });

	_viewMatrix = _camera->GetView();
	_projectionMatrix = perspective(radians(45.0f), (float)_gameWindow->GetWidht() / (float)_gameWindow->GetHeight(), 0.1f, 100.0f);

	_object = new Object(2, { 0,0,0 });
	_shader = new Shader("src/Shaders/VertexShader.glsl", "src/Shaders/FragmentShader.glsl");
	_object->CreateCube();
	_object->InitObject();

	double dArray[16] = { 0.0 };


}

void render() 
{
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.3f, 0.3f, 1.0f);


	_shader->UseShader();
	_shader->SetUniform("view", _camera->GetView());
	_shader->SetUniform("projection", _projectionMatrix);
	_shader->SetUniform("model", _object->GetModelMat());
	_object->DrawObject(_shader);

	/* Swap front and back buffers */
	glfwSwapBuffers(_gameWindow->m_window);

	/* Poll for and process events */
	glfwPollEvents();
}

int main(void)
{
	float _deltaTime = 0.0, _curTime = 0.0, _lastTime = 0.0;
	createStuff();
	while (!glfwWindowShouldClose(_gameWindow->m_window))
	{	
		_curTime = (float)glfwGetTime();
		_deltaTime = _curTime - _lastTime;
		_lastTime = _curTime;
		_camera->UpdateMovement(_deltaTime, 10);
		render();
	}

	_gameWindow->~Window();
	glfwTerminate();
	return 0;
}