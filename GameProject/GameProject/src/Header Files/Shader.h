#pragma once
#include <GL/glew.h>

using namespace std;
using namespace glm;

class Shader
{
private:
	GLuint m_shaderProgram;
	GLuint m_activeProgram = GL_NONE;
public:
	Shader(const GLchar* VertexShaderFile, const GLchar* FragmentShaderFile);
	Shader(const GLchar* VertexShaderFile, const GLchar* GeoShaderFile, const GLchar* FragmentShaderFile);
	~Shader();

	GLuint LoadShader(GLenum ShaderType, const GLchar* fileName);
	void UseShader();
	GLuint GetShader();
	GLint GetUniform(const GLchar* name);
	void SetUniform(const GLchar* name, const int& value);
	void SetUniform(const GLchar* name, const float& value);
	void SetUniform(const GLchar* name, const vec2& vector);
	void SetUniform(const GLchar* name, const vec3& vector);
	void SetUniform(const GLchar* name, const mat4& matrix);
	void SetTexture2D(int id, const GLchar* name, GLuint texture);
	GLint GetUniform(const string& name);
	void Uniform(const string& name, int value);
	void Uniform(const string& name, float value);
	void Uniform(const string& name, const vec3& value);
};
