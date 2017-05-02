#ifndef INTERSECTION_RECORD_H_
#define INTERSECTION_RECORD_H_

#include <glm/glm.hpp>
#include "primitive.h"

class Primitive;

struct IntersectionRecord
{
	float t_;
	glm::vec3 position_;
	glm::vec3 normal_;
	const Primitive *object;
};

#endif /* INTERSECTION_RECORD_H_ */

