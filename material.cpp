#include "material.h"

glm::vec3 Material::diffuse_gray( void ){ return glm::vec3{0.5f, 0.5f, 0.5f} / float(M_PI);}

glm::vec3 Material::diffuse_green( void ){ return glm::vec3{0.0f, 1.0f, 0.0f} / float(M_PI);}

glm::vec3 Material::diffuse_red( void ){ return glm::vec3{1.0f, 0.0f, 0.0f} / float(M_PI);}