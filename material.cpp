#include "material.h"

glm::vec3 Material::diffuse_gray( void ){ return glm::vec3{0.725f, 0.71f, 0.68f} / float(M_PI);}

glm::vec3 Material::diffuse_green( void ){ return glm::vec3{0.14f, 0.45f, 0.091f} / float(M_PI);}

glm::vec3 Material::diffuse_red( void ){ return glm::vec3{0.63f, 0.065f, 0.05f} / float(M_PI);}

glm::vec3 Material::diffuse_yellow( void ){ return glm::vec3{0.78f, 0.78f, 0.78f} / float(M_PI);}

glm::vec3 Material::diffuse_white( void ){ return glm::vec3{1.0f} / float(M_PI);}