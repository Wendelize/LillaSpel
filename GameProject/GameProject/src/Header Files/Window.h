#pragma once


class Window
{
private:
	int m_width, m_height;
	//Camera _camera;
public:
	GLFWwindow *m_window;

	Window(int width = 640, int height = 480);
	~Window();

	int GetWidht();
	int GetHeight();
	void SetWidht(int width);
	void SetHeight(int height);
};
