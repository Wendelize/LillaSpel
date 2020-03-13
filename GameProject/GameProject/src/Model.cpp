#include "Header Files/Include.h"
#include "Header Files/stb_image.h"

void Model::LoadModel(string path)
{
	Assimp::Importer _import;
	const aiScene* _scene = _import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!_scene || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !_scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << _import.GetErrorString() << endl;
		return;
	}

	ProcessNode(_scene->mRootNode, _scene);
	_import.FreeScene();
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* _mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(ProcessMesh(_mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<VertexData> vertices;
	vector<unsigned int> indices;
	vector<TextureData> textures;
	vector<Material> materials;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		VertexData vertex;
		// Positions
		vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.pos = vector;
		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		// COLOR
		vertex.color = vec3(0, 1, 0);

		// UV-coords
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			vec2 _vec;
			_vec.x = mesh->mTextureCoords[0][i].x;
			_vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = _vec;
		}
		else
			vertex.uv = vec2(0.0f, 0.0f);

		// Add vertex to VertexData
		vertices.push_back(vertex);
	}
	// Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* _material = scene->mMaterials[mesh->mMaterialIndex];
		materials.push_back(loadMaterial(_material));
	}

	if (m_textureBool == true)
	{
		aiMaterial* materialAI = scene->mMaterials[mesh->mMaterialIndex];
		// 1. diffuse map
		TextureData diffuseMap = LoadTexture(materialAI, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.push_back(diffuseMap);
		// 2. specular map
		TextureData specularMap = LoadTexture(materialAI, aiTextureType_SPECULAR, "texture_specular");
		textures.push_back(specularMap);
	}

	return Mesh(vertices, indices, textures, materials);
}

TextureData Model::LoadTexture(aiMaterial* mat, aiTextureType type, string typeName)
{
	TextureData texture;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_texturesLoaded.size(); j++)
		{
			if (std::strcmp(m_texturesLoaded[j].path.data(), str.C_Str()) == 0)
			{
				texture = m_texturesLoaded[j];
				skip = true;
				// a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{
			// if texture hasn't been loaded already, load it
			texture.id; // = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			m_texturesLoaded.push_back(texture); // add to loaded textures
		}
	}
	return texture;
}


Material Model::loadMaterial(aiMaterial* mat)
{
	Material _material;
	aiColor3D _color(0.f, 0.f, 0.f);
	float _shininess;

	mat->Get(AI_MATKEY_COLOR_DIFFUSE, _color);
	_material.Diffuse = vec3(_color.r, _color.g, _color.b);

	mat->Get(AI_MATKEY_COLOR_AMBIENT, _color);
	_material.Ambient = vec3(_color.r, _color.g, _color.b);

	mat->Get(AI_MATKEY_COLOR_SPECULAR, _color);
	_material.Specular = vec3(_color.r, _color.g, _color.b);

	mat->Get(AI_MATKEY_SHININESS, _shininess);
	_material.Shininess = _shininess;

	return _material;
}

int Model::TextureFromFile(const char* path, const string& directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

Model::Model(const char* path, bool textureBool)
{
	LoadModel(path);
	m_textureBool = textureBool;
}

Model::~Model()
{
}

void Model::Draw(Shader* shader)
{
	for (uint i = 0; i < m_meshes.size(); i++)
		m_meshes[i].Draw(shader);
}

vector<Mesh> Model::GetMeshes()
{
	return m_meshes;
}
