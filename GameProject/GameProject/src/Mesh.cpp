#include "Header Files/Include.h"

void Mesh::SetUpMesh()
{
	//Bind VertexArray
	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);

	//Bind VertexBuffer & IndexBuffer
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, VertexBufferSize(), &m_vertices[0], GL_STATIC_DRAW);


	glGenBuffers(1, &m_indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesBufferSize(), &m_indices[0], GL_STATIC_DRAW);

	//Write vertex data to memory
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(float) * 6));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(float) * 9));

	glBindVertexArray(0);
}

Mesh::Mesh(vector<VertexData> vertices, vector<unsigned int> indices, vector<TextureData> textures)
{
    m_vertices = vertices;
    m_indices = indices;
    m_textures = textures;

    SetUpMesh();
}

Mesh::~Mesh()
{
}

void Mesh::SetTexture(Shader shader)
{
	unsigned int _diffuseNr = 1;
	unsigned int _specularNr = 1;
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = m_textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(_diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(_specularNr++);

		shader.SetInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::Draw(Shader shader)
{
	//SetTexture(shader);
	glBindVertexArray(m_vertexArray);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
