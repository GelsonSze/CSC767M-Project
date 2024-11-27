//include some standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include "Model.h"

class Pearl : public Model {
public:
	Pearl() {
		this->model_path = "assets/Pearl/sphere.obj";
		this->textures = { "textures/Pearl/pearl_texture.png" };
	};
};