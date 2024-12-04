/*  Base code by Alun Evans 2016 LaSalle (aevanss@salleurl.edu) modified by: Conrado Ruiz, Ferran Ruiz 2024*/

// student names: Alfonso Miguel Cruz, Gelson Sze

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
#include "Skybox.cpp"

#include "globals.h"

using namespace std;
using namespace glm;

//global variables to help us do things
int g_ViewportWidth = 512; int g_ViewportHeight = 512; // Default window size, in pixels
double mouse_x, mouse_y;	//variables storing mouse position
const vec3 g_backgroundColor(0.2f, 0.2f, 0.2f); // background colour - a GLM 3-component vector

//vec3 g_light_dir(10.0, 10.0, 10.0);
//float x = 0.0f, y = 0.0f, z = 0.0f;
//float s = 0.2, r = 0.0;
//float radius = 1.5f;

//global variables for global header
int totalObjs = 0;
int totalTextures = 0;

float cameraPosx = 0.0f, cameraPosy = 0.3f, cameraPosz = 1.1f;
//float cameraPosx = 0.0f, cameraPosy = 0.2f, cameraPosz = -2.0f;
vec3 cameraPos = vec3(cameraPosx, cameraPosy, cameraPosz);
vec3 cameraCenter = vec3(0.0f, 0.0f, 0.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

vec3 cameraFront = normalize(cameraPos - cameraCenter);
vec3 cameraSide = normalize(cross(cameraUp, cameraFront));
vec3 cameraTop = cross(cameraFront, cameraSide);
vec3 cameraDirection = vec3(0.0f, 0.0f, 0.0f);

//mat4 view_matrix = glm::lookAt(
//	cameraPos,
//	cameraCenter,
//	cameraUp
//);
mat4 projection_matrix = perspective(
	90.0f, // fov
	1.0f, // aspect ratio
	0.1f, // near plane (distance from camera)
	50.0f // far plane (distance from camera)
);


//objects
Clam clam = Clam();
Coral coral = Coral();
Fish fish1 = Fish();
Fish fish2 = Fish();
Fish fish3 = Fish();
Jellyfish jellyfish = Jellyfish();
Pearl pearl = Pearl();
Pillar pillar = Pillar();
Seaweed seaweed = Seaweed();
Shrine shrine = Shrine();
Stalagmite stalagmite = Stalagmite();
Terrain terrain = Terrain();
Trident trident = Trident();
Turtle turtle = Turtle();

Skybox skybox = Skybox();

//globals
GLuint g_SimpleShader = 0;
GLuint g_SimpleShader_sky = 0;
vector <Model*> models;
GLuint g_SimpleShader_depth = 0;
GLuint g_SimpleShader_shadow = 0;

// FPS Camera Variables
bool isDrag = false;
bool firstMouse = true;
float lastX = 256.0f;
float lastY = 256.0f;
float cam_yaw = -90.0f;
float cam_pitch = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Test Variables
GLuint texture_id_sphere = 0;
GLuint g_Vao_sphere = 0;
GLuint g_NumTriangles_sphere = 0;

//SHADOW MAPPING
unsigned int depthMapFBO;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthMap;

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

float fishXY(float t, float c) {
	return sin(c * t);
}

float fishZ(float t) {
	return cos(t);
}

void push_back_models() {
	// ANIMALS
	models.push_back(&clam);
	models.push_back(&fish1);
	models.push_back(&fish2);
	models.push_back(&fish3);
	models.push_back(&jellyfish);
	models.push_back(&turtle);

	// OBJECTS
	models.push_back(&pearl);
	models.push_back(&pillar);

	models.push_back(&stalagmite);
	models.push_back(&trident);

	// CORALS
	models.push_back(&coral);

	// TERRAIN
	models.push_back(&terrain);

	// SHRINES
	models.push_back(&shrine);

	// SEAWEEDS
	models.push_back(&seaweed);
}

void loadTest() {
	vector<tinyobj::shape_t> sphere = {};
	bool ret = tinyobj::LoadObj(sphere, "assets/Test/sphere.obj");

	gl_createAndBindAttribute(
		&(sphere[0].mesh.positions[0]),
		sphere[0].mesh.positions.size() * sizeof(float),
		g_SimpleShader,
		"a_vertex",
		3
	);

	// extract uv coords buffer from the shape to create the VBO
	gl_createAndBindAttribute(
		&(sphere[0].mesh.texcoords[0]),
		sphere[0].mesh.texcoords.size() * sizeof(GLfloat),
		g_SimpleShader,
		"a_uv",
		2
	);

	// Add Index VBO
	gl_createIndexBuffer(
		&(sphere[0].mesh.indices[0]),
		sphere[0].mesh.indices.size() * sizeof(unsigned int));

	// Add Lighting VBO
	gl_createAndBindAttribute(
		&(sphere[0].mesh.normals[0]),
		sphere[0].mesh.normals.size() * sizeof(float),
		g_SimpleShader,
		"a_normal",
		3
	);

	// unbind everything
	gl_unbindVAO();

	// store number of triangles (use in draw())
	g_NumTriangles_sphere = sphere[0].mesh.indices.size() / 3;

	stbi_set_flip_vertically_on_load(true);

	char sphere_path[] = "textures/Test/sphere.png";
	int width, height, numChannels;
	unsigned char* pixels_sphere;

	// load texture objects
	stbi_set_flip_vertically_on_load(true);

	pixels_sphere = stbi_load(sphere_path, &width, &height, &numChannels, 0);

	glGenTextures(1, &texture_id_sphere);

	glBindTexture(GL_TEXTURE_2D, texture_id_sphere);

	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR
	);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pixels_sphere
	);
}

// ------------------------------------------------------------------------------------------
// This function manually creates a square geometry (defined in the array vertices[])
// ------------------------------------------------------------------------------------------
void load()
{
	Shader simpleShader("src/shader.vert", "src/shader.frag");
	g_SimpleShader = simpleShader.program;
	
	Shader simpleShader_key("src/shader_sky.vert", "src/shader_sky.frag");
	g_SimpleShader_sky = simpleShader_key.program;

	Shader simpleShader_depth("src/shader_depth.vert", "src/shader_depth.frag");
	g_SimpleShader_depth = simpleShader_depth.program;

	push_back_models();	
	skybox.load(g_SimpleShader_sky);

	/** FOR TESTING PURPOSES ONLY!! **/
	//loadTest();
	/** FOR TESTING PURPOSES ONLY!! **/

	//**********************
	// CODE TO LOAD TO MEMORY
	//**********************
	for (auto &model : models) {
		//cout << "BEFORE LOAD" << "\n";
		model->load();
		//cout << "AFTER LOAD" << "\n";

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

	//SHADOW MAPPING
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	cout << "LOADING DONE!" << endl;
}

void drawTest() {
	GLuint model_loc = glGetUniformLocation(g_SimpleShader, "u_model");
	GLuint view_loc = glGetUniformLocation(g_SimpleShader, "u_view");
	GLuint projection_loc = glGetUniformLocation(g_SimpleShader, "u_projection");

	GLuint u_texture = glGetUniformLocation(g_SimpleShader, "u_texture");

	GLuint light_loc = glGetUniformLocation(g_SimpleShader, "u_light_dir");
	GLuint cam_pos_loc = glGetUniformLocation(g_SimpleShader, "u_cam_pos");
	GLuint ambient_loc = glGetUniformLocation(g_SimpleShader, "u_ambient");
	GLuint diffuse_loc = glGetUniformLocation(g_SimpleShader, "u_diffuse");
	GLuint specular_loc = glGetUniformLocation(g_SimpleShader, "u_specular");
	GLuint shininess_loc = glGetUniformLocation(g_SimpleShader, "u_shininess");

	mat4 model_matrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -2.0f)) *
						scale(mat4(1.0f), vec3(0.3, 0.3, 0.3));

	mat4 view_matrix = glm::lookAt(cameraPos, cameraCenter, cameraUp);

	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	glUniform3f(light_loc, 10.0f, 10.0f, 10.0f);
	glUniform3f(cam_pos_loc, cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform3f(ambient_loc, 0.1f, 0.1f, 0.1f);
	glUniform3f(diffuse_loc, 1.0f, 1.0f, 1.0f);
	glUniform3f(specular_loc, 1.0f, 1.0f, 1.0f);
	glUniform1f(shininess_loc, 40.0f);

	// bind the sampler to the texture unit 0
	glUniform1i(u_texture, 0);

	// activate texture unit i and bin the texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_sphere);

	//bind the geometry
	gl_bindVAO(g_Vao_sphere);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_sphere, GL_UNSIGNED_INT, 0);
}

// ------------------------------------------------------------------------------------------
// This function actually draws to screen and called non-stop, in a loop
// ------------------------------------------------------------------------------------------
void draw()
{
	mat4 view_matrix = glm::lookAt(
		cameraPos,
		cameraCenter,
		cameraUp
	);
	//float currentFrame = glfwGetTime();
	//deltaTime = currentFrame - lastFrame;
	//lastFrame = currentFrame;
	mat4 lightProjection, lightView;
	mat4 lightSpaceMatrix;

	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	vec3 lightPos(cameraPos.x, cameraPos.y, cameraPos.z);
	lightView = lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	//shadow mapping
	glUseProgram(g_SimpleShader_depth);
	// render depth of scene to texture (from light's perspective)
	GLuint light_loc = glGetUniformLocation(g_SimpleShader_depth, "lightSpaceMatrix");
	glUniformMatrix4fv(light_loc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, skybox.texture_id_skybox);
		//pearl.draw(g_SimpleShader_depth, view_matrix);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	glViewport(0, 0, g_ViewportWidth, g_ViewportHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	glUseProgram(g_SimpleShader_sky);
	skybox.draw(g_SimpleShader_sky, view_matrix);


	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glUseProgram(g_SimpleShader);

	GLuint view_loc = glGetUniformLocation(g_SimpleShader, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

	// this is the camera position, eye/cameraPos
	GLuint cam_pos_loc = glGetUniformLocation(g_SimpleShader, "u_cam_pos");
	glUniform3f(cam_pos_loc, cameraPos.x, cameraPos.y, cameraPos.z);
	
	GLuint light_matrix_loc = glGetUniformLocation(g_SimpleShader, "lightSpaceMatrix");
	glUniformMatrix4fv(light_matrix_loc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	//GLuint diffuseTextureloc = glGetUniformLocation(g_SimpleShader, "u_texture_diffuse");
	//glUniform1i(diffuseTextureloc, 0);

	GLuint shadowMap_loc = glGetUniformLocation(g_SimpleShader, "shadowMap");
	glUniform1i(shadowMap_loc, 1);

	// MOVEMENT 01 - Spiral Turtle
	turtle.set_modelTransform(sin(glfwGetTime()),
							  0.1 * glfwGetTime() - 1,
							  cos(glfwGetTime()),
							  0, 0, 0,
							  1.0f, 1.0f, 1.0f);
	if (glfwGetTime() > 6 * glm::pi<float>())
		glfwSetTime(0.0);
	turtle.draw(g_SimpleShader, view_matrix);

	//// MOVEMENT 02 - Jellyfish
	jellyfish.set_modelTransform(flowerX(glfwGetTime()),
								 flowerY(glfwGetTime()),
								 flowerZ(glfwGetTime()),
								 0, 180.0f, 0,
								 0.75f, 0.75f, 0.75f);
	jellyfish.draw(g_SimpleShader, view_matrix);

	//// MOVEMENT 03 - 05 - Fish 1, Fish 2, Fish 3
	fish1.set_modelTransform(fishXY(glfwGetTime(), 2),
							 fishXY(glfwGetTime(), 1),
							 fishZ(glfwGetTime()),
							 0, 0, 0,
							 0.3f, 0.3f, 0.3f);
	fish1.draw(g_SimpleShader, view_matrix);

	fish2.set_modelTransform(fishXY(glfwGetTime(), 2) - 0.1,
							 fishXY(glfwGetTime(), 1) - 0.1,
							 fishZ(glfwGetTime()) - 0.1,
							 0, 0, 0,
							 0.3f, 0.3f, 0.3f);
	fish2.draw(g_SimpleShader, view_matrix);

	fish3.set_modelTransform(fishXY(glfwGetTime(), 2) - 0.2,
							 fishXY(glfwGetTime(), 1) + 0.2,
							 fishZ(glfwGetTime()) - 0.2,
							 0, 0, 0,
							 0.3f, 0.3f, 0.3f);
	fish3.draw(g_SimpleShader, view_matrix);

	// CORAL
	// NW
	float cx = -1.4f, cy = 0.7f, cz = -1.4f;
	int i = 0;
	for (i = 0; i < 9; i++) {
		switch (i) {
			case 1:
				cz = -1.0f;
				cy = 0.4f;
				break;
			case 2:
				cx = -1.0f;
				break;
			case 3:
				cz = -1.4f;
				break;
			case 4:
				cx = -1.4f;
				cy = 0.1f;
				cz = -0.7f;
				break;
			case 5:
				cx = -1.0f;
				break;
			case 6:
				cx = -0.7f;
				break;
			case 7:
				cz = -1.0f;
				break;
			case 8:
				cz = -1.4f;
				break;
		}

		coral.set_modelTransform(cx, cy, cz,
										0.0f, 0.0f, 0.0f,
										0.05f, 0.05f, 0.05f);
		coral.draw(g_SimpleShader, view_matrix);
	}


	//NE
	cx = 1.4f;
	cy = 0.7f;
	cz = -1.4f;
	for (i = 0; i < 9; i++) {
		switch (i) {
		case 1:
			cz = -1.0f;
			cy = 0.4f;
			break;
		case 2:
			cx = 1.0f;
			break;
		case 3:
			cz = -1.4f;
			break;
		case 4:
			cx = 1.4f;
			cy = 0.1f;
			cz = -0.7f;
			break;
		case 5:
			cx = 1.0f;
			break;
		case 6:
			cx = 0.7f;
			break;
		case 7:
			cz = -1.0f;
			break;
		case 8:
			cz = -1.4f;
			break;
		}

		coral.set_modelTransform(cx, cy, cz,
			0.0f, 0.0f, 0.0f,
			0.05f, 0.05f, 0.05f);
		coral.draw(g_SimpleShader, view_matrix);
	}

	//SW
	cx = -1.4f;
	cy = 0.7f;
	cz = 1.4f;
	for (i = 0; i < 9; i++) {
		switch (i) {
		case 1:
			cz = 1.0f;
			cy = 0.4f;
			break;
		case 2:
			cx = -1.0f;
			break;
		case 3:
			cz = 1.4f;
			break;
		case 4:
			cx = -1.4f;
			cy = 0.1f;
			cz = 0.7f;
			break;
		case 5:
			cx = -1.0f;
			break;
		case 6:
			cx = -0.7f;
			break;
		case 7:
			cz = 1.0f;
			break;
		case 8:
			cz = 1.4f;
			break;
		}

		coral.set_modelTransform(cx, cy, cz,
			0.0f, 0.0f, 0.0f,
			0.05f, 0.05f, 0.05f);
		coral.draw(g_SimpleShader, view_matrix);
	}
	// TERRAIN
	float tx = -1.0f, tz = -1.0f, angle = 90.0f;
	for (i = 0; i < 3; i++) {
		switch (i) {
			case 1:
				tx *= -1.0f;
				angle = 0.0f;
				break;
			case 2:
				tx *= -1.0f;
				tz *= -1.0f;
				angle = 180.0f;
				break;
		}

		terrain.set_modelTransform(tx, 0.0f, tz,
									   0.0f, angle, 0.0f,
									   0.075f, 0.075f, 0.075f);
		terrain.draw(g_SimpleShader, view_matrix);
	}

	// SHRINES
	float sx = 0.0f, sz = -1.25f;
	// float angle = 90.0f; int i;
	angle = 90.0f;
	for (i = 0; i < 3; i++) {
		switch (i) {
		case 1:
			sx = -1.25f;
			sz = 0.0f;
			angle = 180.0f;
			break;
		case 2:
			sx = 0.0;
			sz = 1.25f;
			angle = 270.0f;
			break;
		}

		shrine.set_modelTransform(sx, 0.0f, sz,
			0.0f, angle, 0.0f,
			0.05f, 0.05f, 0.05f);
		shrine.draw(g_SimpleShader, view_matrix);
	}

	float wx = 0.75f, wz = 1.00f;
	for (i = 0; i < 7; i++) {
		switch (i) {
		case 1:
			wz = 1.25f;
			break;
		case 2:
			wz = 1.50f;
			break;
		case 3:
			wx = 1.0f;
			wz = 0.75f;
			break;
		case 4:
			wx = 1.25f;
			break;
		case 5:
			wx = 1.50f;
			break;
		case 6:
			wx = 0.75f;
			break;
		}

		seaweed.set_modelTransform(
			wx, -0.25f, wz,
			0.0f, 90.0f, 0.0f,
			0.05f, 0.05f, 0.05f);
		seaweed.draw(g_SimpleShader, view_matrix);
	}
	
	// TRIDENT
	trident.set_modelTransform(
		1.25f, 0.3f, 1.25f,
		0.0f, 45.0f, 0.0f,
		0.2f, 0.2f, 0.2f);
	trident.draw(g_SimpleShader, view_matrix);

	// STALAGMITE
	stalagmite.set_modelTransform(
		1.25f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.2f, 0.2f, 0.2f);
	stalagmite.draw(g_SimpleShader, view_matrix);

	// PILLAR AND PEARL
	pillar.set_modelTransform(0.0f, 0.1f, -0.125f,
		0.0f, 0.0f, 0.0f,
		0.0125f, 0.0125f, 0.0125f);
	pillar.draw(g_SimpleShader, view_matrix);

	// TODO: APPLY ALPHA BLENDING
	pearl.set_modelTransform(0.0f, 0.3f, -0.125f,
		0.0f, 0.0f, 0.0f,
		0.075f, 0.075f, 0.075f);
	pearl.draw(g_SimpleShader, view_matrix);

	// CLAM
	clam.set_modelTransform(
		0.0f, -0.15f, -1.20f,
		0.0f, 0.0f, 0.0f,
		1.5f, 1.5f, 1.5f);
	clam.draw(g_SimpleShader, view_matrix);
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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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


