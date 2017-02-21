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
	intersection_record.object = this;
	return true;



	// float a, b, c, d, e, f, g, h, i, j, k, l;

	// a = v_a_.x - v_b_.x;
	// b = v_a_.y - v_b_.y;
	// c = v_a_.z - v_b_.z;

	// d = v_a_.x - v_c_.x;
	// e = v_a_.y - v_c_.y;
	// f = v_a_.z - v_c_.z;

	// g = ray.direction_.x;
	// h = ray.direction_.y;
	// i = ray.direction_.z;

	// j = v_a_.x - ray.origin_.x;
	// k = v_a_.y - ray.origin_.y;
	// l = v_a_.z - ray.origin_.z;

	// float M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
	
	// float t = -(f*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c)) / M;
	// if(t < 0.0f){
	// 	return false;
	// }

	// float gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c)) / M;
	// if(gamma < 0.0f || gamma > 1.0f){
	// 	return false;
	// }

	
	// float beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g)) / M;
	// if(beta < 0.0f || beta > 1.0f - gamma){
	// 	return false;
	// }

	// intersection_record.t_ = t;
	// intersection_record.position_ = ray.origin_ + t*ray.direction_;

	// return true;
}

