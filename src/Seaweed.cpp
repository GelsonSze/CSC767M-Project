#include "Model.h"

class Seaweed : public Model {
public:
	Seaweed() {
		this->model_path = "assets/Seaweed/seaweed.obj";
		this->textures = { };
		this->model_mtl = { "assets/Seaweed/seaweed.mtl" };
	};
};