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
