#include "Header Files/Include.h"
#include "Header Files/Transform.h"

Window * WINDOW = new Window(1200, 840);
Shader * SHADER; 
Object * OBJECT;
Camera * CAMERA;
mat4 VIEWMATRIX, PROJMATRIX;
Transform * TRANS;
Model* MODEL;

void createStuff()
{
	glEnable(GL_DEPTH_TEST);

	CAMERA = new Camera(WINDOW->m_window, { 0,0, -10 });

	VIEWMATRIX = CAMERA->GetView();
	PROJMATRIX = perspective(radians(45.0f), (float)WINDOW->GetWidht() / (float)WINDOW->GetHeight(), 0.1f, 100.0f);
	TRANS = new Transform();

	OBJECT = new Object(2, { 0,0,0 });
	SHADER = new Shader("src/Shaders/VertexShader.glsl", "src/Shaders/FragmentShader.glsl");
	OBJECT->CreateCube();
	OBJECT->InitObject();

	MODEL = new Model("src/Models/Low-Poly-Racing-Car1.obj");

}

void render() 
{
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.3f, 0.3f, 1.0f);

	SHADER->UseShader();
	SHADER->SetUniform("u_View", CAMERA->GetView());
	SHADER->SetUniform("u_Projection", PROJMATRIX);
	//SHADER->SetUniform("u_Model", OBJECT->GetModelMat());
	SHADER->SetUniform("u_Model", TRANS->GetMatrix());
	//OBJECT->DrawObject(SHADER);
	MODEL->Draw(SHADER);

	/* Swap front and back buffers */
	glfwSwapBuffers(WINDOW->m_window);

	/* Poll for and process events */
	glfwPollEvents();
}

int main(void)
{
	float _deltaTime = 0.0, _curTime = 0.0, _lastTime = 0.0;
	createStuff();
	while (!glfwWindowShouldClose(WINDOW->m_window))
	{	
		_curTime = (float)glfwGetTime();
		_deltaTime = _curTime - _lastTime;
		_lastTime = _curTime;
		TRANS->SetScale(sin(_curTime)+1, sin(_curTime)+1, sin(_curTime)+1);
		TRANS->Rotate(_deltaTime * 4, _deltaTime, _deltaTime );
		CAMERA->UpdateMovement(_deltaTime, 10);
		render();
	}

	WINDOW->~Window();
	glfwTerminate();

	return 0;
}