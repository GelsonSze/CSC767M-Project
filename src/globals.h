#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

using namespace glm;

extern float cameraPosx;
extern float cameraPosy;
extern float cameraPosz;
extern vec3 cameraPos;
extern vec3 cameraCenter;
extern vec3 cameraUp;
extern mat4 view_matrix;
extern mat4 projection_matrix;