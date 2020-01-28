#pragma once
#include "Include.h"

class Scene
{
private:
	Window * m_window;
	Shader * m_shader;
	Camera * m_camera;

public:
	Scene(const GLchar* VertexShaderFile, const GLchar* FragmentShaderFile);
	Scene(const GLchar* VertexShaderFile, const GLchar* GeoShaderFile, const GLchar* FragmentShaderFile);
	~Scene();

	void Init();
	void UseShader(Shader shader);
	void Render(vector<ObjectInfo*> objects);
	void SetWindowSize(int width, int height);
};
