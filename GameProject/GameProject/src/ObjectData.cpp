#include "Header Files/Include.h"

Object::Object(float size, vec3 pos)
{
	m_size = size;
	m_position = pos;
}

Object::~Object()
{
}

void Object::CreatePlane()
{
	//CREATE VERTICES DATA
	vertexData vertices[] =
	{
	 { {-m_size / 2,	0,  m_size / 2 }, { 0, 1, 0 }, {1,1,1}, {0, 0}},
	 { {-m_size / 2,	0, -m_size / 2 }, { 0, 1, 0 }, {1,1,1}, {0, 1}},
	 { { m_size / 2,	0, -m_size / 2 }, { 0, 1, 0 }, {1,1,1}, {1, 1}},
	 { { m_size / 2,	0,  m_size / 2 }, { 0, 1, 0 }, {1,1,1}, {1, 0}}
	};
	m_nrOfV = sizeof(vertices) / sizeof(*vertices);
	m_vertices = new vertexData[m_nrOfV];
	memcpy(m_vertices, vertices, sizeof(vertices));

	//CREATE INDECES DATA
	unsigned int indexData[] = { 0, 2, 1,  0, 3, 2 };
	m_nrOfI = sizeof(indexData) / sizeof(*indexData);
	m_indices = new unsigned int[m_nrOfI];
	memcpy(m_indices, indexData, sizeof(indexData));
}

void Object::CreateCube()
{
	//CREATE VERTICES DATA
	vertexData vertices[] =
	{
		//FRONT
		 { {-m_size / 2, -m_size / 2,  m_size / 2 },    { 0, 0, 1 },	{ 1, 1, 0 }, {0, 0} },
		 { {-m_size / 2,  m_size / 2,  m_size / 2 },	{ 0, 0, 1 },	{ 1, 1, 0 }, {0, 1} },
		 { { m_size / 2,  m_size / 2,  m_size / 2 },	{ 0, 0, 1 },	{ 1, 1, 0 }, {1, 1} },
		 { { m_size / 2, -m_size / 2,  m_size / 2 },	{ 0, 0, 1 },	{ 1, 1, 0 }, {1, 0} },
		 //BACK														  
		 { { m_size / 2, -m_size / 2, -m_size / 2 },    { 0, 0, -1 },	{ 1, 1, 1 }, {0, 0} },
		 { { m_size / 2,  m_size / 2, -m_size / 2 },	{ 0, 0, -1 },	{ 1, 1, 1 }, {0, 1} },
		 { {-m_size / 2,  m_size / 2, -m_size / 2 },	{ 0, 0, -1 },	{ 1, 1, 1 }, {1, 1} },
		 { {-m_size / 2, -m_size / 2, -m_size / 2 },	{ 0, 0, -1 },	{ 1, 1, 1 }, {1, 0} },
		 //TOP														  
		 { {-m_size / 2,  m_size / 2,  m_size / 2 },	{ 0, 1,  0 },	{ 1, 0, 1 }, {0, 0} },
		 { {-m_size / 2,  m_size / 2, -m_size / 2 },	{ 0, 1,  0 },	{ 1, 0, 1 }, {0, 1} },
		 { { m_size / 2,  m_size / 2, -m_size / 2 },	{ 0, 1,  0 },	{ 0, 0, 1 }, {1, 1} },
		 { { m_size / 2,  m_size / 2,  m_size / 2 },	{ 0, 1,  0 },	{ 1, 0, 1 }, {1, 0} },
		 //BOT														  
		 { {-m_size / 2,  -m_size / 2, -m_size / 2 },	{ 0, -1, 0 },	{ 1, 0, 0 }, {0, 0} },
		 { {-m_size / 2,  -m_size / 2,  m_size / 2 },  { 0, -1, 0 },	{ 1, 0, 0 }, {0, 1} },
		 { { m_size / 2,  -m_size / 2,  m_size / 2 },  { 0, -1, 0 },	{ 1, 0, 0 }, {1, 1} },
		 { { m_size / 2,  -m_size / 2, -m_size / 2 },  { 0, -1, 0 },	{ 1, 0, 0 }, {1, 0} },
		 //RIGHT													  
		 { { m_size / 2,  -m_size / 2,  m_size / 2 },	{ 1, 0, 0 },	{ 1, 1, 1 }, {0, 0} },
		 { { m_size / 2,   m_size / 2,  m_size / 2 },	{ 1, 0, 0 },	{ 1, 1, 1 }, {0, 1} },
		 { { m_size / 2,   m_size / 2, -m_size / 2 },	{ 1, 0, 0 },	{ 1, 1, 1 }, {1, 1} },
		 { { m_size / 2,  -m_size / 2, -m_size / 2 },	{ 1, 0, 0 },	{ 1, 1, 1 }, {1, 0} },
		 //LEFT                                  					 
		 { {-m_size / 2,  -m_size / 2, -m_size / 2 },	{ -1, 0, 0 },	{ 1, 1, 1 }, {0, 0} },
		 { {-m_size / 2,   m_size / 2, -m_size / 2 },	{ -1, 0, 0 },	{ 1, 1, 1 }, {0, 1} },
		 { {-m_size / 2,   m_size / 2,  m_size / 2 },	{ -1, 0, 0 },	{ 1, 1, 1 }, {1, 1} },
		 { {-m_size / 2,  -m_size / 2,  m_size / 2 },	{ -1, 0, 0 },	{ 1, 1, 1 }, {1, 0} }
	};

	m_nrOfV = sizeof(vertices) / sizeof(*vertices);
	m_vertices = new vertexData[m_nrOfV];
	memcpy(m_vertices, vertices, sizeof(vertices));

	//CREATE INDECES DATA
	unsigned int indexData[] = { 0,  2,   1,   0,   3,   2,
								  4,  6,   5,   4,   7,   6,
								  8,  10,  9,   8,   11,  10,
								 12,  14,  13,  12,  15,  14,
								 16,  18,  17,  16,  19,  18,
								 20,  22,  21,  20,  23,  22 };
	m_nrOfI = sizeof(indexData) / sizeof(*indexData);
	m_indices = new unsigned int[m_nrOfI];
	memcpy(m_indices, indexData, sizeof(indexData));

}

void Object::LoadMesh()
{
}

void Object::DrawObject(Shader* shaderProgram)
{
	glBindVertexArray(m_vertexArray);
	//shaderProgram->setUniform("model", _modelMatrix);
	glDrawElements(GL_TRIANGLES, m_nrOfI, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Object::InitObject()
{
	//Bind VertexArray
	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);

	//Bind VertexBuffer & IndexBuffer
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize(), m_vertices, GL_STATIC_DRAW);


	glGenBuffers(1, &m_indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize(), m_indices, GL_STATIC_DRAW);

	//Write vertex data to memory
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(float) * 6));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(float) * 9));



}

mat4 Object::GetModelMat()
{
	//_modelMatrix = translate(mat4(1.0f), _position);
	//_modelMatrix = rotate(_modelMatrix, radians(20.0f), vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = rotate(mat4(1.0f), (float)glfwGetTime(), vec3(1.0f, 1.0f, 0.0f));
	return m_modelMatrix;
}