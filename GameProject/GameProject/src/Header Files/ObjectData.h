#pragma once
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

//CLASSES
#include "Shader.h"

struct vertexData {
	vec3 pos;
	vec3 normal;
	vec3 color;
};

class Object
{
private:
	vertexData * _vertices;
	unsigned int * _indices;
	GLuint _nrOfV;
	GLuint _nrOfI;
	float _size;
	vec3 _position;
	mat4 _modelMatrix;

	GLsizeiptr vertexBufferSize()const {
		return (_nrOfV * sizeof(vertexData));
	}

	GLsizeiptr indexBufferSize()const {
		return (_nrOfI * sizeof(unsigned int));
	}

public:
	GLuint VB, IB, VA;

	Object(float size, vec3 pos);
	~Object();

	void createPlane();
	void createCube();
	void createPyramid3();
	void createPyramid4();
	void loadMesh();
	void initObject();
	void drawObject(Shader * shaderProgram);
	mat4 getModelMat();
};

Object::Object(float size, vec3 pos)
{
	_size = size;
	_position = pos;

}

Object::~Object()
{
}

void Object::createPlane()
{
	//CREATE VERTICES DATA
	vertexData vertices[] =
	{
	 { {-_size / 2,	0,  _size / 2 }, { 0, 1, 0 }, {1,1,1}},
	 { {-_size / 2, 0, -_size / 2 }, { 0, 1, 0 }, {1,1,1}},
	 { { _size / 2,	0, -_size / 2 }, { 0, 1, 0 }, {1,1,1}},
	 { { _size / 2,	0,  _size / 2 }, { 0, 1, 0 }, {1,1,1}}
	};
	_nrOfV = sizeof(vertices) / sizeof(*vertices);
	_vertices = new vertexData[_nrOfV];
	_vertices = vertices;

	//CREATE INDECES DATA
	unsigned int indexData[] = { 0, 2, 1,  0, 3, 2 };
	_nrOfI = sizeof(indexData) / sizeof(*indexData);
	_indices = new unsigned int[_nrOfI];
	_indices = indexData;
}

void Object::createCube()
{
	//CREATE VERTICES DATA
	vertexData vertices[] =
	{
		//FRONT
		 { {-_size / 2, -_size / 2,  _size / 2 },   { 0, 0, 1 },	{ 1, 1, 0 } },
		 { {-_size / 2,  _size / 2,  _size / 2 },	{ 0, 0, 1 },	{ 1, 1, 0 } },
		 { { _size / 2,  _size / 2,  _size / 2 },	{ 0, 0, 1 },	{ 1, 1, 0 } },
		 { { _size / 2, -_size / 2,  _size / 2 },	{ 0, 0, 1 },	{ 1, 1, 0 } },
		 //BACK														  
		 { { _size / 2, -_size / 2, -_size / 2 },   { 0, 0, -1 },	{ 1, 1, 1 } },
		 { { _size / 2,  _size / 2, -_size / 2 },	{ 0, 0, -1 },	{ 1, 1, 1 } },
		 { {-_size / 2,  _size / 2, -_size / 2 },	{ 0, 0, -1 },	{ 1, 1, 1 } },
		 { {-_size / 2, -_size / 2, -_size / 2 },	{ 0, 0, -1 },	{ 1, 1, 1 } },
		 //TOP														  
		 { {-_size / 2,  _size / 2,  _size / 2 },	{ 0, 1,  0 },	{ 1, 0, 1 } },
		 { {-_size / 2,  _size / 2, -_size / 2 },	{ 0, 1,  0 },	{ 1, 0, 1 } },
		 { { _size / 2,  _size / 2, -_size / 2 },	{ 0, 1,  0 },	{ 0, 0, 1 } },
		 { { _size / 2,  _size / 2,  _size / 2 },	{ 0, 1,  0 },	{ 1, 0, 1 } },
		 //BOT														  
		 { {-_size / 2,  -_size / 2, -_size / 2 },	{ 0, -1, 0 },	{ 1, 0, 0 } },
		 { {-_size / 2,  -_size / 2,  _size / 2 },  { 0, -1, 0 },	{ 1, 0, 0 } },
		 { { _size / 2,  -_size / 2,  _size / 2 },  { 0, -1, 0 },	{ 1, 0, 0 } },
		 { { _size / 2,  -_size / 2, -_size / 2 },  { 0, -1, 0 },	{ 1, 0, 0 } },
		 //RIGHT													  
		 { { _size / 2,  -_size / 2,  _size / 2 },	{ 1, 0, 0 },	{ 1, 1, 1 } },
		 { { _size / 2,   _size / 2,  _size / 2 },	{ 1, 0, 0 },	{ 1, 1, 1 } },
		 { { _size / 2,   _size / 2, -_size / 2 },	{ 1, 0, 0 },	{ 1, 1, 1 } },
		 { { _size / 2,  -_size / 2, -_size / 2 },	{ 1, 0, 0 },	{ 1, 1, 1 } },
		 //LEFT                                  					 
		 { {-_size / 2,  -_size / 2, -_size / 2 },	{ -1, 0, 0 },	{ 1, 1, 1 } },
		 { {-_size / 2,   _size / 2, -_size / 2 },	{ -1, 0, 0 },	{ 1, 1, 1 } },
		 { {-_size / 2,   _size / 2,  _size / 2 },	{ -1, 0, 0 },	{ 1, 1, 1 } },
		 { {-_size / 2,  -_size / 2,  _size / 2 },	{ -1, 0, 0 },	{ 1, 1, 1 } }
	};

	_nrOfV = sizeof(vertices) / sizeof(*vertices);
	_vertices = new vertexData[_nrOfV];
	memcpy(_vertices, vertices, sizeof(vertices));

	//CREATE INDECES DATA
	unsigned int indexData[] = {  0,  2,   1,   0,   3,   2,
								  4,  6,   5,   4,   7,   6,
								  8,  10,  9,   8,   11,  10,
								 12,  14,  13,  12,  15,  14,
								 16,  18,  17,  16,  19,  18,
								 20,  22,  21,  20,  23,  22 };
	_nrOfI = sizeof(indexData) / sizeof(*indexData);
	_indices = new unsigned int[_nrOfI];
	memcpy(_indices, indexData, sizeof(indexData));

}

void Object::createPyramid3()
{
	//CREATE VERTICES DATA
	vertexData vertices[] =
	{
	//BOT
		{ {			0,	0, -_size / 2 }, { 0, -1, 0 }, {1,1,1}},
		{ { _size / 2,  0,  _size / 2 }, { 0, -1, 0 }, {1,1,1}},
		{ {-_size / 2,	0,  _size / 2 }, { 0, -1, 0 }, {1,1,1}},
	//WALL1
		{ {-_size / 2,		0,  _size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ {			0,	_size,			0 }, { 0, 1, 0 }, {1,1,1}},
		{ {			0,		0, -_size / 2 }, { 0, 1, 0 }, {1,1,1}},
	//WALL2
		{ {	_size / 2,		0,	_size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ {			0,	_size,			0 }, { 0, 1, 0 }, {1,1,1}},
		{ {-_size / 2,		0,  _size / 2 }, { 0, 1, 0 }, {1,1,1}},
	//WALL3
		{ {	_size / 2,		0,  _size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ { 0,			_size,			0 }, { 0, 1, 0 }, {1,1,1}},
		{ {	0,				0, -_size / 2 }, { 0, 1, 0 }, {1,1,1}}
	};
	_nrOfV = sizeof(vertices) / sizeof(*vertices);
	_vertices = new vertexData[_nrOfV];
	_vertices = vertices;

	//CREATE INDECES DATA
	unsigned int indexData[] = { 0, 2, 1,  3, 4, 5,  6, 7, 8,  9, 10, 11 };
	_nrOfI = sizeof(indexData) / sizeof(*indexData);
	_indices = new unsigned int[_nrOfI];
	_indices = indexData;
}

void Object::createPyramid4()
{
	//CREATE VERTICES DATA
	vertexData vertices[] =
	{
	//BOT
		{ {-_size / 2,	0,  _size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ {-_size / 2,  0, -_size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ { _size / 2,	0, -_size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ { _size / 2,	0,  _size / 2 }, { 0, 1, 0 }, {1,1,1}},
	//LEFT
		{ {-_size / 2,	0,  _size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ {-_size / 2,  0, -_size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ { _size / 2,	0, -_size / 2 }, { 0, 1, 0 }, {1,1,1}},
	//RIGHT
		{ {-_size / 2,	0,  _size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ {-_size / 2,  0, -_size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ { _size / 2,	0, -_size / 2 }, { 0, 1, 0 }, {1,1,1}},
	//FRONT
		{ {-_size / 2,	0,  _size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ {-_size / 2,  0, -_size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ { _size / 2,	0, -_size / 2 }, { 0, 1, 0 }, {1,1,1}},
	//BACK
		{ {-_size / 2,	0,  _size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ {-_size / 2,  0, -_size / 2 }, { 0, 1, 0 }, {1,1,1}},
		{ { _size / 2,	0,  _size / 2 }, { 0, 1, 0 }, {1,1,1}}
	};
	_nrOfV = sizeof(vertices) / sizeof(*vertices);
	_vertices = new vertexData[_nrOfV];
	_vertices = vertices;

	//CREATE INDECES DATA
	unsigned int indexData[] = { 0, 2, 1,  0, 3, 2 };
	_nrOfI = sizeof(indexData) / sizeof(*indexData);
	_indices = new unsigned int[_nrOfI];
	_indices = indexData;
}

void Object::loadMesh()
{
}

void Object::drawObject(Shader * shaderProgram)
{
	glBindVertexArray(VA);
	//shaderProgram->setUniform("model", _modelMatrix);
	glDrawElements(GL_TRIANGLES, _nrOfI, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Object::initObject()
{
	//Bind VertexArray
	glGenVertexArrays(1, &VA);
	glBindVertexArray(VA);

	//Bind VertexBuffer & IndexBuffer
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize(), _vertices, GL_STATIC_DRAW);


	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize(), _indices, GL_STATIC_DRAW);

	//Write vertex data to memory
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(float) * 6));

	
}

mat4 Object::getModelMat()
{
	//_modelMatrix = translate(mat4(1.0f), _position);
	//_modelMatrix = rotate(_modelMatrix, radians(20.0f), vec3(1.0f, 0.0f, 0.0f));
	_modelMatrix = rotate(mat4(1.0f), (float)glfwGetTime(), vec3(1.0f, 1.0f, 0.0f));
	return _modelMatrix;
}