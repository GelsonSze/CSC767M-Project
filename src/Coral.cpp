#include <random>
#include "Model.h"

class Coral : public Model {
public:
	Coral() {
		this->model_path = "assets/Coral/coral.obj";
		this->textures = { "textures/Coral/coral_diff_01.png",
						   "textures/Coral/coral_spec.png",
						   "textures/Coral/coral_nmap.png" };
		this->model_mtl = { "assets/Coral/coral.mtl" };
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

		GLuint u_texture_normal = glGetUniformLocation(g_Shader, "u_texture_normal");
		glUniform1i(u_texture_normal, 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture_ids[2]);

		GLuint u_texture = glGetUniformLocation(g_Shader, "u_texture");
		glUniform1i(u_texture, 0);

	}

	string getRandomCoralTexture() {
		vector<string> textures_list = { "textures/Coral/coral_diff_01.png" ,
						   "textures/Coral/coral_diff_02.png" ,
						   //"textures/Coral/coral_diff_03.png",
						   "textures/Coral/coral_diff_04.png" };

		std::random_device randomizer;
		std::mt19937 gen(randomizer());
		std::uniform_int_distribution<> dis(0, 3);

		return textures_list[dis(gen)];
	}
};