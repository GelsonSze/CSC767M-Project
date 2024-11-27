#include "Model.h"

class Jellyfish : public Model {
public:
	Jellyfish() {
		this->model_path = "assets/Jellyfish/jellyfish.obj";
		this->textures = { "textures/Jellyfish/jellyfish_material_diffuse.png", "textures/Coral/jellyfish_material_specularGlossiness.png"};
		this->model_mtl = { "assets/Jellyfish/jellyfish.mtl" };
	};
};