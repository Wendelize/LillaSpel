#include "Header Files/Include.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Level::Level(float size)
{
	m_size = size;
	m_verticesPlane = NULL;
	m_indicesPlane = NULL;
	m_verticesSkybox = NULL;
	m_indicesSkybox = NULL;
	CreatePlane();
	CreateSkybox();
	InitLevel();
}

Level::~Level()
{
	// Delete the objects if they have been created.
	if (m_verticesPlane)
	{
		delete[] m_verticesPlane;
	}
	if (m_indicesPlane)
	{
		delete[] m_indicesPlane;
	}
	if (m_verticesSkybox)
	{
		delete[] m_verticesSkybox;
	}
	if (m_indicesSkybox)
	{
		delete[] m_indicesSkybox;
	}
}

void Level::CreatePlane()
{
	//CREATE VERTICES DATA
	levelData vertices[] =
	{
		{ {-m_size / 2,	0,  m_size / 2 }, { 0, 1, 0 }, {1,1,1}, {0, 0}},
		{ {-m_size / 2,	0, -m_size / 2 }, { 0, 1, 0 }, {1,1,1}, {0, 1}},
		{ { m_size / 2,	0, -m_size / 2 }, { 0, 1, 0 }, {1,1,1}, {1, 1}},
		{ { m_size / 2,	0,  m_size / 2 }, { 0, 1, 0 }, {1,1,1}, {1, 0}}
	};
	m_nrOfVPlane = sizeof(vertices) / sizeof(*vertices);
	m_verticesPlane = new vertexData[m_nrOfVPlane];
	memcpy(m_verticesPlane, vertices, sizeof(vertices));

	//CREATE INDECES DATA
	unsigned int indexData[] = { 0, 2, 1,  0, 3, 2 };
	m_nrOfIPlane = sizeof(indexData) / sizeof(*indexData);
	m_indicesPlane = new unsigned int[m_nrOfIPlane];
	memcpy(m_indicesPlane, indexData, sizeof(indexData));
	m_type = 0;

	LoadPlaneTexture();
}

void Level::CreateSkybox()
{
	//CREATE VERTICES DATA
	levelData vertices[] =
	{

		 { {-1,  -1,  1 },	{ 0, 0, 1 },	{ 1, 1, 0 }, {0, 0} },
		 { {-1,   1,  1 },	{ 0, 0, 1 },	{ 1, 1, 0 }, {0, 1} },
		 { { 1,   1,  1 },	{ 0, 0, 1 },	{ 1, 1, 0 }, {1, 1} },
		 { { 1,  -1,  1 },	{ 0, 0, 1 },	{ 1, 1, 0 }, {1, 0} },

		 { { 1,  -1, -1 },	{ 0, 0, -1 },	{ 1, 1, 1 }, {0, 0} },
		 { { 1,   1, -1 },	{ 0, 0, -1 },	{ 1, 1, 1 }, {0, 1} },
		 { {-1,   1, -1 },	{ 0, 0, -1 },	{ 1, 1, 1 }, {1, 1} },
		 { {-1,  -1, -1 },	{ 0, 0, -1 },	{ 1, 1, 1 }, {1, 0} },

		 { {-1,   1,  1 },	{ 0, 1,  0 },	{ 1, 0, 1 }, {0, 0} },
		 { {-1,   1, -1 },	{ 0, 1,  0 },	{ 1, 0, 1 }, {0, 1} },
		 { { 1,   1, -1 },	{ 0, 1,  0 },	{ 0, 0, 1 }, {1, 1} },
		 { { 1,   1,  1 },	{ 0, 1,  0 },	{ 1, 0, 1 }, {1, 0} },

		 { {-1,  -1, -1 },	{ 0, -1, 0 },	{ 1, 0, 0 }, {0, 0} },
		 { {-1,  -1,  1 },	{ 0, -1, 0 },	{ 1, 0, 0 }, {0, 1} },
		 { { 1,  -1,  1 },	{ 0, -1, 0 },	{ 1, 0, 0 }, {1, 1} },
		 { { 1,  -1, -1 },	{ 0, -1, 0 },	{ 1, 0, 0 }, {1, 0} },

		 { { 1,  -1,  1 },	{ 1, 0, 0 },	{ 1, 1, 1 }, {0, 0} },
		 { { 1,   1,  1 },	{ 1, 0, 0 },	{ 1, 1, 1 }, {0, 1} },
		 { { 1,   1, -1 },	{ 1, 0, 0 },	{ 1, 1, 1 }, {1, 1} },
		 { { 1,  -1, -1 },	{ 1, 0, 0 },	{ 1, 1, 1 }, {1, 0} },

		 { {-1,  -1, -1 },	{ -1, 0, 0 },	{ 1, 1, 1 }, {0, 0} },
		 { {-1,   1, -1 },	{ -1, 0, 0 },	{ 1, 1, 1 }, {0, 1} },
		 { {-1,   1,  1 },	{ -1, 0, 0 },	{ 1, 1, 1 }, {1, 1} },
		 { {-1,  -1,  1 },	{ -1, 0, 0 },	{ 1, 1, 1 }, {1, 0} }
	};

	m_nrOfVSkybox = sizeof(vertices) / sizeof(*vertices);
	m_verticesSkybox = new vertexData[m_nrOfVSkybox];
	memcpy(m_verticesSkybox, vertices, sizeof(vertices));

	//CREATE INDECES DATA
	unsigned int indexData[] = { 0,  2,   1,   0,   3,   2,
								  4,  6,   5,   4,   7,   6,
								  8,  10,  9,   8,   11,  10,
								 12,  14,  13,  12,  15,  14,
								 16,  18,  17,  16,  19,  18,
								 20,  22,  21,  20,  23,  22 };
	m_nrOfISkybox = sizeof(indexData) / sizeof(*indexData);
	m_indicesSkybox = new unsigned int[m_nrOfISkybox];
	memcpy(m_indicesSkybox, indexData, sizeof(indexData));
	m_type = 1;

	LoadSkyboxTexture();
}

void Level::InitLevel()
{
	//PLANE
	//Bind VertexArray
	glGenVertexArrays(1, &m_vertexArrayPlane);
	glBindVertexArray(m_vertexArrayPlane);

	//Bind VertexBuffer & IndexBuffer
	glGenBuffers(1, &m_vertexBufferPlane);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferPlane);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSizePlane(), m_verticesPlane, GL_STATIC_DRAW);

	glGenBuffers(1, &m_indicesBufferPlane);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferPlane);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSizePlane(), m_indicesPlane, GL_STATIC_DRAW);

	//Write vertex data to memory
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(float) * 6));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(float) * 9));

	//SKYBOX
	//Bind VertexArray
	glGenVertexArrays(1, &m_vertexArraySkybox);
	glBindVertexArray(m_vertexArraySkybox);

	//Bind VertexBuffer & IndexBuffer
	glGenBuffers(1, &m_vertexBufferSkybox);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferSkybox);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSizeSkybox(), m_verticesSkybox, GL_STATIC_DRAW);

	glGenBuffers(1, &m_indicesBufferSkybox);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferSkybox);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSizeSkybox(), m_indicesSkybox, GL_STATIC_DRAW);

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

void Level::DrawPlane(Shader* shaderProgram)
{
	glBindVertexArray(m_vertexArrayPlane);
	BindTexture();

	glDrawElements(GL_TRIANGLES, m_nrOfIPlane, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Level::DrawSkybox(Shader* shaderProgram)
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	
	glBindVertexArray(m_vertexArraySkybox);
	Bind3DTexture();

	glDrawElements(GL_TRIANGLES, m_nrOfISkybox, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

void Level::DrawLevel(Shader* shaderProgramPlane, Shader* shaderProgramSkybox)
{
	shaderProgramPlane->UseShader();
	DrawPlane(shaderProgramPlane);
	shaderProgramSkybox->UseShader();
	DrawSkybox(shaderProgramSkybox);
}

mat4 Level::GetModelMat()
{
	m_modelMatrix = translate(mat4(1.0f), vec3(0.0, -0.4, 0.0));
	return m_modelMatrix;
}

void Level::LoadPlaneTexture()
{
	glGenTextures(1, &m_texturePlane);
	glBindTexture(GL_TEXTURE_2D, m_texturePlane);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Different ground textures to use.
	string path = "src/Textures/ground.png";
	string path2 = "src/Textures/moss.png";
	string path3 = "src/Textures/burnt_sand.png";

	unsigned char* data = stbi_load("src/Textures/ground.png", &m_width, &m_height, &m_nrChannels, 0);
	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Level::LoadSkyboxTexture()
{
	// stbi_set_flip_vertically_on_load(1);
	vector<string> faces{
		"src/Textures/right.tga", // RIGHT TEXTURE
		"src/Textures/left.tga", // LEFT TEXTURE
		"src/Textures/top.tga", // UP TEXTURE
		"src/Textures/bottom.tga", // DOWN TEXTURE
		"src/Textures/back.tga", // BACK TEXTURE
		"src/Textures/front.tga", // FRONT TEXTURE
	};

	vector<string> faces2{
		"src/Textures/moondust_rt.tga", // RIGHT TEXTURE
		"src/Textures/moondust_lf.tga", // LEFT TEXTURE
		"src/Textures/moondust_up.tga", // UP TEXTURE
		"src/Textures/moondust_dn.tga", // DOWN TEXTURE
		"src/Textures/moondust_bk.tga", // BACK TEXTURE
		"src/Textures/moondust_ft.tga", // FRONT TEXTURE
	};

	vector<string> faces3{
		"src/Textures/posx.jpg", // RIGHT TEXTURE
		"src/Textures/negx.jpg", // LEFT TEXTURE
		"src/Textures/posy.jpg", // UP TEXTURE
		"src/Textures/negy.jpg", // DOWN TEXTURE
		"src/Textures/posz.jpg", // BACK TEXTURE
		"src/Textures/negz.jpg", // FRONT TEXTURE
	};

	glGenTextures(1, &m_textureSkybox);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureSkybox);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces3[i].c_str(), &m_width, &m_height, &m_nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Level::BindTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_texturePlane);
}

void Level::Bind3DTexture()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureSkybox);
}
