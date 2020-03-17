#pragma once
#include "Include.h"

struct Particle
{
	vec3 position, velocity;
	vec4 color;
	float size, rotate, weight;
	float life;
	float cameraDist;

	Particle()
		: position(0), velocity(0), color(0), size(0), rotate(0), weight(0), life(0), cameraDist(0)
	{
	}

	bool operator<(const Particle& other) const
	{
		return cameraDist > other.cameraDist;
	}
};

class ParticleSystem
{
private:
	Shader* m_particleShader;
	GLuint m_billBoardBuffer, m_particlePosBuffer, m_particleColorBuffer;
	GLfloat *m_particlePos, *m_particleColor;
	GLuint m_tex;
	GLuint m_VAO;
	int m_nrOfParticle = 100, m_lastUsedParticle = 0, m_particleCount;
	Particle* m_particles;
	bool m_active;
	float m_startLife = 0;
	float m_startSize = 0;
	float m_gravity = 0;

public:

	ParticleSystem(int nrOfParticles);
	~ParticleSystem();

	void InitParticles();
	void Init();
	int FindParticle();
	void SortParticles();
	void Draw();
	Shader* GetShader();

	void GenerateParticles(vec3 emitterPos, float speed, float spread, float life, vec3 color1, vec3 color2, float size,
	                       vec3 dir, float gravity);
	void Simulate(float dt);

	void SetActive();
	bool GetActive();
};
