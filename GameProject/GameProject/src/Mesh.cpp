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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), static_cast<void*>(nullptr));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));
	// BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, color));
	// BUFFER_OFFSET(sizeof(float) * 9));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));
	// BUFFER_OFFSET(sizeof(float) * 6));


	glBindVertexArray(0);
}

Mesh::Mesh(vector<VertexData> vertices, vector<unsigned int> indices, vector<TextureData> textures,
           vector<Material> materials)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;
	m_materials = materials;

	SetUpMesh();
}

Mesh::~Mesh()
{
	glBindVertexArray(0);
}

void Mesh::SetTexture(Shader* shader)
{
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		string number;
		string name = m_textures[i].type;
		if (name == "texture_diffuse")
			number = "u_Texture.diffuse";
		else if (name == "texture_specular")
			number = "u_Texture.specular"; // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		//const int value = i;
		glUniform1i(glGetUniformLocation(shader->GetShader(), name.c_str()), i);
		// TODO: kanske fixa s� shader har en setUniform f�r detta?
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetMaterial(Shader* shader)
{
	// 4 since material struct has 4 variables we want to get to shader
	shader->UseShader();
	for (unsigned int i = 0; i < 4; i++)
	{
		string name = "";
		if (i == 0)
		{
			name = "diffuse";
			shader->SetUniform(("u_Material." + name).c_str(), m_materials.back().Diffuse);
		}
		else if (i == 1)
		{
			name = "specular";
			shader->SetUniform(("u_Material." + name).c_str(), m_materials.back().Specular);
		}
		else if (i == 2)
		{
			name = "ambient";
			shader->SetUniform(("u_Material." + name).c_str(), m_materials.back().Ambient);
		}
		else
		{
			name = "shininess";
			shader->SetUniform(("u_Material." + name).c_str(), m_materials.back().Shininess);
		}
	}
}

void Mesh::Draw(Shader* shader)
{
	//SetTexture(shader);
	SetMaterial(shader);
	glBindVertexArray(m_vertexArray);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Mesh::UpdateMesh(vector<VertexData> vertices, vector<unsigned int> indices)
{
	m_vertices = vertices;
	m_indices = indices;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, VertexBufferSize(), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesBufferSize(), &m_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}
