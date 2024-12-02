#include "Particles.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "glfunctions.h"

using namespace std;
// code gotten from https://www.youtube.com/watch?v=MBQ6DGvKWW0&t=1806s
void Particles::SpawnParticles(GLuint num_particles) {
	num_particles = num_particles;
	vector<GLfloat> particle_positions;
	
	GLfloat minimiser = 2.0f / num_particles;

	for (size_t i = 0; i < num_particles; i++) {
		particle_positions.push_back(-1.0f + i * minimiser);
		particle_positions.push_back(sin( i * minimiser));
	}
	
	//particle_vao = gl_createAndBindVAO();
	glGenVertexArrays(1, &particle_vao);
	glBindVertexArray(particle_vao);

	GLuint particle_vbo;
	glGenBuffers(1, &particle_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, particle_vbo);
	glBufferData(GL_ARRAY_BUFFER, particle_positions.size() * sizeof(GLfloat), particle_positions.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexArrayAttrib(particle_vao, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Particles::Render() {
	glBindVertexArray(particle_vao);
	glDrawArrays(GL_POINTS, 0, num_particles);
	glBindVertexArray(0);
}