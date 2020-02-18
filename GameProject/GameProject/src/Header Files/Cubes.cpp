

#include "Cubes.h"

Cubes::Cubes(vec3 center)
{
	m_center = center;
	/*m_corners[0] = vec3(center.x + 3, center.y + 3, center.z + 3);
	m_corners[1] = vec3(center.x - 3, center.y + 3, center.z + 3);
	m_corners[2] = vec3(center.x - 3, center.y - 3, center.z + 3);
	m_corners[3] = vec3(center.x - 3, center.y + 3, center.z - 3);
	m_corners[4] = vec3(center.x - 3, center.y - 3, center.z - 3);
	m_corners[5] = vec3(center.x + 3, center.y - 3, center.z + 3);
	m_corners[6] = vec3(center.x + 3, center.y - 3, center.z - 3);
	m_corners[7] = vec3(center.x + 3, center.y + 3, center.z - 3);
	*/
	m_corners[0] = vec3(center.x + 1.5, center.y + 1.5, center.z + 1.5);
	m_corners[1] = vec3(center.x - 1.5, center.y + 1.5, center.z + 1.5);
	m_corners[3] = vec3(center.x - 1.5, center.y + 1.5, center.z - 1.5);
	m_corners[7] = vec3(center.x + 1.5, center.y + 1.5, center.z - 1.5);

	for (int i = 0; i < 8; i++) {
		m_cornerUsed[i] = false;
	}

	m_cornerUsed[0] = true;
	m_cornerUsed[1] = true;
	m_cornerUsed[3] = true;
	m_cornerUsed[7] = true;
	vec3 pos;
	vec3 normal;
	vec3 color;
	vec2 uv;
	VertexData temp;
	temp.pos = m_corners[0];
	temp.normal = vec3(0, 1, 0);
	temp.color = vec3(1,0,0);
	temp.uv = vec2(1);
	m_vertices.push_back(temp);
	temp.pos = m_corners[1];
	m_vertices.push_back(temp);
	temp.pos = m_corners[3];
	m_vertices.push_back(temp);
	temp.pos = m_corners[7];
	m_vertices.push_back(temp);

	m_indices.push_back(0);
	m_indices.push_back(1);
	m_indices.push_back(2);
	m_indices.push_back(0);
	m_indices.push_back(2);
	m_indices.push_back(3);
	
	Material temp1;
	temp1.Diffuse = vec3(0.8);
	temp1.Specular = vec3(1);
	temp1.Ambient = vec3(1);
	temp1.Shininess = 625;
	
	m_material.push_back(temp1);
	
	TextureData temp2;
	temp2.id = 0;
	temp2.type = " ";
	m_mesh = new Mesh(m_vertices, m_indices, m_textures, m_material);
}

Cubes::~Cubes()
{
	delete m_mesh;
}

void Cubes::CreateMesh()
{


}

vector<VertexData> Cubes::GetVertexData()
{
	return m_vertices;
}

vector<unsigned int> Cubes::GetIndicesData()
{
	return m_indices;
}

vector<TextureData> Cubes::GetTextureData()
{
	return m_textures;
}

vector<Material> Cubes::GetMaterialData()
{
	return m_material;
}

Mesh* Cubes::GetMesh()
{
	return m_mesh;
}

void Cubes::MarchCube(int lookupIndex)
{
	if (lookupIndex == 0 || lookupIndex == 255)
		return;

	int edgeIndex = 0;
	VertexData temp;
	temp.normal = vec3(0, 1, 0);
	temp.color = vec3(1, 0, 0);
	temp.uv = vec2(1);
	for (int i = 0; i < 5; i++) // Maximum 5 triangles per cell
	{
		for (int j = 0; j < 3; j++) // Maximum 3 points per triangle
		{
			int indice = m_triTable[lookupIndex][edgeIndex];

			if (indice == -1)
				return;

			vec3 v1 = m_center + m_edgeTable[indice][0]; // Start 
			vec3 v2 = m_center + m_edgeTable[indice][1]; // and end of a cube-edge. 
			vec3 vPos = (v1 + v2) * 0.5f; // Middle point of edge

			temp.pos = vPos;
			m_vertices.push_back(temp);
			m_indices.push_back(m_vertices.size() - 1);


			m_tutVerts.push_back(vPos);
			m_triangles.push_back(m_tutVerts.size() - 1); // Adds the current number of vertices as an index
			edgeIndex++;
		}
	}
}

void Cubes::BuildMesh()
{
	if (m_vertices.size() > 0) {
		m_mesh->UpdateMesh(m_vertices, m_indices);
	}
}

void Cubes::ClearMeshData()
{
	m_vertices.clear();
	m_indices.clear();
}

void Cubes::Update()
{

	//51 ÄR PLATT!!!
	m_lookupIndex = 51;
	cout << m_lookupIndex << endl;
	ClearMeshData();
	MarchCube(m_lookupIndex);
	BuildMesh();
	if (m_lookupIndex == 255)
		m_lookupIndex = 0;
}
