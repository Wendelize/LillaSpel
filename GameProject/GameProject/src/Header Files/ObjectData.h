#pragma once
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

struct vertexData {
	vec3 pos;
	vec3 normal;
	vec3 color;
	vec2 uv;
};

class Object
{
private:
	vertexData * m_vertices;
	unsigned int * m_indices;
	GLuint m_nrOfV = 0;
	GLuint m_nrOfI = 0;
	float m_size;
	vec3 m_position;
	mat4 m_modelMatrix;

	GLsizeiptr vertexBufferSize()const {
		return (m_nrOfV * sizeof(vertexData));
	}

	GLsizeiptr indexBufferSize()const {
		return (m_nrOfI * sizeof(unsigned int));
	}

public:
	GLuint m_vertexBuffer, m_indicesBuffer, m_vertexArray;

	Object(float size, vec3 pos);
	~Object();

	void CreatePlane();
	void CreateCube();
	void LoadMesh();
	void InitObject();
	void DrawObject(Shader * shaderProgram);
	mat4 GetModelMat();
};
