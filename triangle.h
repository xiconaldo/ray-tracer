#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <glm/glm.hpp>
#include "primitive.h"
#include "intersection_record.h"
#include "ray.h"

class Triangle : public Primitive
{
public:

	Triangle( void );

	Triangle( const glm::vec3 &v_a,
			const glm::vec3 &v_b,
			const glm::vec3 &v_c );

	bool intersect( const Ray &ray,
					IntersectionRecord &intersection_record ) const;

	glm::vec3 v_a_;
	glm::vec3 v_b_;
	glm::vec3 v_c_;

private:
	glm::vec3 max_components(const glm::vec3 &vecA, const glm::vec3 &vecB);
	glm::vec3 min_components(const glm::vec3 &vecA, const glm::vec3 &vecB);
};

#endif /* TRIANGLE_H_ */
