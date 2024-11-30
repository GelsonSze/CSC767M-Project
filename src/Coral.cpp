#include "Model.h"

class Coral : public Model {
public:
	Coral() {
		this->model_path = "assets/Coral/coral.obj";
		this->textures = { "textures/Coral/coral_diff.png", "textures/Coral/coral_spec.png", "textures/Coral/nmap.png" };
		this->model_mtl = { "assets/Coral/coral.mtl" };
	};

	void bindTextures(GLuint g_Shader) override {
		GLuint u_texture = glGetUniformLocation(g_Shader, "u_texture");
		glUniform1i(u_texture, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_ids[0]); //index of id is the order of textures declared

		GLuint u_texture_spec = glGetUniformLocation(g_Shader, "u_texture_spec");
		glUniform1i(u_texture_spec, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_ids[1]);

		GLuint u_texture_normal = glGetUniformLocation(g_Shader, "u_texture_normal");
		glUniform1i(u_texture_normal, 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture_ids[2]);
	}
};