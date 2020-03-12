#pragma once
#include "Mesh.h"

class Model
{
private:
	vector<Mesh> m_meshes;
	vector<TextureData> m_texturesLoaded;
	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	bool m_textureBool = false;
	string m_dir = "../Models/";

	void LoadModel(string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	TextureData LoadTexture(aiMaterial* mat, aiTextureType type, string typeName);
	Material loadMaterial(aiMaterial* mat);
	int TextureFromFile(const char* path, const string& directory, bool gamma);

public:
	Model(const char* path, bool textureBool = false);
	~Model();

	void Draw(Shader* shader);
	vector<Mesh> GetMeshes();
};
