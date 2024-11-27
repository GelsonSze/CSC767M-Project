#include "Model.h"

class Pillar : public Model {
public:
	Pillar() {
		this->model_path = "assets/Pillar/pillar.obj";
		this->textures = { "textures/Pillar/pillar_diffuse.png", "textures/Pillar/pillar_roughness.png" };
		this->model_mtl = { "assets/Pillar/pillar.mtl" };
	};
};