#include "Header Files\ParticleSystem.h"

ParticleSystem::ParticleSystem(int nrOfParticles)
{
	m_nrOfParticle = nrOfParticles;
	m_particles = new Particle[m_nrOfParticle];
	m_particleShader = new Shader("src/Shaders/ParticleVS.glsl", "src/Shaders/ParticleFS.glsl");
	m_particlePos	= new GLfloat[4 * m_nrOfParticle];
	m_particleColor = new GLfloat[4 * m_nrOfParticle];
	m_active = false;

	this->Init();
}

ParticleSystem::~ParticleSystem()
{
	delete[] m_particles;
	delete m_particleShader;
	delete m_particlePos;
	delete m_particleColor;
	
}

void ParticleSystem::InitParticles()
{
	for (int i = 0; i < m_nrOfParticle; i++) {
		m_particles[i].life = 0.f;
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
	glBufferData(GL_ARRAY_BUFFER, m_nrOfParticle * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
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


void ParticleSystem::Draw()
{	 
	glBindBuffer(GL_ARRAY_BUFFER, m_particlePosBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_nrOfParticle * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); 
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_particleCount * sizeof(GLfloat) * 4, m_particlePos);

	glBindBuffer(GL_ARRAY_BUFFER, m_particleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_nrOfParticle * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_particleCount * sizeof(GLfloat) * 4, m_particleColor);

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
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, 0, (void*)0);

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

void ParticleSystem::GenerateParticles( vec3 emitterPos, float spread, float life, vec3 color1, vec3 color2, float size, vec3 dir)
{
	for (int i = 0; i < m_nrOfParticle; i++)
	{
		m_particles[i].life = life;
		m_particles[i].position = emitterPos;

		//glm::vec3 maindir = glm::vec3(0.0f, 8.0f, 0.0f);
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
		);
		m_particles[i].velocity = dir + randomdir * spread;

		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if (color1 == vec3(NULL))
		{
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			m_particles[i].color.x = r;
			m_particles[i].color.y = g;
			m_particles[i].color.z = b;
			m_particles[i].color.w = 1;
		}
		else
		{
			m_particles[i].color = vec4(color1, 1.0f);;

			if(color2.x == 1)
				m_particles[i].color.x += static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			if (color2.y == 1)
				m_particles[i].color.y += static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			if (color2.z == 1)
				m_particles[i].color.z += static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		}
		
		m_particles[i].size = size;
	}
}

void ParticleSystem::Simulate(float dt)
{
	int nrOfDead = 0;
	m_particleCount = m_nrOfParticle;
	for (int i = 0; i < m_nrOfParticle; i++) 
	{
		Particle& p = m_particles[i];
		if (p.life > 0.0f)
		{
			p.life -= dt;

			// Simulate simple physics : gravity only, no collisions
			p.velocity += glm::vec3(0.0f, -9.82, 0.0f) * (float)dt;
			p.position += p.velocity * (float)dt;
			p.cameraDist = length(p.position - vec3(0, 3, 33));
			p.size *= 0.95; //Instead of trancparancy reduce size
			
			// Fill the GPU buffer
			m_particlePos[4 * i + 0] = p.position.x;
			m_particlePos[4 * i + 1] = p.position.y;
			m_particlePos[4 * i + 2] = p.position.z;

			m_particlePos[4 * i + 3] = p.size;

			m_particleColor[4 * i + 0] = p.color.x;
			m_particleColor[4 * i + 1] = p.color.y;
			m_particleColor[4 * i + 2] = p.color.z;
			m_particleColor[4 * i + 3] = p.color.w;
		}
		else
		{
			p.life = 0;
			nrOfDead++;

			if (nrOfDead >= m_nrOfParticle)
			{
				m_active = false;
			}
		}
	}
}

void ParticleSystem::SetActive()
{
	m_active = true;
}

bool ParticleSystem::GetActive()
{
	return m_active;
}
