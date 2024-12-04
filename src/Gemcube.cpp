#include "Model.h"

class Gemcube : public Model {
public:
	Gemcube() {
		this->model_path = "assets/Gemcube/default.obj";
		this->textures = { "textures/Gemcube/sapphire.png" };
		this->model_mtl = { "assets/Gemcube/default.mtl" };
	};

	virtual void draw(GLuint g_Shader, mat4 vm) override {
		//gl_bindVAO(g_Vao);

		activateTextures(g_Shader);
		bindTextures(g_Shader);
		deactivateTextures(g_Shader);

		GLuint modelLoc = glGetUniformLocation(g_Shader, "u_model");
		mat4 model = translate(mat4(1.0f), vec3(t.x, t.y, t.z)) *
			rotate(mat4(1.0), r.x, vec3(1.0f, 0.0f, 0.0f)) *
			rotate(mat4(1.0), r.y, vec3(0.0f, 1.0f, 0.0f)) *
			rotate(mat4(1.0), r.z, vec3(0.0f, 0.0f, 1.0f)) *
			scale(mat4(1.0f), vec3(s.x, s.y, s.z));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		GLuint view_loc = glGetUniformLocation(g_Shader, "u_view");
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(vm));

		GLuint projection_loc = glGetUniformLocation(g_Shader, "u_projection");
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		// Draw to screen
		gl_bindVAO(g_Vao);
		glDrawElements(GL_TRIANGLES, 3 * g_NumTriangles, GL_UNSIGNED_INT, 0);
	}

	void bindTextures(GLuint g_Shader) override {
		GLuint u_texture = glGetUniformLocation(g_Shader, "u_texture");
		glUniform1i(u_texture, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_ids[0]); //index of id is the order of textures declared
	}
};