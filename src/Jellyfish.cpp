#include "Model.h"

class Jellyfish : public Model {
public:
	Jellyfish() {
		this->model_path = "assets/Jellyfish/jellyfish.obj";
		this->textures = { "textures/Jellyfish/jellyfish_material_diffuse.png", "textures/Coral/jellyfish_material_specularGlossiness.png"};
		this->model_mtl = { "assets/Jellyfish/jellyfish.mtl" };
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