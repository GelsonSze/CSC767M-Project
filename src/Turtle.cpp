#include "Model.h"

class Turtle : public Model {
public:
	Turtle() {
		this->model_path = "assets/Turtle/turtle.obj";
		this->textures = { "textures/Turtle/turtle_body_diffuse.png", "textures/Turtle/turtle_body_normal.png",
			"textures/Turtle/turtle_body_specular_glossiness.png", "textures/Turtle/turtle_eyes_diffuse.png"
		};
		this->model_mtl = { "assets/Turtle/turtle.mtl" };
	};
	void bindTextures(GLuint g_Shader) override {
		GLuint u_texture_diffuse = glGetUniformLocation(g_Shader, "u_texture_diffuse");
		glUniform1i(u_texture_diffuse, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_ids[0]); //index of id is the order of textures declared

		GLuint u_texture_spec = glGetUniformLocation(g_Shader, "u_texture_normal");
		glUniform1i(u_texture_spec, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_ids[1]);

		GLuint u_texture_normal = glGetUniformLocation(g_Shader, "u_texture_spec");
		glUniform1i(u_texture_normal, 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture_ids[2]);

		GLuint u_texture = glGetUniformLocation(g_Shader, "u_texture");
		glUniform1i(u_texture, 0);
	}
};