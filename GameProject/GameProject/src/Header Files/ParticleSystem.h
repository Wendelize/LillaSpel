#pragma once
#include "Include.h" 

struct Particle
{
	vec3 position, velocity, color;
	float size, rotate, weight;
	float life;
	float cameraDist;

	Particle()
		: position(0), velocity(0), color(0), size(0), rotate(0), weight(0), life(0), cameraDist(0)
	{}

	bool operator<(const Particle& other) const
	{
		return cameraDist > other.cameraDist;
	}
};

class ParticleSystem
{
private:
	Shader* m_particleShader;
	GLuint m_tex;
	GLuint m_VAO;
	GLuint m_billBoardBuffer, m_particlePosBuffer, m_particleColorBuffer;
	int m_nrOfParticle = 100, m_lastUsedParticle = 0;
	Particle* m_particles;

public:
	//vector<Particle> m_particles;

	ParticleSystem(int nrOfParticles);
	~ParticleSystem();

	void Init(int nrOfParticles);
	int FindParticle();
	void SortParticles();
};