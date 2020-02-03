#include "Header Files/Include.h"

Shader::Shader(const GLchar* VertexShaderFile, const GLchar* FragmentShaderFile)
{
	GLuint vs = LoadShader(GL_VERTEX_SHADER, VertexShaderFile);
	GLuint fs = LoadShader(GL_FRAGMENT_SHADER, FragmentShaderFile);

	m_shaderProgram = glCreateProgram();

	glAttachShader(m_shaderProgram, vs);
	glAttachShader(m_shaderProgram, fs);

	glLinkProgram(m_shaderProgram);

	glDetachShader(m_shaderProgram, vs);
	glDetachShader(m_shaderProgram, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);


	GLsizei  size = 0;
	GLchar test[1000];
	glGetProgramInfoLog(m_shaderProgram, 1000, &size, test);

	for (GLsizei i = 0; i < size; i++) {
		cout << test[i];
	}
}

Shader::Shader(const GLchar* VertexShaderFile, const GLchar* GeoShaderFile, const GLchar* FragmentShaderFile)
{
	GLuint vs = LoadShader(GL_VERTEX_SHADER, VertexShaderFile);
	GLuint gs = LoadShader(GL_GEOMETRY_SHADER, GeoShaderFile);
	GLuint fs = LoadShader(GL_FRAGMENT_SHADER, FragmentShaderFile);

	m_shaderProgram = glCreateProgram();

	glAttachShader(m_shaderProgram, vs);
	glAttachShader(m_shaderProgram, gs);
	glAttachShader(m_shaderProgram, fs);

	glLinkProgram(m_shaderProgram);

	glDetachShader(m_shaderProgram, vs);
	glDetachShader(m_shaderProgram, gs);
	glDetachShader(m_shaderProgram, fs);

	glDeleteShader(vs);
	glDeleteShader(gs);
	glDeleteShader(fs);


	GLsizei  sizei = 0;
	GLchar test[1000];
	glGetProgramInfoLog(m_shaderProgram, 1000, &sizei, test);

	for (GLsizei i = 0; i < sizei; i++) {
		cout << test[i];
	}
}

Shader::~Shader()
{
	glDeleteProgram(m_shaderProgram);
}

inline GLuint Shader::LoadShader(GLenum ShaderType, const GLchar* fileName)
{

	GLuint shader = glCreateShader(ShaderType);

	ifstream shaderFile(fileName);

	if (shaderFile.fail())
	{
		cout << "Not open" << endl;
	}

	string shaderText((istreambuf_iterator<char>(shaderFile)), istreambuf_iterator<char>());
	shaderFile.close();
	const char* shaderTextPtr = shaderText.c_str();
	glShaderSource(shader, 1, &shaderTextPtr, nullptr);
	glCompileShader(shader);

	return shader;
}

void Shader::UseShader()
{
	glUseProgram(0);
	glUseProgram(m_shaderProgram);
	m_activeProgram = m_shaderProgram;
}

GLuint Shader::GetShader() {
	return m_shaderProgram;
}

GLint Shader::GetUniform(const GLchar* name)
{
	if (m_activeProgram != m_shaderProgram)
		throw printf("cant get uniform, shader not active");

	GLint location = glGetUniformLocation(m_shaderProgram, name);

	if (location == -1)
		printf("cant find uniform '%s' in shader nr %d\n", name, m_shaderProgram);

	return location;
}

void Shader::SetUniform(const GLchar* name, const int& value)
{
	glUniform1i(GetUniform(name), value);
}

void Shader::SetUniform(const GLchar* name, const float& value)
{
	glUniform1f(GetUniform(name), value);
}

void Shader::SetUniform(const GLchar* name, const glm::vec2& vector)
{
	glUniform2fv(GetUniform(name), 1, value_ptr(vector));
}

void Shader::SetUniform(const GLchar* name, const glm::vec3& vector)
{
	glUniform3fv(GetUniform(name), 1, value_ptr(vector));
}

void Shader::SetUniform(const GLchar* name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniform(name), 1, GL_FALSE, value_ptr(matrix));
}

void Shader::SetTexture2D(int id, const GLchar* name, GLuint texture)
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, texture);
	SetUniform(name, id);
}

GLint Shader::GetUniform(const string& name)
{
	if (m_activeProgram != m_shaderProgram)
		throw printf("cant get uniform, shader not active");

	GLint location = glGetUniformLocation(m_shaderProgram, name.c_str());

	if (location == -1)
		printf("cant find uniform '%s' in shader nr %d\n", name, m_shaderProgram);

	return location;
}

void Shader::Uniform(const string& name, int value)
{
	glUniform1i(GetUniform(name), value);
}

void Shader::Uniform(const string& name, float value)
{
	glUniform1f(GetUniform(name), value);
}

void Shader::Uniform(const string& name, const vec3& value)
{
	glUniform3fv(GetUniform(name), 1, &value[0]);
}


