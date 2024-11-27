#include "Model.h"

class Clam : public Model {
public:
	Clam() {
		this->model_path = "assets/Clam/clam.obj";
		this->textures = { "textures/Clam/concha_baseColor.png", "textures/Clam/concha_normal.png"};
		this->model_mtl = { "assets/Clam/clam.mtl" };
	};


private:
	void bindTextures() override{
		GLuint u_texture = glGetUniformLocation(g_SimpleShader, "u_texture");

		glUniform1i(u_texture, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_ids[0]); //index of id is the order of textures declared

		GLuint u_texture_normal = glGetUniformLocation(g_SimpleShader, "u_texture_normal");
		glUniform1i(u_texture_normal, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_ids[1]);
	}
};