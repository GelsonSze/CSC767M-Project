#include "Model.h"

class Clam : public Model {
public:
	Clam() {
		this->model_path = "assets/Clam/clam.obj";
		this->textures = { "textures/Clam/concha_baseColor.png", "textures/Clam/concha_normal.png"};
		this->model_mtl = { "assets/Clam/clam.mtl" };
	};
};