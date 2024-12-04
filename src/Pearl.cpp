#include "Model.h"

class Pearl : public Model {
public:
	Pearl() {
		this->model_path = "assets/Pearl/sphere.obj";
		this->textures = { "textures/Pearl/pearl.bmp" };
		this->model_mtl = { };
	};

	void bindTextures(GLuint g_Shader) override {
		GLuint u_texture = glGetUniformLocation(g_Shader, "u_texture");
		glUniform1i(u_texture, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_ids[0]); //index of id is the order of textures declared

		GLuint u_texture_diffuse = glGetUniformLocation(g_Shader, "u_texture_diffuse");
		glUniform1i(u_texture_diffuse, 0);

		GLuint u_texture_normal = glGetUniformLocation(g_Shader, "u_texture_normal");
		glUniform1i(u_texture_normal, 0);

		GLuint u_texture_spec = glGetUniformLocation(g_Shader, "u_texture_spec");
		glUniform1i(u_texture_spec, 0);
	}
};