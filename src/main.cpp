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
float cameraPosx = 0.0f, cameraPosy = 0.3f, cameraPosz = 1.1f;
vec3 cameraPos = vec3(cameraPosx, cameraPosy, cameraPosz);
vec3 cameraCenter = vec3(0.0f, 0.0f, 0.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
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
string model_path = "assets/Stalagmite/stalagmite.obj";
vector <string> textures = { "textures/Stalagmite/stalagmite_base_color.png",
	"textures/Stalagmite/stalagmite_metallic_roughness.png", "textures/Stalagmite/stalagmite_normal.png" };
string model_mtl = { "assets/Stalagmite/stalagmite.mtl" };

GLuint g_SimpleShader = 0;
GLuint g_NumTriangles = 0;
GLuint g_Vao = 0;
GLuint g_Vao1 = 0;
vector <GLuint> texture_ids;
vec3 t, r, s;

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

// ------------------------------------------------------------------------------------------
// This function manually creates a square geometry (defined in the array vertices[])
// ------------------------------------------------------------------------------------------
void load()
{
	////**********************
	//// CODE TO SET OBJ
	////**********************
	//const char* model_c_str = model_path.c_str();
	//vector< tinyobj::shape_t > shapes;
	//vector<tinyobj::material_t> mat = {};
	//string err;
	//bool ret = false;
	//if (model_mtl.size() > 0) {
	//	const char* mtl_c_str = model_mtl.c_str();
	//	ret = tinyobj::LoadObj(shapes, mat, err, model_c_str, mtl_c_str);
	//	cout << "Material File: " << mtl_c_str << " exists\n";
	//}
	//else {
	//	ret = tinyobj::LoadObj(shapes, model_c_str);
	//}

	//if (ret)
	//	cout << "OBJ File: " << model_c_str << " successfully loaded\n";
	//else
	//	cout << "OBJ File: " << model_c_str << " cannot be found or is not a valid OBJ File\n";

	////**********************
	//// CODE TO LOAD TEXTURE
	////**********************
	//int width = 0, height = 0, numChannels = 0;
	//unsigned char* pixels;
	//GLuint textureid;
	//GLenum format = GL_RGBA;
	//for (auto tex : textures) {
	//	cout << "TEX IS " << tex << "\n";

	//	stbi_set_flip_vertically_on_load(true);
	//	pixels = stbi_load(tex.c_str(), &width, &height, &numChannels, 0);
	//	glGenTextures(1, &textureid);
	//	glBindTexture(GL_TEXTURE_2D, textureid);
	//	texture_ids.push_back(textureid);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//	if (pixels) {
	//		if (numChannels == 2) {
	//			format = GL_RG;
	//		}
	//		else if (numChannels == 3) {
	//			format = GL_RGB;
	//		}
	//		else if (numChannels == 4) {
	//			format = GL_RGBA;
	//		}
	//		else {
	//			std::cout << "Failed to load Texture " << tex.c_str() << "\n width: " << width <<
	//				", height: " << height << ", numChannels: " << numChannels;
	//		}
	//	}
	//	//glGenerateMipmap(GL_TEXTURE_2D);
	//	glTexImage2D(GL_TEXTURE_2D, // target
	//		0,						// level = 0, no mipmap
	//		format,					// how the data will be stored (Grayscale, RGB, RGBA)
	//		width,					// width of the image
	//		height,					// height of the image
	//		0,						// border
	//		format,					// format of original data
	//		GL_UNSIGNED_BYTE,		// type of data
	//		pixels);
	//}
	clam.load();

	//**********************
	// CODE TO LOAD TO MEMORY
	//**********************
	Shader simpleShader("src/shader.vert", "src/shader.frag");
	g_SimpleShader = simpleShader.program;

	// Create the VAO where we store all geometry (stored in g_Vao)
	g_Vao = gl_createAndBindVAO();
	std::cout << "vao: " << g_Vao;
	vector< tinyobj::shape_t > shapes = clam.shapes;
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

	//store number of triangles (use in draw())
	g_NumTriangles = shapes[0].mesh.indices.size() / 3;
}

// ------------------------------------------------------------------------------------------
// This function actually draws to screen and called non-stop, in a loop
// ------------------------------------------------------------------------------------------
void draw()
{	
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

	clam.draw();

	//bind the geometry
	gl_bindVAO(g_Vao);

	// light position
	GLuint light_loc = glGetUniformLocation(g_SimpleShader, "u_light_dir");
	glUniform3f(light_loc, 10.0f, 10.0f, 10.0f);

	GLuint cam_pos_loc = glGetUniformLocation(g_SimpleShader, "u_cam_pos");
	// this is the camera position, eye/cameraPos <- takes in global variable declared in main from globals.h
	glUniform3f(cam_pos_loc, cameraPosx, cameraPosy, cameraPosz);

	GLuint ambient_loc = glGetUniformLocation(g_SimpleShader, "u_ambient");
	glUniform3f(ambient_loc, 0.1, 0.1, 0.1); // grey shadows

	GLuint diffuse_loc = glGetUniformLocation(g_SimpleShader, "u_diffuse");
	glUniform3f(diffuse_loc, 1.0, 1.0, 1.0); // white diffuse light

	GLuint specular_loc = glGetUniformLocation(g_SimpleShader, "u_specular");
	glUniform3f(specular_loc, 1.0, 1.0, 1.0); // white specular light

	GLuint shininess_loc = glGetUniformLocation(g_SimpleShader, "u_shininess");
	glUniform1f(shininess_loc, 20.0f);

	GLuint modelLoc = glGetUniformLocation(g_SimpleShader, "u_model");
	//mat4 model = translate(mat4(1.0f), vec3(t.x, t.y, t.z)) *
	//	rotate(mat4(1.0), r.x, vec3(1.0f, 0.0f, 0.0f)) *
	//	rotate(mat4(1.0), r.y, vec3(0.0f, 1.0f, 0.0f)) *
	//	rotate(mat4(1.0), r.z, vec3(0.0f, 0.0f, 1.0f)) *
	//	scale(mat4(1.0f), vec3(s.x, s.y, s.z));
	mat4 model = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) *
		rotate(mat4(1.0), 0.0f, vec3(1.0f, 0.0f, 0.0f)) *
		rotate(mat4(1.0), 0.0f, vec3(0.0f, 1.0f, 0.0f)) *
		rotate(mat4(1.0), 0.0f, vec3(0.0f, 0.0f, 1.0f)) *
		scale(mat4(1.0f), vec3(1.7f, 1.7f, 1.7f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Draw to screen
	glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);
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
}

// ------------------------------------------------------------------------------------------
// This function is called every time you click the mouse
// ------------------------------------------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
		cout << "Left mouse down at " << mouse_x << ", " << mouse_y << endl;
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


