#include "Header Files/Include.h"

Window::Window(int width, int height)
{
	m_width = width;
	m_height = height;

	if (!glfwInit())
		cout << "Could not Initialize the library" << endl;

	m_window = glfwCreateWindow(m_width, m_height, "EXTERMINATOR 2000-------------------------------I think diz is a game.", NULL, NULL);
	if (!m_window) {
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

void Window::SetWidht(int width)
{
	m_width = width;
}

void Window::SetHeight(int height)
{
	m_height = height;
}
