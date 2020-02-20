#pragma once 
#include "Include.h" 
#include "Transform.h"

class MarchingCubes 
{ 
private: 
    // For mesh
    vector<VertexData> m_vertices;
    vector<unsigned int> m_indices;
    vector<TextureData> m_textures;
    vector<Material> m_materials;
    Mesh* m_mesh;
    Transform* m_transform;
    btBvhTriangleMeshShape* m_physicsMesh;
    btTriangleMesh m_tetraMesh;

    // ADD PHYSICS! 

    float m_terrainSurface = 0.5f;
    int m_width = 32;
    int m_height = 1;
    float m_terrainMap[32 + 1][32 + 1][32 + 1]; // width height width
    float m_middle = 16;
    int m_shrink = 0;
    float m_time = 0.0f;
    int m_currentLvl = 5;
    int m_holeSize = 8;
    bool m_smoothTerrain = true;
    bool m_way = true;
    btRigidBody* m_body;
    btCollisionShape* m_platformShape;
    btTransform* m_btTransform;
    btDefaultMotionState* m_motionState;
public:  
    MarchingCubes();
    ~MarchingCubes();

    void Init();
    void Update();
    void MarchCube(vec3 position);
    void ClearMeshData();
    void BuildMesh();
    void Draw(Shader* shader);
    void PopulateTerrainMap(int level);
    int GetLookupIndex(float *cube);
    void CreateMeshData();
    float SampleTerrain(vec3 point);
    void DrawWarning(int x, int y, int z);
    btRigidBody* GetBody();
    void CreatePhysics();
    void UpdatePhysics();

    vector<VertexData> GetVertices();
    void SetCurrentLevel(int level);
    int GetCurrentLevel();
    void SetHoleSize(int holeSize);
    int GetHoleSize();
};

