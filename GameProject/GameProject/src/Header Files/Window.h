#pragma once

using namespace std;
using namespace glm;

class Window
{
private:
	int m_width, m_height;
	//Camera _camera;
public:
	GLFWwindow* m_window;

	Window(int width = 640, int height = 480);
	~Window();

	int GetWidht();
	int GetHeight();
};

Window::Window(int width, int height)
{
	m_width = width;
	m_height = height;

	if (!glfwInit())
		cout << "Could not Initialize the library" << endl;

	m_window = glfwCreateWindow(m_width, m_height, "EXTERMINATOR 2000-------------------------------I think diz is a game.", NULL, NULL);
	if (!m_window){
		glfwTerminate();
		cout << "Could not open window" << endl;
	}

	glfwMakeContextCurrent(m_window);
	if (glewInit() != GLEW_OK)
		printf("Error\n");
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

int Window::GetWidht()
{
	return m_width;
}

int Window::GetHeight()
{
	return m_height;
}
