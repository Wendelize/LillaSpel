#pragma once 
#include "Include.h" 
#include "Transform.h"
#include <atomic>
#include <thread>
#include "PerlinNoise.h"

#define WIDTH 50 
#define HEIGHT 12 

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

    PerlinNoise* m_noise;

    float m_terrainSurface = 0.5f;
    int m_width = WIDTH;
    int m_height = HEIGHT;
    float m_terrainMap[WIDTH + 1][HEIGHT + 1][WIDTH + 1]; // width height width 
    float m_middle = WIDTH / 2;
    int m_shrink = 0;
    float m_time = 0.0f;
    int m_currentLvl = 6;
    int m_holeSize = 8;
    bool m_smoothTerrain = true;
    bool m_way = true;
    int m_lvlChanger = 0;
    int m_terraceHeight = 2;
    bool m_flatShaded = true;
    bool m_hole = false;
    btRigidBody* m_body;
    btCollisionShape* m_platformShape;
    btTransform* m_btTransform;
    btDefaultMotionState* m_motionState;
    btCollisionShape*       m_newPlatformShape;
    btTransform*            m_newBtTransform;
    btDefaultMotionState*   m_newMotionState;
    btRigidBody*            m_newBody;
    btTriangleMesh m_newTetraMesh;
    std::atomic<btRigidBody*> test; 
public:  
    MarchingCubes();
    ~MarchingCubes();

    void Init();
    void Update(GLFWwindow* window);
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
    void MapUpdate();

    vector<VertexData> GetVertices();
    void SetCurrentLevel(int level);
    int GetCurrentLevel();
    void SetHoleSize(int holeSize);
    int GetHoleSize();
    int VertForIndice(vec3 vertex);
    void CalculateNormals();
    void MakeHole(vec3 position);
};

