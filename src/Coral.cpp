#include "Model.h"

class Coral : public Model {
public:
	Coral() {
		this->model_path = "assets/Coral/coral.obj";
		this->textures = { "textures/Coral/coral_diff.png", "textures/Coral/coral_spec.png", "textures/Coral/nmap.png" };
		this->model_mtl = { "assets/Coral/coral.mtl" };
	};
};