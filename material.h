#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <glm/glm.hpp>


struct Material
{
	enum SamplerMode{ UNIFORM, DIRECTIONAL, GLOSSY };

	glm::vec3 emittance_;
	glm::vec3 reflectance_;
	float roughness_;

	glm::vec3 (*brdf_pointer)( const glm::vec3& reflectance,
	 						  const float roughness,
							  const glm::vec3& n,
						  	  const glm::vec3& wi,
						  	  const glm::vec3& wo);

	glm::vec3 (*btdf_pointer)( const glm::vec3& reflectance );
	SamplerMode mode;

	glm::vec3 brdf( const glm::vec3& n, const glm::vec3& wi, const glm::vec3& wo);
	glm::vec3 btdf( void );

	static glm::vec3 diffuse( const glm::vec3& reflectance,
	 						  const float roughness,
							  const glm::vec3& n,
						  	  const glm::vec3& wi,
						  	  const glm::vec3& wo);

	static glm::vec3 specular( const glm::vec3& reflectance,
	 						  const float roughness,
							  const glm::vec3& n,
						  	  const glm::vec3& wi,
						  	  const glm::vec3& wo);

	static glm::vec3 glossy( const glm::vec3& reflectance,
	 						  const float roughness,
							  const glm::vec3& n,
						  	  const glm::vec3& wi,
						  	  const glm::vec3& wo);

    static glm::vec3 mirror(const glm::vec3& reflectance);
};

#endif /* MTERIAL_H_ */
