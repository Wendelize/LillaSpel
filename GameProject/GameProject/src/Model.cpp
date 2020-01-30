#include "Header Files/Include.h"


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
    vector<VertexData> _vertices;
    vector<unsigned int> _indices;
    vector<TextureData> _textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        VertexData _vertex;
        // Positions
        vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        _vertex.pos = vector;
        // Normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        _vertex.normal = vector;
        // COLOR
        _vertex.color = vec3(i, 0, 0);

        // UV-coords
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            _vertex.uv = vec;
        }
        else
            _vertex.uv = glm::vec2(0.0f, 0.0f);

        // Add vertex to VertexData
        _vertices.push_back(_vertex);
    }
    // Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            _indices.push_back(face.mIndices[j]);
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        //TODO : Textures? Only one. 
    }

    return Mesh(_vertices, _indices, _textures);
}

vector<TextureData> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    //TODO : TEXTURE!
    vector<TextureData> _tex;
    return _tex;
}

Model::Model(const char* path)
{
	LoadModel(path);
}

Model::~Model()
{

}

void Model::Draw(Shader *shader)
{
	for (uint i = 0; i < m_meshes.size(); i++)
		m_meshes[i].Draw(shader);
}
