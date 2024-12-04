#include "Model.h"

class Trident : public Model {
public:
	Trident() {
		this->model_path = "assets/Trident/trident.obj";
		this->textures = { "textures/Trident/trident_diff.png", "textures/Trident/trident_spec.png" };
		this->model_mtl = { "assets/Trident/trident.mtl" };
	};

	void bindTextures(GLuint g_Shader) override {
		GLuint u_texture_diffuse = glGetUniformLocation(g_Shader, "u_texture_diffuse");
		glUniform1i(u_texture_diffuse, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_ids[0]); //index of id is the order of textures declared

		GLuint u_texture_spec = glGetUniformLocation(g_Shader, "u_texture_spec");
		glUniform1i(u_texture_spec, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_ids[1]);

		GLuint u_texture = glGetUniformLocation(g_Shader, "u_texture");
		glUniform1i(u_texture, 0);

		GLuint u_texture_normal = glGetUniformLocation(g_Shader, "u_texture_normal");
		glUniform1i(u_texture_normal, 0);
	}
};