#include "Model.h"

class Stalagmite : public Model {
public:
	Stalagmite() {
		this->model_path = "assets/Stalagmite/stalagmite.obj";
		this->textures = { "textures/Stalagmite/stalagmite_base_color.png",
			"textures/Stalagmite/stalagmite_metallic_roughness.png", "textures/Stalagmite/stalagmite_normal.png" };
		this->model_mtl = { "assets/Stalagmite/stalagmite.mtl" };
	};

	void bindTextures(GLuint g_Shader) override {
		GLuint u_texture = glGetUniformLocation(g_Shader, "u_texture");
		glUniform1i(u_texture, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_ids[0]);//index of id is the order of textures declared

		//spec
		GLuint u_texture_spec = glGetUniformLocation(g_Shader, "u_texture_spec");
		glUniform1i(u_texture_spec, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_ids[1]);

		//normal
		GLuint u_texture_normal = glGetUniformLocation(g_Shader, "u_texture_normal");
		glUniform1i(u_texture_normal, 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture_ids[2]);

		GLuint u_texture_diffuse = glGetUniformLocation(g_Shader, "u_texture_diffuse");
		glUniform1i(u_texture_diffuse, 0);
	};
};