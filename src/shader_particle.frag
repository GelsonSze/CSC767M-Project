#version 330 core

in float particleLifespan;
in vec3 color;
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.843, 0.0, particleLifespan); // Gold color with alpha based on lifespan
}