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
public:
	string model_path;
	vector <string> textures;
	string model_mtl;
	GLuint g_NumTriangles = 0;
	GLuint g_Vao = 0;
	vector <GLuint> texture_ids;
	vector< tinyobj::shape_t > shapes;
	vec3 t, r, s;
	vec3 light, ambience, diffuse, specular;
	float shininess;

public:
	Model() {
		set_modelTransform();
		set_light();
		set_ambience();
		set_diffuse();
		set_specular();
	};

	virtual ~Model() {};
	
	virtual void load() {
		//**********************
		// [1] CODE TO SET OBJ
		//**********************
		const char* model_c_str = this->model_path.c_str();
		
		// For objects with mtl files
		vector<tinyobj::material_t> mat = {};
		string err;

		// Load Objects
		bool ret = false;
		if (this->model_mtl.size() > 0) {
			const char* mtl_c_str = this->model_mtl.c_str();
			ret = tinyobj::LoadObj(shapes, mat, err, model_c_str, mtl_c_str);
			cout << "Material File: " << mtl_c_str << " exists\n";
		}
		else {
			ret = tinyobj::LoadObj(shapes, model_c_str);
		}

		if (ret) {
			cout << "OBJ File: " << model_c_str << " successfully loaded\n";
			totalObjs ++;
		}
		else
			cout << "OBJ File: " << model_c_str << " cannot be found or is not a valid OBJ File\n";

		g_NumTriangles = shapes[0].mesh.indices.size() / 3;

		//**********************
		// CODE TO LOAD TEXTURE
		//**********************
		int width = 0, height = 0, numChannels = 0;
		unsigned char* pixels;
		GLuint textureid;
		GLenum format = GL_RGBA;
		for (auto tex : this->textures) {
			cout << "TEX IS " << tex << "\n";
			totalTextures++;

			stbi_set_flip_vertically_on_load(true);
			pixels = stbi_load(tex.c_str(), & width, & height, & numChannels, 0);
			glGenTextures(1, &textureid);
			glBindTexture(GL_TEXTURE_2D, textureid);
			this->texture_ids.push_back(textureid);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			
			if (pixels) {
				cout << "Num channels: " << numChannels << "\n";
				switch (numChannels) {
					case 1:
						format = GL_RED;
						break;
					case 2:
						format = GL_RG;
						break;
					case 3:
						format = GL_RGB;
						break;
					case 4:
						format = GL_RGBA;
						break;
					default:
						cout << "Failed to load Texture " << tex.c_str() << "\n width: " << width <<
							", height: " << height << ", numChannels: " << numChannels;
				}
			}

			cout << "BEFORE GLTEXIMAGE2D" << "\n";
			glTexImage2D(GL_TEXTURE_2D, // target
				0,						// level = 0, no mipmap
				format,					// how the data will be stored (Grayscale, RGB, RGBA)
				width,					// width of the image
				height,					// height of the image
				0,						// border
				format,					// format of original data
				GL_UNSIGNED_BYTE,		// type of data
				pixels);
			cout << "AFTER GLTEXIMAGE2D" << "\n";
		}
	};

	virtual void draw(GLuint g_Shader, mat4 vm, mat4 pm) {
		gl_bindVAO(g_Vao);
		GLuint u_texture = glGetUniformLocation(g_Shader, "u_texture");
		//FOR DEBUG
		/*cout << "PRINTING IDS\n";
		for (auto id : texture_ids) {
			cout << id << " ";
		}
		cout << "\n";*/

		bindTextures(g_Shader);

		// light position
		GLuint light_loc = glGetUniformLocation(g_Shader, "u_light_dir");
		glUniform3f(light_loc, light.x, light.y, light.z);

		GLuint cam_pos_loc = glGetUniformLocation(g_Shader, "u_cam_pos");
		// this is the camera position, eye/cameraPos <- takes in global variable declared in main from globals.h
		glUniform3f(cam_pos_loc, cameraPosx, cameraPosy, cameraPosz);

		GLuint ambient_loc = glGetUniformLocation(g_Shader, "u_ambient");
		glUniform3f(ambient_loc, ambience.x, ambience.y, ambience.z); // grey shadows

		GLuint diffuse_loc = glGetUniformLocation(g_Shader, "u_diffuse");
		glUniform3f(diffuse_loc, diffuse.x, diffuse.y, diffuse.z); // white diffuse light

		GLuint specular_loc = glGetUniformLocation(g_Shader, "u_specular");
		glUniform3f(specular_loc, specular.x, specular.y, specular.z); // white specular light

		GLuint shininess_loc = glGetUniformLocation(g_Shader, "u_shininess");
		glUniform1f(shininess_loc, shininess);

		GLuint modelLoc = glGetUniformLocation(g_Shader, "u_model");
		mat4 model = translate(mat4(1.0f), vec3(t.x, t.y, t.z)) *
			rotate(mat4(1.0), r.x, vec3(1.0f, 0.0f, 0.0f)) *
			rotate(mat4(1.0), r.y, vec3(0.0f, 1.0f, 0.0f)) *
			rotate(mat4(1.0), r.z, vec3(0.0f, 0.0f, 1.0f)) *
			scale(mat4(1.0f), vec3(s.x, s.y, s.z));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		GLuint view_loc = glGetUniformLocation(g_Shader, "u_view");
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(vm));

		GLuint projection_loc = glGetUniformLocation(g_Shader, "u_projection");
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(pm));

		// Draw to screen
		glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);
	}

	//xyz values that starts with t is for translate while r is for rotate, s is for scale
	virtual void set_modelTransform(float tx = 0.0f, float ty = 0.0f, float tz = 0.0f, 
								float rx = 0.0f, float ry = 0.0f, float rz = 0.0f,
								float sx = 1.7f, float sy = 1.7f, float sz = 1.7f) {
		this->t = vec3(tx, ty, tz);
		this->r = vec3(rx, ry, rz);
		this->s = vec3(sx, sy, sz);
	};

	virtual void set_light(float x = 10.0f, float y = 10.0f, float z = 10.0f) {
		this->light = vec3(x, y, z);
	};

	virtual void set_ambience(float x = 0.1f, float y = 0.1f, float z = 0.1f) {
		this->ambience = vec3(x, y, z);
	};

	virtual void set_diffuse(float x = 1.0f, float y = 1.0f, float z = 1.0f) {
		this->diffuse = vec3(x, y, z);
	};

	virtual void set_specular(float x = 1.0f, float y = 1.0f, float z = 1.0f) {
		this->specular = vec3(x, y, z);
	};

	virtual void set_shininess(float shininess = 20.f) {
		this->shininess = shininess;
	};

	virtual void bindTextures(GLuint g_Shader) {};
};