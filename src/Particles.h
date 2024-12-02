#pragma once
#include <GL/glew.h>
class Particles
{
public:
	void SpawnParticles(GLuint num_particles);
	void Render();

//private:
	GLuint particle_vao;
	GLuint num_particles;
};

