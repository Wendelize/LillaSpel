#include "Header Files\ParticleSystem.h"

ParticleSystem::ParticleSystem(int nrOfParticles)
{
	m_particleShader = new Shader("src/Shaders/ParticleVS.glsl", "src/Shaders/ParticleFS.glsl");
	this->Init(nrOfParticles);
}

ParticleSystem::~ParticleSystem()
{
	delete m_particleShader;
}

void ParticleSystem::Init(int nrOfParticles)
{
	//Mesh
	GLuint VBO;
	struct Vertex
	{
		vec2 pos;
	};

	Vertex quad[] = 
	{
		{{0.0f, .5f}},
		{{.5f, 0.0f}}, 
		{{0.0f, 0.0f}}, 
	   				 
		{{0.0f, .5f}}, 
		{{.5f, .5f}}, 
		{{.5f, 0.0f}}
	};

	GLuint _nrOf= sizeof(quad) / sizeof(*quad);
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(m_VAO);

	//Mesh-buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _nrOf*sizeof(Vertex), quad, GL_STATIC_DRAW);

	//Mesh-attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glBindVertexArray(0);

	//Create default particles
	for (int i = 0; i < nrOfParticles; i++)
	{
		m_particles.push_back(Particle());
	}
}

void ParticleSystem::Update(float dt, vec2 emitterPos)
{
	//For loop if we want to add more than one new particle
	int indexDeadParticle = GetDeadParticle();
	RespawnParticle(m_particles[indexDeadParticle], emitterPos, dt);

	for (auto p : m_particles)
	{
		p.m_life -= dt;
		if (p.m_life > 0.f) 
		{
			//p.m_position -= p.m_velocity * dt;
			//p.m_color.b -= dt * 1.0f;
		}
	}
}

void ParticleSystem::Render()
{
	m_particleShader->UseShader();
	for (Particle p : m_particles)
	{
		//if (p.m_life > 0.0f)
		//{
			m_particleShader->SetUniform("u_Offset", p.m_position);
			m_particleShader->SetUniform("u_Color", p.m_color);
			glBindVertexArray(this->m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		//}
	}
}

int ParticleSystem::GetDeadParticle()
{
	int i = 0;
	for (auto p: m_particles)
	{
		if (p.m_life <= 0.f)
		{		
			return i;
			i++;
		}
	}
	return 0; //If no dead particle set first particle. 
	/*int indexDeadParticle = 0;
	for (int i = indexDeadParticle; i < m_nrOfParticles; i++)
	{
		if (m_particles[i].m_life <= 0.f)
		{
			return indexDeadParticle = i;
		}
	}
	for (int i = 0; i < indexDeadParticle; i++)
	{
		if (m_particles[i].m_life <= 0.f)
		{
			return indexDeadParticle = i;
		}
	}
	return 0;*/
}

void ParticleSystem::RespawnParticle(Particle& particle, vec2 emitterPos, float dt)
{
	srand(dt);
	float random = (rand() % 10 + 1);
	particle.m_position = vec2(5.0, 5.0);//emitterPos + random; 
	particle.m_color = vec3(random, 0, 0); 
	particle.m_life = 1.0f;
	particle.m_velocity = vec3(9.82f, 9.82f, 9.82f);
}
