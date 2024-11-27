#include "Model.h"

class Fish : public Model {
public:
	Fish() {
		this->model_path = "assets/Fish/fish.obj";
		this->textures = { "textures/Fish/fish_baseColor.png", "textures/Fish/fish_metallicRoughness.png", "textures/Fish/fish_normal.png" };
		this->model_mtl = { "assets/Fish/fish.mtl" };
	};
};