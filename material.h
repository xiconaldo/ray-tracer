#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <glm/glm.hpp>

struct Material
{
    glm::vec3 emittance_;
    glm::vec3 (*brdf)( void );
};

#endif /* MTERIAL_H_ */