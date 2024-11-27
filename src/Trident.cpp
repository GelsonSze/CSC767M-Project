#include "Model.h"

class Trident : public Model {
public:
	Trident() {
		this->model_path = "assets/Trident/trident.obj";
		this->textures = { "textures/Trident/trident_diff.png", "textures/Trident/trident_spec.png" };
		this->model_mtl = { "assets/Trident/trident.mtl" };
	};
};