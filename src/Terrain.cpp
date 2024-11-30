#include "Model.h"

class Terrain : public Model {
public:
	Terrain() {
		this->model_path = "assets/Terrain/terrain.obj";
		this->textures = { "textures/Terrain/seafloor.bmp" }; // TODO
		this->model_mtl = { "assets/Terrain/terrain.mtl" };
	};

	void bindTextures(GLuint g_Shader) override {
		GLuint u_texture = glGetUniformLocation(g_Shader, "u_texture");

		glUniform1i(u_texture, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_ids[0]); //index of id is the order of textures declared

		GLuint u_texture_normal = glGetUniformLocation(g_Shader, "u_texture_normal");
		glUniform1i(u_texture_normal, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_ids[1]);
	}
};