#include "Header Files/Include.h"
#include "Header Files/Transform.h"

Window * WINDOW = new Window(1200, 840);
Shader * SHADER; 
Object * OBJECT;
Camera * CAMERA;
mat4 VIEWMATRIX, PROJMATRIX;
Transform * TRANS;
Model* MODEL;
Level* LEVEL;
Shader* PLANESHADER;
Shader* SKYBOXSHADER;

void createStuff()
{
	glEnable(GL_DEPTH_TEST);

	CAMERA = new Camera(WINDOW->m_window, { 0, 10, -40 });

	VIEWMATRIX = CAMERA->GetView();
	PROJMATRIX = perspective(radians(45.0f), (float)WINDOW->GetWidht() / (float)WINDOW->GetHeight(), 0.1f, 100.0f);
	TRANS = new Transform();

	OBJECT = new Object(2, { 0,0,0 });
	SHADER = new Shader("src/Shaders/VertexShader.glsl", "src/Shaders/FragmentShader.glsl");
	OBJECT->CreateCube();
	OBJECT->InitObject();

	MODEL = new Model("src/Models/Platform2.obj");

	LEVEL = new Level(20);
	PLANESHADER = new Shader("src/Shaders/VertexPlaneShader.glsl", "src/Shaders/FragmentPlaneShader.glsl");
	SKYBOXSHADER = new Shader("src/Shaders/VertexSkyboxShader.glsl", "src/Shaders/FragmentSkyboxShader.glsl");
}

void render() 
{
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.3f, 0.3f, 1.0f);

	SHADER->UseShader();
	SHADER->SetUniform("u_View", CAMERA->GetView());
	SHADER->SetUniform("u_Projection", PROJMATRIX);
	SHADER->SetUniform("u_Model", TRANS->GetMatrix());

	MODEL->Draw(SHADER);

	PLANESHADER->UseShader();
	PLANESHADER->SetUniform("u_View", CAMERA->GetView());
	PLANESHADER->SetUniform("u_Projection", PROJMATRIX);
	PLANESHADER->SetUniform("u_Model", LEVEL->GetModelMat());

	SKYBOXSHADER->UseShader();
	SKYBOXSHADER->SetUniform("u_View", mat4(mat3(CAMERA->GetView())));
	SKYBOXSHADER->SetUniform("u_Projection", PROJMATRIX);

	LEVEL->DrawLevel(PLANESHADER, SKYBOXSHADER);

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
		// TRANS->SetScale(sin(_curTime)+1, sin(_curTime)+1, sin(_curTime)+1);
		// TRANS->Rotate(_deltaTime * 4, _deltaTime, _deltaTime );
		CAMERA->UpdateMovement(_deltaTime, 10);
		render();
	}

	WINDOW->~Window();
	glfwTerminate();

	return 0;
}