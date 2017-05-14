#ifndef BVH_H
#define BVH_H

#include "object.h"
#include <iostream>
#include <iomanip>

struct BBox{

	typedef std::vector< int > Vec_int;

	BBox *left;
	BBox *right;
	Vec_int *primitives_index;

	glm::vec3 positive_corner;
	glm::vec3 negative_corner;
	glm::vec3 center;

	bool intersect( const Ray &ray );

};

class BVH{

public:
	BVH();

	void construct(std::vector< int >& primitives_index);

	void constructNode(BBox *node, std::vector< int > primitives_index);

	void print(BBox *node);

	BBox *root;

	private:
	glm::vec3 max_components(const glm::vec3 &vecA, const glm::vec3 &vecB);
	glm::vec3 min_components(const glm::vec3 &vecA, const glm::vec3 &vecB);
};

#endif