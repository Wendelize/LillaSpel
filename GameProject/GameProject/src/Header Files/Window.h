#pragma once

using namespace std;
using namespace glm;

class Window
{
private:
	int _width, _height;
	//Camera _camera;
public:
	GLFWwindow* _window;

	Window(int width = 640, int height = 480);
	~Window();

	int getWidht();
	int getHeight();
};

Window::Window(int width, int height)
{
	_width = width;
	_height = height;

	if (!glfwInit())
		cout << "Could not Initialize the library" << endl;

	_window = glfwCreateWindow(_width, _height, "EXTERMINATOR 2000-------------------------------I think diz is a game.", NULL, NULL);
	if (!_window){
		glfwTerminate();
		cout << "Could not open window" << endl;
	}

	glfwMakeContextCurrent(_window);
	if (glewInit() != GLEW_OK)
		printf("Error\n");
}

Window::~Window()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

int Window::getWidht()
{
	return _width;
}

int Window::getHeight()
{
	return _height;
}
