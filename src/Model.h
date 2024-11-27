#pragma once
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

#include "tiny_obj_loader.h" 

using namespace std;
using namespace glm;

class Model {
protected:
	string model_path;
	vector <string> textures;
	GLuint g_SimpleShader = 0;
	GLuint g_NumTriangles = 0;
	GLuint g_Vao = 0;

public:
	Model(){};
	Model(string model_path, vector<string> textures) : model_path{model_path}, textures{textures} {};
	virtual ~Model() {
	};
	virtual void load() {

		const char* model_c_str = this->model_path.c_str();
		std::vector< tinyobj::shape_t > shapes;

		bool ret = tinyobj::LoadObj(shapes, model_c_str);

		if (ret)
			cout << "OBJ File: " << model_c_str << "successfully loaded\n";
		else
			cout << "OBJ File: " << model_c_str << "cannot be found or is not a valid OBJ File";

		Shader simpleShader("src/shader.vert", "src/shader.frag");
		g_SimpleShader = simpleShader.program;

		// Create the VAO where we store all geometry (stored in g_Vao)
		g_Vao = gl_createAndBindVAO();
		std::cout << "vao: " << g_Vao;

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
	};

	virtual void draw() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// activate shader
		glUseProgram(g_SimpleShader);

		//bind the geometry
		gl_bindVAO(g_Vao);

		// Draw to screen
		glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);

		gl_unbindVAO();
	}
};
