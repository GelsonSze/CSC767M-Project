#include "Model.h"

class Seaweed : public Model {
public:
	Seaweed() {
		this->model_path = "assets/Seaweed/seaweed.obj";
		this->textures = { "textures/Seaweed/seaweed.png" };
		this->model_mtl = { "assets/Seaweed/seaweed.mtl" };
	};

	void bindTextures(GLuint g_Shader) override {
		GLuint u_texture = glGetUniformLocation(g_Shader, "u_texture");
		glUniform1i(u_texture, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_ids[0]);
	}
};