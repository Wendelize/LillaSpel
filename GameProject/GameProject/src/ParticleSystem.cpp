#include "Header Files\ParticleSystem.h"

ParticleSystem::ParticleSystem(int nrOfParticles)
{
	m_nrOfParticle = nrOfParticles;
	m_particles = new Particle[m_nrOfParticle];
	m_particleShader = new Shader("src/Shaders/ParticleVS.glsl", "src/Shaders/ParticleFS.glsl");
	m_particlePos	= new GLfloat[4 * m_nrOfParticle];
	m_particleColor = new GLfloat[4 * m_nrOfParticle];

	this->Init();
}

ParticleSystem::~ParticleSystem()
{
	delete m_particleShader;
}

void ParticleSystem::InitParticles()
{
	


	for (int i = 0; i < m_nrOfParticle; i++) {
		m_particles[i].life = -1.0f;
		m_particles[i].cameraDist = -1.0f;
	}
}

void ParticleSystem::Init()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	InitParticles();

	static const GLfloat g_vertex_buffer_data[] = {
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};

	glGenBuffers(1, &m_billBoardBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_billBoardBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &m_particlePosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_particlePosBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_nrOfParticle * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &m_particleColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_particleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_nrOfParticle * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);


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

void ParticleSystem::GenerateParticles(float dt)
{
	int newparticles = (int)(dt * 10000.0);
	if (newparticles > (int)(0.016f * 10000.0))
		newparticles = (int)(0.016f * 10000.0);

	for (int i = 0; i < newparticles; i++) {
		int particleIndex = FindParticle();
		m_particles[particleIndex].life = 5.0f; // This particle will live 5 seconds.
		m_particles[particleIndex].position = glm::vec3(0, 0, -20.0f);

		float spread = 1.5f;
		glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
		);

		m_particles[particleIndex].velocity = maindir + randomdir * spread;

		m_particles[particleIndex].color.x = rand() % 256;
		m_particles[particleIndex].color.y = 0;
		m_particles[particleIndex].color.z = rand() % 256;
		m_particles[particleIndex].color.w = (rand() % 256) / 3;

		m_particles[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;

	}
}

void ParticleSystem::SimulateParticles(float dt)
{
	m_particleCount = 0;
	for (int i = 0; i < m_nrOfParticle; i++) {

		Particle& p = m_particles[i]; // shortcut

		if (p.life > 0.0f) {

			// Decrease life
			p.life -= dt;
			if (p.life > 0.0f) {

				// Simulate simple physics : gravity only, no collisions
				p.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * (float)dt * 0.5f;
				p.position += p.velocity * (float)dt;
				p.cameraDist = length(p.position - vec3(0, 18, 33));
				//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

				// Fill the GPU buffer
				m_particlePos[4 * m_particleCount + 0] = p.position.x;
				m_particlePos[4 * m_particleCount + 1] = p.position.y;
				m_particlePos[4 * m_particleCount + 2] = p.position.z;

				m_particlePos[4 * m_particleCount + 3] = p.size;

				m_particleColor[4 * m_particleCount + 0] = p.color.x;
				m_particleColor[4 * m_particleCount + 1] = p.color.y;
				m_particleColor[4 * m_particleCount + 2] = p.color.z;
				m_particleColor[4 * m_particleCount + 3] = p.color.w;

			}
			else {
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.cameraDist = -1.0f;
			}

			m_particleCount++;

		}
	}

	SortParticles();
	cout << "Particle count : " << m_particleCount;
}

void ParticleSystem::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_particlePosBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_nrOfParticle * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_particleCount * sizeof(GLfloat) * 4, m_particlePos);

	glBindBuffer(GL_ARRAY_BUFFER, m_particleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_nrOfParticle * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_particleCount * sizeof(GLubyte) * 4, m_particleColor);

	m_particleShader->UseShader();
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_tex);
	//m_particleShader->SetTexture2D(0, "u_Tex", m_tex);


	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_billBoardBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_particlePosBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_particleColorBuffer);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1


	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_particleCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

Shader* ParticleSystem::GetShader()
{
	return m_particleShader;
}
