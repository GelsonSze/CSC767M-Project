#version 330 core

in vec2 position;
in float lifespan;
in vec3 color;

out float particleLifespan;
out vec3 v_color;

void main() {
    particleLifespan = lifespan;
    v_color = color;
    gl_Position = vec4(position, 0.0, 1.0);
    gl_PointSize = 100.0; // Size of each particle
}