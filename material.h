#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <glm/glm.hpp>


struct Material
{
	enum SamplerMode{ UNIFORM, DIRECTIONAL };

	glm::vec3 emittance_;
	glm::vec3 reflectance_;
	glm::vec3 (*brdf_pointer)( const glm::vec3& reflectance );
	glm::vec3 (*btdf_pointer)( const glm::vec3& reflectance );

	SamplerMode mode;
	glm::vec3 brdf( void );
	glm::vec3 btdf( void );

	static glm::vec3 diffuse( const glm::vec3& reflectance );
	static glm::vec3 specular( const glm::vec3& reflectance );
};

#endif /* MTERIAL_H_ */