#include "Header Files\MCubes.h"

MCubes::MCubes()
{
	for (int i = 0; i < 9; i += 3) {
		for (int j = 0; j < 9; j += 3) {
//			for (int k = 0; k < 9; k += 3) {

				m_Cubes.push_back(new Cubes(vec3(i,  0, j)));
				m_Cubes.push_back(new Cubes(vec3(-i, 0, j)));
				m_Cubes.push_back(new Cubes(vec3(i,  0, -j)));
				m_Cubes.push_back(new Cubes(vec3(-i, 0, -j)));


				m_Cubes.push_back(new Cubes(vec3(i,  -0, j)));
				m_Cubes.push_back(new Cubes(vec3(-i, -0, j)));
				m_Cubes.push_back(new Cubes(vec3(i,  -0, -j)));
				m_Cubes.push_back(new Cubes(vec3(-i, -0, -j)));
//			}
		}
	}
}

MCubes::~MCubes()
{
	delete m_Mesh;
}

void MCubes::CreateMesh()
{

}

void MCubes::UpdateCubes()
{
	for (int i = 0; i < m_Cubes.size(); i++) {
		m_Cubes.at(i)->Update();
	}
}

void MCubes::RenderCubes(Shader* shader)
{
	shader->UseShader();
	shader->SetUniform("u_Model", m_model);
	shader->SetUniform("u_PlayerColor", vec3(1));
	shader->SetUniform("u_Glow", false);
	for (int i = 0; i < m_Cubes.size(); i++) {
		m_Cubes.at(i)->GetMesh()->Draw(shader);
	}
}


