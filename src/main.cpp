/*  Base code by Alun Evans 2016 LaSalle (aevanss@salleurl.edu) modified by: Conrado Ruiz, Ferran Ruiz 2024*/

// student name: Gelson Sze

//include some standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

//include OpenGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

//include some custom code files
#include "glfunctions.h"	//include all OpenGL stuff
#include "Shader.h"			// class to compile shaders

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h" 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Clam.cpp"
#include "Coral.cpp"
#include "Fish.cpp"
#include "Jellyfish.cpp"
#include "Pearl.cpp"
#include "Pillar.cpp"
#include "Seaweed.cpp"
#include "Stalagmite.cpp"
#include "Trident.cpp"
#include "Turtle.cpp"
#include "Seafloor.cpp"
#include "Terrain.cpp"
#include "Shrine.cpp"

#include "globals.h"

using namespace std;
using namespace glm;

//global variables to help us do things
int g_ViewportWidth = 512; int g_ViewportHeight = 512; // Default window size, in pixels
double mouse_x, mouse_y;	//variables storing mouse position
const vec3 g_backgroundColor(0.2f, 0.2f, 0.2f); // background colour - a GLM 3-component vector


//global variables for global header
int totalObjs = 0;
int totalTextures = 0;

float cameraPosx = 0.0f, cameraPosy = 0.3f, cameraPosz = 1.1f;
vec3 cameraPos = vec3(cameraPosx, cameraPosy, cameraPosz);
vec3 cameraCenter = vec3(0.0f, 0.0f, 0.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

vec3 cameraFront = normalize(cameraPos - cameraCenter);
vec3 cameraSide = normalize(cross(cameraUp, cameraFront));
vec3 cameraTop = cross(cameraFront, cameraSide);
vec3 cameraDirection = vec3(0.0f, 0.0f, 0.0f);

mat4 view_matrix = glm::lookAt(
	cameraPos,
	cameraCenter,
	cameraUp
);
mat4 projection_matrix = perspective(
	90.0f, // fov
	1.0f, // aspect ratio
	0.1f, // near plane (distance from camera)
	50.0f // far plane (distance from camera)
);


//objects
Clam clam = Clam();
Coral coral = Coral();
Fish fish = Fish();
Jellyfish jellyfish = Jellyfish();
Pearl pearl = Pearl();
Pillar pillar = Pillar();
Seaweed seaweed = Seaweed();
Stalagmite stalagmite = Stalagmite();
Trident trident = Trident();
Turtle turtle = Turtle();
Seafloor seafloor = Seafloor();
Terrain terrain = Terrain();
Shrine shrine = Shrine();

//globals
GLuint g_SimpleShader = 0;
vector <Model*> models;

// FPS Camera Variables
bool isDrag = false;
bool firstMouse = true;
float lastX = 256.0f;
float lastY = 256.0f;
float cam_yaw = -90.0f;
float cam_pitch = 0.0f;

void updateCameraVectors(float x) {
	cameraFront = normalize(x * (cameraPos - cameraCenter));
	cameraSide = normalize(cross(cameraUp, cameraFront));
	cameraTop = cross(cameraFront, cameraSide);

	//cout << cameraPos.x << " , " << cameraPos.y << " , " << cameraPos.z << endl;
	//cout << cameraCenter.x << " , " << cameraCenter.y << " , " << cameraCenter.z << endl;
	//cout << cameraUp.x << " , " << cameraUp.y << " , " << cameraUp.z << endl;
}

float flowerX(float t) {
	return (0.5 * cos(t)) + (0.1 * cos(5 * t) * cos(t));
}

float flowerY(float t) {
	return (0.5 * sin(t)) + (0.1 * cos(5 * t) * sin(t));
}

float flowerZ(float t) {
	return ((-0.13 - (0.25 * flowerX(t)) + (0.25 * flowerY(t))) / -0.25) - 0.5;
}

//Particles
//code gotten from https://learnopengl.com/In-Practice/2D-Game/Particles 
//it is code for a 2d game which contains stuff that is abstracted so u have to adjust the code manually
struct Particle {
	glm::vec2 position, velocity;
	glm::vec4 color;
	float     life;

	Particle()
		: position(1.0f, 0.0f), velocity(0.0f), color(1.0f, 0.0f, 0.0f, 1.0f), life(30.0f) { }
};
unsigned int nr_particles = 500;
vector<Particle> particles;
unsigned int lastUsedParticle = 0;
GLuint g_ParticleShader = 0;
GLuint particle_vao = 0;

//time for particles
float currentFrame = 0.0f;
float lastFrame = 0.0f;
float deltaTime = 0.0f;
GLuint particleid;

// ------------------------------------------------------------------------------------------
// This function manually creates a square geometry (defined in the array vertices[])
// ------------------------------------------------------------------------------------------
void load()
{
	//models.push_back(&clam);
	//models.push_back(&coral);
	//models.push_back(&fish);
	//models.push_back(&jellyfish);
	//models.push_back(&pearl);
	//models.push_back(&pillar);
	//models.push_back(&seaweed);
	//models.push_back(&stalagmite);
	//models.push_back(&trident);
	//models.push_back(&shrine);
	//models.push_back(&terrain);
	//models.push_back(&seafloor);
	models.push_back(&turtle);

	//**********************
	// CODE TO LOAD TO MEMORY
	//**********************
	Shader simpleShader("src/shader.vert", "src/shader.frag");
	g_SimpleShader = simpleShader.program;

	Shader particleShader("src/shader_particle.vert", "src/shader_particle.frag");
	g_ParticleShader = particleShader.program;

	// Create the VAO where we store all geometry (stored in g_Vao)
	for (auto &model : models) {
		cout << "BEFORE LOAD" << "\n";
		model->load();
		cout << "AFTER LOAD" << "\n";
		model->g_Vao = gl_createAndBindVAO();
		std::cout << "vao: " << model->g_Vao << "\n";
		vector< tinyobj::shape_t > shapes = model->shapes;
		//create vertex buffer for positions, colors, and indices, and bind them to shader
		gl_createAndBindAttribute(&(shapes[0].mesh.positions[0]),
			shapes[0].mesh.positions.size() * sizeof(float),
			simpleShader.program, "a_vertex", 3);

		gl_createAndBindAttribute(
			&(shapes[0].mesh.texcoords[0]),
			shapes[0].mesh.texcoords.size() * sizeof(GLfloat),
			simpleShader.program,
			"a_uv", 2);

		gl_createAndBindAttribute(
			&(shapes[0].mesh.normals[0]),
			shapes[0].mesh.normals.size() * sizeof(float),
			simpleShader.program,
			"a_normal", 3);

		gl_createIndexBuffer(&(shapes[0].mesh.indices[0]),
			shapes[0].mesh.indices.size() * sizeof(unsigned int));

		//unbind everything
		gl_unbindVAO();
	}

	for (unsigned int i = 0; i < nr_particles; ++i)
		particles.push_back(Particle());

	particle_vao = gl_createAndBindVAO();
	gl_unbindVAO();

	//int width = 0, height = 0, numChannels = 0;
	//unsigned char* pixels;

	//glGenTextures(1, &particleid);
	//stbi_set_flip_vertically_on_load(true);
	//pixels = stbi_load("textures/Particles/particle.png", &width, &height, &numChannels, 0);
	//GLenum format = GL_RGB;
	//if (pixels) {
	//	cout << "Num channels: " << numChannels << "\n";
	//	if (numChannels == 1) {
	//		format = GL_RED;
	//	}
	//	else if (numChannels == 2) {
	//		format = GL_RG;
	//	}
	//	else if (numChannels == 3) {
	//		format = GL_RGB;
	//	}
	//	else if (numChannels == 4) {
	//		format = GL_RGBA;
	//	}
	//	else {
	//		cout << "Failed to load Particle Texture" << "\n width: " << width <<
	//			", height: " << height << ", numChannels: " << numChannels;
	//	}
	//}
	//glBindTexture(GL_TEXTURE_2D, particleid);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, // target
	//	0,						// level = 0, no mipmap
	//	format,					// how the data will be stored (Grayscale, RGB, RGBA)
	//	width,					// width of the image
	//	height,					// height of the image
	//	0,						// border
	//	format,					// format of original data
	//	GL_UNSIGNED_BYTE,		// type of data
	//	pixels);
}

unsigned int FirstUnusedParticle();
void RespawnParticle(Particle& particle, Model& model);
// ------------------------------------------------------------------------------------------
// This function actually draws to screen and called non-stop, in a loop
// ------------------------------------------------------------------------------------------
void draw()
{
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// activate shader
	glUseProgram(g_SimpleShader);

	GLuint view_loc = glGetUniformLocation(g_SimpleShader, "u_view");

	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

	//projection matrix
	GLuint projection_loc = glGetUniformLocation(g_SimpleShader, "u_projection");

	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));


	//bind the geometry
	//trident.set_modelTransform(2.0f, -7.0f, -3.5f, 0, 0, 0, 1.0f, 1.0f, 1.0f);
	//trident.draw(g_SimpleShader);
	//turtle.set_modelTransform(0, 0.2f, 0.9f, 0, 180.0f, 0, 0.3f, 0.3f, 0.3f);
	 
	// MOVEMENT 02 - Flower Jellyfish
	//turtle.set_modelTransform(flowerX(glfwGetTime()), flowerY(glfwGetTime()), flowerZ(glfwGetTime()), 0, 180.0f, 0, 0.3f, 0.3f, 0.3f);


	// MOVEMENT 01 - Spiral Turtle
	//turtle.set_modelTransform(sin(glfwGetTime()), 0.1 * glfwGetTime() - 1, cos(glfwGetTime()), 0, 180.0f, 0, 0.3f, 0.3f, 0.3f);
	//if (glfwGetTime() > 6 * 3.141529)
	//	glfwSetTime(0.0);


	// MOVEMENT 03 - 05 - Fish 1, Fish 2, Fish 3 TODO

	//turtle.draw(g_SimpleShader);

	//coral.set_modelTransform(0.0f, 0.0f, 0.0f, 0.0f, 45.0f, 0.0f, 0.1f, 0.1f, 0.1f);
	//coral.draw(g_SimpleShader);

	//shrine.set_modelTransform(0.0f, 0.0f, 0.0f, 0.0f, 45.0f, 0.0f, 0.05f, 0.05f, 0.05f);
	//shrine.draw(g_SimpleShader);

	//terrain.set_modelTransform(0.0f, 0.0f, 0.0f, 0.0f, 45.0f, 0.0f, 0.05f, 0.05f, 0.05f);
	//terrain.draw(g_SimpleShader);
	
	//jellyfish.set_modelTransform(0.0f, 0.0f, 0.f, 0.0f, 0.0f, 0.0f, 0.01f, 0.01f, 0.01f);
	turtle.set_modelTransform(0.5f, 0.5f);
	turtle.draw(g_SimpleShader);

	unsigned int nr_new_particles = 2;
	// add new particles
	for (unsigned int i = 0; i < nr_new_particles; ++i)
	{
		int unusedParticle = FirstUnusedParticle();
		RespawnParticle(particles[unusedParticle], pearl);
	}
	// update all particles
	for (unsigned int i = 0; i < nr_particles; ++i)
	{
		Particle& p = particles[i];
		p.life -= deltaTime; // reduce life
		if (p.life > 0.0f)
		{	// particle is alive, thus update
			p.position -= p.velocity * deltaTime;
			p.color.a -= deltaTime * 2.5f;
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_POINT_SIZE);
	glUseProgram(g_ParticleShader);
	for (Particle particle : particles)
	{
		if (particle.life > 0.0f)
		{
			gl_bindVAO(particle_vao);
			//GLuint projection_loc = glGetUniformLocation(g_ParticleShader, "projection");
			//glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
			GLuint position_loc = glGetUniformLocation(g_ParticleShader, "position");
			glUniform2f(position_loc, particle.position.x, particle.position.y);  // Set x and y position

			GLuint life_loc = glGetUniformLocation(g_ParticleShader, "lifespan");
			glUniform1f(life_loc, particle.life);  // Set lifespan

			GLuint particle_color_loc = glGetUniformLocation(g_ParticleShader, "color");
			glUniform3f(particle_color_loc, particle.color.r, particle.color.g, particle.color.b);  // Set color
	
			//GLuint sprite = glGetUniformLocation(g_ParticleShader, "sprite");
			//glUniform1i(sprite, 0);

			//glActiveTexture(GL_TEXTURE10);
			//glBindTexture(GL_TEXTURE_2D, particleid);

			glBindVertexArray(particle_vao);
			glDrawArrays(GL_POINTS, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

unsigned int FirstUnusedParticle()
{
	// search from last used particle, this will usually return almost instantly
	for (unsigned int i = lastUsedParticle; i < nr_particles; ++i) {
		if (particles[i].life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// otherwise, do a linear search
	for (unsigned int i = 0; i < lastUsedParticle; ++i) {
		if (particles[i].life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// override first particle if all others are alive
	lastUsedParticle = 0;
	return 0;
};

void RespawnParticle(Particle& particle, Model& model)
{
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.position = vec2(model.t.x, model.t.y) + random;
	particle.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	particle.life = 3.0f;
	particle.velocity = vec2(((rand() % 100) - 50) / 10.0f, ((rand() % 100) - 50) / 10.0f);
}

// ------------------------------------------------------------------------------------------
// This function is called every time you press a screen
// ------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //quit
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
	//reload
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		load();

	// FPS Camera Commands
	if (key == GLFW_KEY_W && (action == GLFW_PRESS | GLFW_REPEAT)) {
		std::cout << "Press: W" << endl;

		cameraPos -= (cameraFront / 100.0f);
		updateCameraVectors(1.0f);
	}

	if (key == GLFW_KEY_A && (action == GLFW_PRESS | GLFW_REPEAT)) {
		cout << "Press: A" << endl;

		if (all(equal(cameraFront, cameraUp))) {
			cameraSide = vec3(1e-5f, 1e-5f, 1e-5f);
		}

		cameraPos -= (cameraSide / 100.0f);
		cameraCenter -= (cameraSide / 100.0f);
		updateCameraVectors(1.0f);
	}

	if (key == GLFW_KEY_S && (action == GLFW_PRESS | GLFW_REPEAT)) {
		cout << "Press: S" << endl;

		cameraPos -= (cameraFront / 100.0f);
		updateCameraVectors(-1.0f);
	}

	if (key == GLFW_KEY_D && (action == GLFW_PRESS | GLFW_REPEAT)) {
		cout << "Press: D" << endl;

		if (all(equal(cameraFront, cameraUp))) {
			cameraSide = vec3(1e-5f, 1e-5f, 1e-5f);
		}

		cameraPos += (cameraSide / 100.0f);
		cameraCenter += (cameraSide / 100.0f);
		updateCameraVectors(1.0f);
	}
}

// ------------------------------------------------------------------------------------------
// This function is called every time you click the mouse
// ------------------------------------------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		isDrag = true;
		cout << "Left mouse down at " << mouse_x << ", " << mouse_y << endl;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		isDrag = false;
		cout << "Left mouse up at " << mouse_x << ", " << mouse_y << endl;
	}
}

void mouse_callback(GLFWwindow* window, double x, double y) {
	float xoffset, yoffset;
	float sensitivity = 0.01f;

	if (isDrag) {
		if (firstMouse) {
			lastX = x;
			lastY = y;
			firstMouse = false;
		}

		xoffset = x - lastX;
		yoffset = lastY - y;

		lastX = x;
		lastY = y;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		cam_yaw += xoffset;
		cam_pitch += yoffset;

		if (cam_pitch > 89.0f)
			cam_pitch = 89.0f;

		if (cam_pitch < -89.0f)
			cam_pitch = -89.0f;

		cameraCenter = normalize(vec3(
			cos(cam_yaw) * cos(cam_pitch),
			sin(cam_pitch),
			sin(cam_yaw) * cos(cam_pitch)
		));

		cameraDirection = cameraPos;
		updateCameraVectors(1.0f);
	}
}

int main(void)
{
	//setup window and boring stuff, defined in glfunctions.cpp
	GLFWwindow* window;
	if (!glfwInit())return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(g_ViewportWidth, g_ViewportHeight, "Hello OpenGL!", NULL, NULL);
	if (!window) {glfwTerminate();	return -1;}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	//input callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	glClearColor(g_backgroundColor.x, g_backgroundColor.y, g_backgroundColor.z, 1.0f);

	//load all the resources
	load();

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
		draw();
        // Swap front and back buffers
        glfwSwapBuffers(window);
        
        // Poll for and process events
        glfwPollEvents();
        
        //mouse position must be tracked constantly (callbacks do not give accurate delta)
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
    }

    //terminate glfw and exit
    glfwTerminate();
    return 0;
}


