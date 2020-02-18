#pragma once 
#include "Include.h" 

class MarchingCubes 
{ 
private: 
    // For mesh
    vector<VertexData> m_vertices;
    vector<unsigned int> m_indices;
    vector<TextureData> m_textures;
    vector<Material> m_materials;
    Mesh* m_mesh;

    // From tutorial
    vector<vec3> m_tutVerts;
    vector<int> m_triangles; // Technically indices
    int m_lookupIndex = -1;

    float terrainSurface = 0.5f;
    int width = 32;
    int height = 30;
    float terrainMap[32 + 1][32 + 1][32 + 1]; // width height width

public:  
    MarchingCubes();
    ~MarchingCubes();

    void Init();
    void Update();
    void MarchCube(vec3 position, float *cube);
    void ClearMeshData();
    void BuildMesh();
    void Draw();
    void PopulateTerrainMap();
    int GetLookupIndex(float *cube);
    void CreateMeshData();

    /* 
    int Polyganise(GridCell grid, double isolevel, Triangle* triangles);
    vec3 VertexInterpolation(double isolevel, vec3 p1, vec3 p2, double valueP1, double valueP2);
    void March(GridCell* grids);
    void Init();
    void Update();
    void Draw(int lookup);
    GridCell* GetGrid();
    */
};

