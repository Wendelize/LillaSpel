#pragma once
#include "Mesh.h"

class Model
{
private:
	vector<Mesh> m_meshes;
	string m_dir = "../Models/";

	void LoadModel(string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<TextureData> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
		string typeName);

public:
	Model(const char * path);
	~Model();

	void Draw(Shader shader);

};
