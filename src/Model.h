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

using namespace std;
using namespace glm;


class Model {
protected:
	string model_path;
	vector <string> textures;
	string model_mtl;
	GLuint g_SimpleShader = 0;
	GLuint g_NumTriangles = 0;
	GLuint g_Vao = 0;
	vector <GLuint> texture_ids;
	vec3 t, r, s;
	GLuint earth_texture_id;
	GLuint texture_id_normal, texture_id_spec;

public:
	Model(){};
	virtual ~Model() {
	};
	virtual void load() {

		//**********************
		// CODE TO SET OBJ
		//**********************
		const char* model_c_str = this->model_path.c_str();
		vector< tinyobj::shape_t > shapes;
		vector<tinyobj::material_t> mat = {};
		string err;
		bool ret = false;
		if (this->model_mtl.size() > 0) {
			const char* mtl_c_str = this->model_mtl.c_str();
			ret = tinyobj::LoadObj(shapes, mat, err, model_c_str, mtl_c_str);
			cout << "Material File: " << mtl_c_str << " exists\n";
		}
		else {
			ret = tinyobj::LoadObj(shapes, model_c_str);
		}

		if (ret)
			cout << "OBJ File: " << model_c_str << " successfully loaded\n";
		else
			cout << "OBJ File: " << model_c_str << " cannot be found or is not a valid OBJ File\n";

		//**********************
		// CODE TO LOAD TEXTURE
		//**********************
		int width = 0, height = 0, numChannels = 0;
		unsigned char* pixels;
		GLuint textureid;
		GLenum format = GL_RGBA;
		for (auto tex : this->textures) {
			cout << "TEX IS " << tex << "\n";

			stbi_set_flip_vertically_on_load(true);
			pixels = stbi_load(tex.c_str(), & width, & height, & numChannels, 0);
			glGenTextures(1, &textureid);
			glBindTexture(GL_TEXTURE_2D, textureid);
			this->texture_ids.push_back(textureid);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			
			if (pixels) {
				if (numChannels == 2) {
					format = GL_RG;
				}
				else if (numChannels == 3) {
					format = GL_RGB;
				}
				else if (numChannels == 4) {
					format = GL_RGBA;
				}
				else {
					std::cout << "Failed to load Texture " << tex.c_str() << "\n width: " << width << 
						", height: " << height << ", numChannels: " << numChannels;
				}
			}
			//glGenerateMipmap(GL_TEXTURE_2D);
			glTexImage2D(GL_TEXTURE_2D, // target
				0,						// level = 0, no mipmap
				format,					// how the data will be stored (Grayscale, RGB, RGBA)
				width,					// width of the image
				height,					// height of the image
				0,						// border
				format,					// format of original data
				GL_UNSIGNED_BYTE,		// type of data
				pixels);
		}

		//**********************
		// CODE TO LOAD TO MEMORY
		//**********************
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

		//bind the textures separately per object class
		GLuint u_texture = glGetUniformLocation(g_SimpleShader, "u_texture");

		bindTextures();

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
		mat4 model = translate(mat4(1.0f), vec3(t.x, t.y, t.z)) *
			rotate(mat4(1.0), r.x, vec3(1.0f, 0.0f, 0.0f)) *
			rotate(mat4(1.0), r.y, vec3(0.0f, 1.0f, 0.0f)) *
			rotate(mat4(1.0), r.z, vec3(0.0f, 0.0f, 1.0f)) *
			scale(mat4(1.0f), vec3(s.x, s.y, s.z));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Draw to screen
		glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);
	}

	virtual void bindTextures() {};

	//xyz values that starts with t is for translate while r is for rotate, s is for scale
	virtual void set_modelTransform(float tx = 0.0f, float ty = 0.0f, float tz = 0.0f, 
								float rx = 0.0f, float ry = 0.0f, float rz = 0.0f,
								float sx = 1.7f, float sy = 1.7f, float sz = 1.7f) {
		this->t = vec3(tx, ty, tz);
		this->r = vec3(rx, ry, rz);
		this->s = vec3(sx, sy, sz);
	};
};