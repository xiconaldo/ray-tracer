#include "triangle.h"

Triangle::Triangle( void )
{}

Triangle::Triangle( const glm::vec3 &v_a,
					const glm::vec3 &v_b,
					const glm::vec3 &v_c ) :
		v_a_{ v_a },
		v_b_{ v_b },
		v_c_{ v_c }
{}

bool Triangle::intersect( const Ray &ray,
						IntersectionRecord &intersection_record ) const
{
	
	glm::mat3 D = glm::mat3{-ray.direction_, v_b_ - v_a_, v_c_ - v_a_};
	glm::vec3 R = ray.origin_ - v_a_;

	float det = glm::determinant(D);

	if(det == 0.0f){
		return false;
	}

	glm::mat3 aux = D;
	aux[0] = R;

	float t = glm::determinant(aux)/det;

	aux = D;
	aux[1] = R;

	float gamma = glm::determinant(aux)/det;

	aux = D;
	aux[2] = R;
	float beta = glm::determinant(aux)/det;

	if(t < 0.0f){
		return false;
	}

	if(gamma < 0.0f || gamma > 1.0f){
		return false;
	}

	if(beta < 0.0f || beta > 1.0f - gamma){
		return false;
	}

	intersection_record.t_ = t;
	intersection_record.position_ = ray.origin_ + t*ray.direction_;
	intersection_record.normal_ = glm::normalize(glm::cross(v_b_ - v_a_, v_c_ - v_a_));

	//if(glm::dot(intersection_record.normal_, ray.direction_) > 0.0f)
	//	intersection_record.normal_ = -intersection_record.normal_;

	intersection_record.object = this;
	return true;
}

