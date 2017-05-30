#include "material.h"

glm::vec3 Material::diffuse( const glm::vec3& reflectance,
						     const float roughness,
							 const glm::vec3& n,
						     const glm::vec3& wi,
						     const glm::vec3& wo ){
	return reflectance / float(M_PI);
}

glm::vec3 Material::specular( const glm::vec3& reflectance,
						  	  const float roughness,
							  const glm::vec3& n,
						  	  const glm::vec3& wi,
						  	  const glm::vec3& wo ){
	return glm::vec3(1.0f);
}

glm::vec3 Material::glossy(   const glm::vec3& reflectance,
							  const float roughness,
							  const glm::vec3& n,
							  const glm::vec3& wi,
							  const glm::vec3& wo){

	float m = roughness;
	glm::vec3 Ro = reflectance;
	glm::vec3 h = glm::normalize( (wi+wo)*0.5f );
	float nh = glm::abs(glm::dot(n, h));
	float nwo = glm::abs(glm::dot(n, wo));
	float nwi = glm::abs(glm::dot(n, wi));
	float hwo = glm::abs(glm::dot(h, wo));
	float hwi = glm::abs(glm::dot(h, wi));

	// Beckmann
	float nh2 = nh*nh;
	float m2 = m*m;
	float d1 = 1 / (M_PI * m2 * nh2 * nh2);
	float d2 = (nh2 - 1) / (m2 * nh2);
	float D = d1 * exp( d2 );

	// Geometric
	float g1 = 2 * nh / hwo;
	float G = glm::min( 1.0f, glm::min(g1 * nwo, g1 * nwi) );

	// Fresnel
	glm::vec3 F = Ro + (1.0f - Ro) * (float)pow( 1.0f - hwi, 5);

	// Cook-Torrence
	glm::vec3 ct = (F * D * G) / (4 * nwo * nwi);

	// PDF
	float om = hwi;
	float pdf = (D * nh) / (4.0f * om);

	return ct / pdf;

}

glm::vec3 Material::mirror(const glm::vec3& reflectance){
	return glm::vec3(1.0f);
}

glm::vec3 Material::brdf( const glm::vec3& n,
						  const glm::vec3& wi,
						  const glm::vec3& wo){

	return brdf_pointer(reflectance_,
						roughness_,
						n,
						wi,
						wo);
}

glm::vec3 Material::btdf( void ){
	return btdf_pointer(reflectance_);
}
