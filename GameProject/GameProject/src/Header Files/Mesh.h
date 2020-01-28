#pragma once

struct VertexData {
	vec3 pos;
	vec3 normal;
	vec3 color;
	vec2 uv;
};

struct TextureData {
	unsigned int id;
	string type;
};

class Mesh
{
private:
	GLuint m_vertexBuffer, m_indicesBuffer, m_vertexArray;

	void SetUpMesh();
	GLsizeiptr VertexBufferSize() const{
		return m_vertices.size() * sizeof(VertexData);
	}
	GLsizeiptr IndicesBufferSize() const {
		return m_indices.size() * sizeof(unsigned int);
	}

public:
	vector<VertexData> m_vertices;
	vector<unsigned int> m_indices;
	vector<TextureData> m_textures;

	Mesh(vector<VertexData> vertices, vector<unsigned int> indices, vector<TextureData> textures);
	~Mesh();

	void SetTexture(Shader shader);
	void Draw(Shader shader);


};
