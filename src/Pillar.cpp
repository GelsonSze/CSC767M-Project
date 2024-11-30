#include "Model.h"

class Pillar : public Model {
public:
	Pillar() {
		this->model_path = "assets/Pillar/pillar.obj";
		this->textures = { "textures/Pillar/pillar_diffuse.png", "textures/Pillar/pillar_roughness.png" };
		this->model_mtl = { "assets/Pillar/pillar.mtl" };
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