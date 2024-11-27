#include "Model.h"

class Stalagmite : public Model {
public:
	Stalagmite() {
		this->model_path = "assets/Stalagmite/stalagmite.obj";
		this->textures = { "textures/Stalagmite/stalagmite_base_color.png", 
			"textures/Stalagmite/stalagmite_metallic_roughness.png", "textures/Stalagmite/stalagmite_normal.png" };
		this->model_mtl = { "assets/Stalagmite/stalagmite.mtl" };
	};
};