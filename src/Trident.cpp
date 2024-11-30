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
	}
};