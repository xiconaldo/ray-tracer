#include "bvh.h"

BVH::BVH(){}

void BVH::construct(std::vector< int >& primitives_index){

	root = new BBox;
	constructNode(root, primitives_index);

}

void BVH::constructNode(BBox *node, const std::vector< int > &primitives_index){

	// Calculate box dimensions
	node->primitives_index = nullptr;
	node->negative_corner = Object::primitives_[ primitives_index[0] ]->negative_corner;
	node->positive_corner = Object::primitives_[ primitives_index[0] ]->positive_corner;

	for(int prim_id : primitives_index){
		node->negative_corner = min_components(Object::primitives_[prim_id]->negative_corner, node->negative_corner);
		node->positive_corner = max_components(Object::primitives_[prim_id]->positive_corner, node->positive_corner);
	}

	// Select best axis to divide
	glm::vec3 b_size = node->positive_corner - node->negative_corner;
	int axis = 0;

	// Find best division
	std::vector< int > left_prim;
	std::vector< int > right_prim;

	int trying = 0;
	float min_cost = FLT_MAX;
	glm::vec3 best_center;
	int best_axis = axis;
	glm::vec3 aux_size;
	float left_area, right_area, total_area;
	float left_cost, right_cost, total_cost, no_div_cost;
	glm::vec3 current_negative_corner;
	glm::vec3 current_positive_corner;
	const int MAX_TRYING = 32;

	total_area = (b_size.x*b_size.y + b_size.y*b_size.z + b_size.x*b_size.z) * 2.0f;
	no_div_cost = primitives_index.size();

	node->center = node->negative_corner;

	do{
		while( trying++ < MAX_TRYING ){

			for(int prim_id : primitives_index){
				if(Object::primitives_[prim_id]->center_[axis] < node->center[axis])
					left_prim.push_back(prim_id);
				else
					right_prim.push_back(prim_id);
			}


			if(left_prim.size() > 0){
				current_negative_corner = Object::primitives_[ left_prim[0] ]->negative_corner;
				current_positive_corner = Object::primitives_[ left_prim[0] ]->positive_corner;

				for(int prim_id : left_prim){
					current_negative_corner = min_components(Object::primitives_[prim_id]->negative_corner, current_negative_corner);
					current_positive_corner = max_components(Object::primitives_[prim_id]->positive_corner, current_positive_corner);
				}

				aux_size = (current_positive_corner - current_negative_corner);
				left_area = (aux_size.x*aux_size.y + aux_size.y*aux_size.z + aux_size.x*aux_size.z) * 2.0f;
				left_cost = (left_area / total_area) * left_prim.size();
			}
			else{ left_cost = 0.0f; }

			if(right_prim.size() > 0){
				current_negative_corner = Object::primitives_[ right_prim[0] ]->negative_corner;
				current_positive_corner = Object::primitives_[ right_prim[0] ]->positive_corner;

				for(int prim_id : right_prim){
					current_negative_corner = min_components(Object::primitives_[prim_id]->negative_corner, current_negative_corner);
					current_positive_corner = max_components(Object::primitives_[prim_id]->positive_corner, current_positive_corner);
				}

				aux_size = (current_positive_corner - current_negative_corner);
				right_area = (aux_size.x*aux_size.y + aux_size.y*aux_size.z + aux_size.x*aux_size.z) * 2.0f;
				right_cost = (right_area / total_area) * right_prim.size();
			}
			else{ right_cost = 0.0f; }

			total_cost = 2 + right_cost + left_cost;

			if(min_cost > total_cost){
				min_cost = total_cost;
				best_center = node->center;
				best_axis = axis;
			}

			node->center[axis] = node->negative_corner[axis] + b_size[axis] * ( trying / float(MAX_TRYING));

			left_prim.clear();
			right_prim.clear();

		}
		axis++;
		trying = 0;
	} while(axis < 3);

	if( no_div_cost < min_cost){
		node->primitives_index = new std::vector<int>(primitives_index.size());
		*(node->primitives_index) = primitives_index;
		node->left = nullptr;
		node->right = nullptr;
		return;
	}

	node->center = best_center;
	axis = best_axis;

	for(int prim_id : primitives_index){

		if(Object::primitives_[prim_id]->center_[axis] < node->center[axis])
			left_prim.push_back(prim_id);
		else
			right_prim.push_back(prim_id);
	}


	node->left = new BBox;
	constructNode(node->left, left_prim);

	node->right = new BBox;
	constructNode(node->right, right_prim);
}

float BVH::cost(){
	return root->cost();
}

glm::vec3 BVH::max_components(const glm::vec3 &vecA, const glm::vec3 &vecB){

	glm::vec3 max;

	for(int i = 0; i < 3; i++)
		if(vecA[i] > vecB[i])
			max[i] = vecA[i];
		else
			max[i] = vecB[i];

	return max;
}

glm::vec3 BVH::min_components(const glm::vec3 &vecA, const glm::vec3 &vecB){

	glm::vec3 min;

	for(int i = 0; i < 3; i++)
		if(vecA[i] < vecB[i])
			min[i] = vecA[i];
		else
			min[i] = vecB[i];

	return min;
}

bool BBox::intersect( const Ray &ray ){

	// return true;

	for(int i = 0; i < 3; i++){

		int j = (i+1)%3;
		int k = (i+2)%3;

		glm::vec3 intersect_point;
		float t = (positive_corner[i] - ray.origin_[i]) / ray.direction_[i];

		intersect_point[i] = positive_corner[i];
		intersect_point[j] = ray.origin_[j] + t*ray.direction_[j];
		intersect_point[k] = ray.origin_[k] + t*ray.direction_[k];

		if(intersect_point[j] <= positive_corner[j] &&
		   intersect_point[j] >= negative_corner[j] &&
		   intersect_point[k] <= positive_corner[k] &&
		   intersect_point[k] >= negative_corner[k] )
			return true;
	}

	for(int i = 0; i < 3; i++){

		int j = (i+1)%3;
		int k = (i+2)%3;

		glm::vec3 intersect_point;
		float t = (negative_corner[i] - ray.origin_[i]) / ray.direction_[i];

		intersect_point[i] = negative_corner[i];
		intersect_point[j] = ray.origin_[j] + t*ray.direction_[j];
		intersect_point[k] = ray.origin_[k] + t*ray.direction_[k];

		if(intersect_point[j] <= positive_corner[j] &&
		   intersect_point[j] >= negative_corner[j] &&
		   intersect_point[k] <= positive_corner[k] &&
		   intersect_point[k] >= negative_corner[k] )
			return true;
	}

	return false;

}

void BVH::print(BBox *node){

	static int depth = -1;
	static std::vector<int> path;

	depth++;

	for(int i = 0; i < depth; i++){
		if(i == depth-1)
			std::cout << "+--";
		else if( path[i] == 0)
			std::cout << "|  ";
		else
			std::cout << "   ";
	}

	std::cout << node->size() << std::endl;

	for(int i = 0; i < depth; i++){
		if( path[i] == 0)
			std::cout << "|  ";
		else
			std::cout << "   ";
	}

	if(node->primitives_index == nullptr)
		std::cout << "|  ";

	std::cout << std::endl;

	path.push_back(0);
	if(node->left != nullptr) print(node->left);
	path.pop_back();
	path.push_back(1);
	if(node->right != nullptr) print(node->right);
	path.pop_back();

	depth--;
}

int BBox::size(){
	if(primitives_index != nullptr)
		return primitives_index->size();
	else
		return left->size() + right->size();
}

float BBox::cost(float parent_area){

	glm::vec3 b_size = positive_corner - negative_corner;
	float area = (b_size.x*b_size.y + b_size.y*b_size.z + b_size.x*b_size.z) * 2.0f;

	if(parent_area == 0) parent_area = area;

	if(primitives_index == nullptr)
		return (area / parent_area) * (2 + left->cost(area) + right->cost(area));

	return (area / parent_area) * primitives_index->size();
}
