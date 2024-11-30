#include "Model.h"

class Terrain : public Model {
public:
	Terrain() {
		this->model_path = "assets/Terrain/terrain.obj";
		this->textures = { "textures/Shrine/shrine_diff.png",
						   "textures/Shrine/shrine_spec.png",
						   "textures/Shrine/shrine_nmap.png" };
		this->model_mtl = { "assets/Terrain/terrain.mtl" };
	};

	void bindTextures(GLuint g_Shader) override {
		GLuint u_texture = glGetUniformLocation(g_Shader, "u_texture_diffuse");
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