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
};