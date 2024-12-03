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

#include "globals.h"

#include "tiny_obj_loader.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

class Skybox {
public:
	GLuint g_Vao_skybox = 0;
	GLuint g_NumTriangles_skybox = 0;
	GLuint texture_id_skybox = 0;

public: 
	Skybox() {
	};

	void load(GLuint g_SimpleShader_sky) {
		char fname_sky[] = "assets/Skybox/default.obj";
		vector<tinyobj::shape_t> shapes_sky = {};
		vector<tinyobj::material_t> mat = {};
		string err;

		bool ret_sky = tinyobj::LoadObj(shapes_sky, mat, err, fname_sky, "assets/");

		if (ret_sky)
			cout << "Skybox Obj File: " << fname_sky << " successfully loaded \n";
		else
			cout << "Error!";

		// Create VAO
		g_Vao_skybox = gl_createAndBindVAO();
		std::cout << "vao: " << g_Vao_skybox << endl;

		// Add Positions VBO
		gl_createAndBindAttribute(
			&(shapes_sky[0].mesh.positions[0]),
			shapes_sky[0].mesh.positions.size() * sizeof(float),
			g_SimpleShader_sky,
			"a_vertex",
			3
		);

		// Add Index VBO
		gl_createIndexBuffer(
			&(shapes_sky[0].mesh.indices[0]),
			shapes_sky[0].mesh.indices.size() * sizeof(unsigned int)
		);

		// Add Texture Coordinates VBO
		gl_createAndBindAttribute(
			&(shapes_sky[0].mesh.texcoords[0]),
			shapes_sky[0].mesh.texcoords.size() * sizeof(GLfloat),
			g_SimpleShader_sky,
			"a_uv",
			2
		);

		// Unbind Everything
		gl_unbindVAO();

		// Store Number of Triangles
		g_NumTriangles_skybox = shapes_sky[0].mesh.indices.size() / 3;

		// Texture Object
		int width_sky, height_sky, numChannels_sky;
		unsigned char* pixels_sky;
		char path_sky[] = "textures/Skybox/skybox_test.png";

		stbi_set_flip_vertically_on_load(true);
		pixels_sky = stbi_load(path_sky, &width_sky, &height_sky, &numChannels_sky, 0);

		glGenTextures(1, &texture_id_skybox);

		glBindTexture(GL_TEXTURE_2D, texture_id_skybox);

		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width_sky,
			height_sky,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			pixels_sky
		);
	}

	void draw(GLuint g_SimpleShader_sky, mat4 vm_sky) {	
		// activate shader
		//glUseProgram(g_SimpleShader_sky);

		//// SKYBOX-RELATED CODE
		GLuint model_loc_sky = glGetUniformLocation(g_SimpleShader_sky, "u_model");
		GLuint view_loc_sky = glGetUniformLocation(g_SimpleShader_sky, "u_view");
		GLuint projection_loc_sky = glGetUniformLocation(g_SimpleShader_sky, "u_projection");

		mat4 model_matrix_sky = translate(mat4(1.0f), cameraPos);
		//mat4 view_matrix_sky = glm::lookAt(cameraEye, cameraCenter, cameraUp);
		//mat4 projection_matrix_sky = perspective(90.0f, 1.0f, 0.1f, 50.0f);
		//mat4 projection_matrix_sky = ortho(-1.0f, 1.0f, -1.0f, 1.0f, -0.1f, 1.0f);

		glUniformMatrix4fv(model_loc_sky, 1, GL_FALSE, glm::value_ptr(model_matrix_sky));
		glUniformMatrix4fv(view_loc_sky, 1, GL_FALSE, glm::value_ptr(vm_sky));
		glUniformMatrix4fv(projection_loc_sky, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		// bind the texture
		GLuint u_texture_sky = glGetUniformLocation(g_SimpleShader_sky, "u_texture");
		glUniform1i(u_texture_sky, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id_skybox);

		gl_bindVAO(g_Vao_skybox);
		glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles_skybox, GL_UNSIGNED_INT, 0);
	}
};