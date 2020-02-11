#include "Header Files\ParticleSystem.h"

ParticleSystem::ParticleSystem(int nrOfParticles)
{
	m_particleShader = new Shader("src/Shaders/ParticleVS.glsl", "src/Shaders/ParticleFS.glsl");
	m_nrOfParticle = nrOfParticles;
	m_particles = new Particle[m_nrOfParticle];
	this->Init(nrOfParticles);
}

ParticleSystem::~ParticleSystem()
{
	delete m_particleShader;
}

void ParticleSystem::Init(int nrOfParticles)
{

	static const GLfloat g_vertex_buffer_data[] = {
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_billBoardBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_billBoardBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &m_particlePosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_particlePosBuffer);
	glBufferData(GL_ARRAY_BUFFER, nrOfParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &m_particleColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_particleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, nrOfParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);


}

int ParticleSystem::FindParticle()
{

	for (int i = m_lastUsedParticle; i < m_nrOfParticle; i++) {
		if (m_particles[i].life < 0) {
			m_lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < m_lastUsedParticle; i++) {
		if (m_particles[i].life < 0) {
			m_lastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void ParticleSystem::SortParticles()
{
	std::sort(&m_particles[0], &m_particles[m_nrOfParticle]);
}