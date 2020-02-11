#pragma once 
#include "Include.h" 

struct Particle
{
	vec3 m_position;
	vec3 m_velocity;
	vec3 m_color;
	float m_rotate;
	float m_life;
	Particle(vec3 position = vec3(8.f, 0.f, 0.f), vec3 velocity = vec3(1.f, 0.f, 0.f), vec3 color = vec3(1.f, 0.f, 1.f), float m_rotate = 0.f, float life = 0.f)
	{
		m_position = position;
		m_velocity = position;
		m_color = color;
		m_life = life;
	};
};

class ParticleSystem
{
private:
	 
	//int m_nrOfParticles; Tror ej vi behöver kan bara kolla size på m_particles?
	GLuint m_VAO;
	Shader* m_particleShader;
	
public:
	vector<Particle> m_particles;
	ParticleSystem(int nrOfParticles);
	~ParticleSystem();

	void Init(int nrOfParticles);
	void Update(float dt, vec2 emitterPos);
	void Render();

	int GetDeadParticle();
	void RespawnParticle(Particle &particle, vec2 emitterPos, float dt);



};