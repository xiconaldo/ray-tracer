#include "material.h"

glm::vec3 Material::diffuse( const glm::vec3& reflectance ){ 
	return reflectance / float(M_PI);
}

glm::vec3 Material::specular( const glm::vec3& reflectance ){ 
	return glm::vec3(1.0f);
}

glm::vec3 Material::brdf( void ){
	return brdf_pointer(reflectance_);
}

glm::vec3 Material::btdf( void ){
	return btdf_pointer(reflectance_);
}