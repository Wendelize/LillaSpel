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
	delete m_particles;
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

void ParticleSystem::GenerateParticles(float dt, vec3 emitterPos, float velocity)
{
	//NOT IN USE ATM - "Template"
	int newparticles = m_nrOfParticle;
	for (int i = 0; i < newparticles; i++) 
	{
		m_particles[i].life = 2.5f; 
		m_particles[i].position = emitterPos;

		float spread = velocity;
		glm::vec3 maindir = glm::vec3(0.0f, 8.0f, 0.0f);
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
		);

		m_particles[i].velocity = maindir + randomdir * spread;

		m_particles[i].color.x = rand() % 256;
		m_particles[i].color.y = rand() % 256;
		m_particles[i].color.z = rand() % 256;
		m_particles[i].color.w = (rand() % 256) / 3;

		m_particles[i].size = 0.3;
	}
}

void ParticleSystem::SimulateParticles(float dt, vec3 emitterPos)
{
	//IS NOT IN USE ATM - "template"
	m_particleCount = 0;
	for (int i = 0; i < m_nrOfParticle; i++) {

		Particle& p = m_particles[i]; // shortcut

			// Decrease life
			p.life -= dt;
			if (p.life > 0.0f) 
			{
				// Simulate simple physics : gravity only, no collisions
				p.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * (float)dt * 0.5f;
				p.position += p.velocity * (float)dt;
				p.cameraDist = length(p.position - vec3(0, 3, 33));
				p.size *= 0.97; //Ist�llet  f�r transparens minska efterhand
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
			else 
			{
				p.life = 2.5f; // This particle will live 5 seconds.
				p.position = emitterPos;

				float spread = 1.5f;
				glm::vec3 maindir = glm::vec3(0.0f, 5.0f, 0.0f);
				glm::vec3 randomdir = glm::vec3(
					(rand() % 2000 - 1000.0f) / 1000.0f,
					(rand() % 2500 - 1000.0f) / 1000.0f,
					(rand() % 1500 - 1000.0f) / 1000.0f
				);

				p.velocity = maindir + randomdir * spread;

				p.color.x = rand() % 256;
				p.color.y = 0;
				p.color.z = rand() % 256;
				//p.color.w = (rand() % 256) / 3; G�r ingen skilland,vi kan inte ha transparens. 

				p.size = 0.3;
			}
			m_particleCount++;
	}
	//SortParticles();	//Beh�ver inte sortera om vi revivar partikeln s� fort vi hittar att den �r d�d.
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

void ParticleSystem::GenerateParticlesForCollision( vec3 emitterPos, float velocity)
{
	for (int i = 0; i < m_nrOfParticle; i++)
	{
		m_particles[i].life = 1.f; 
		m_particles[i].position = emitterPos;

		float spread = velocity;
		glm::vec3 maindir = glm::vec3(0.0f, 8.0f, 0.0f);
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
		);
		m_particles[i].velocity = maindir + randomdir * spread;

		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		vec4  rgb = vec4(1.0f, r, 0, 1.0f);
		m_particles[i].color = rgb;
;
		m_particles[i].size = 0.25;
	}
}

void ParticleSystem::Collision(float dt)
{
	int nrOfDead = 0;
	m_particleCount = 0;
	for (int i = 0; i < m_nrOfParticle; i++) 
	{
		Particle& p = m_particles[i];
		if (p.life > 0.0f)
		{
			p.life -= dt;

			// Simulate simple physics : gravity only, no collisions
			p.velocity += glm::vec3(0.0f, -70.f, 0.0f) * (float)dt * 0.5f;
			p.position += p.velocity * (float)dt;
			p.cameraDist = length(p.position - vec3(0, 3, 33));
			p.size *= 0.95; //Instead of trancparancy reduce size
			
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
		else
		{
			p.life = 0;
			nrOfDead++;

			if (nrOfDead >= m_nrOfParticle)
			{
				m_active = false;
			}
		}
		m_particleCount++;
	}
}

void ParticleSystem::GenerateParticlesForVictory(vec3 emitterPos)
{
	for (int i = 0; i < m_nrOfParticle; i++)
	{
		int particleIndex = FindParticle();
		m_particles[particleIndex].life = 3.f;
		m_particles[particleIndex].position = emitterPos;

		float spread = 5.0f;
		glm::vec3 maindir = glm::vec3(0.0f, 5.0f, 0.0f);
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
		);

		m_particles[particleIndex].velocity = maindir + randomdir * spread;

		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		m_particles[particleIndex].color.x = r;
		m_particles[particleIndex].color.y = g;
		m_particles[particleIndex].color.z = b;
		m_particles[particleIndex].color.w = 1;

		m_particles[particleIndex].size = 0.25;
	}
}

void ParticleSystem::Victory(float dt, vec3 emitterPos)
{
	m_particleCount = 0;
	for (int i = 0; i < m_nrOfParticle; i++)
	{
		Particle& p = m_particles[i]; 
		if (p.life > 0.0f)
		{			
			p.life -= dt;
			if (p.life > 0.0f)
			{
				// Simulate simple physics : gravity only, no collisions
				p.velocity += glm::vec3(0.0f, -1.f, 0.0f) * (float)dt * 0.5f;
				p.position += p.velocity * (float)dt;
				p.cameraDist = length(p.position - vec3(0, 3, 33));
				p.size *= 0.98;
		
				// Fill the GPU buffer
				m_particlePos[4 * m_particleCount + 0] = p.position.x;
				m_particlePos[4 * m_particleCount + 1] = p.position.y;
				m_particlePos[4 * m_particleCount + 2] = p.position.z ;

				m_particlePos[4 * m_particleCount + 3] = p.size;

				m_particleColor[4 * m_particleCount + 0] = p.color.x;
				m_particleColor[4 * m_particleCount + 1] = p.color.y;
				m_particleColor[4 * m_particleCount + 2] = p.color.z;
				m_particleColor[4 * m_particleCount + 3] = p.color.w;
			}
			else
			{
				// Particles that just died will be put at the end of the buffer in SortParticles();
				GenerateParticlesForVictory(emitterPos);
				p.cameraDist = -1.0f;
			}
			m_particleCount++;
		}
	}
	SortParticles();
}

void ParticleSystem::SetActive()
{
	m_active = true;
}

bool ParticleSystem::GetActive()
{
	return m_active;
}
