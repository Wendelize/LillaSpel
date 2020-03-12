#pragma once
#include<vector>

class Window
{
private:
	int m_width, m_height;
public:
	GLFWwindow* m_window;
	std::vector<ImFont*> m_fonts;

	Window(int width = 1500, int height = 900);
	~Window();

	int GetWidht();
	int GetHeight();

	void SetWidht(int width);
	void SetHeight(int height);
};
