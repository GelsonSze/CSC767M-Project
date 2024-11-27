#include "Model.h"

class Pearl : public Model {
public:
	Pearl() {
		this->model_path = "assets/Pearl/sphere.obj";
		this->textures = { "textures/Pearl/pearl_texture.png" };
		this->model_mtl = { "" };
	};
};