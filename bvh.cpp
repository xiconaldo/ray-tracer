#include "bvh.h"

BVH::BVH(){}

void BVH::construct(std::vector< int >& primitives_index){

	root = new BBox;
	constructNode(root, primitives_index);

}

void BVH::constructNode(BBox *node, std::vector< int > primitives_index){

	static int depth = 0;
	//static int sum = 0;
	depth++;
	
	node->primitives_index = nullptr;

	node->negative_corner = Object::primitives_[ primitives_index[0] ]->negative_corner;
	node->positive_corner = Object::primitives_[ primitives_index[0] ]->positive_corner;

	std::cout << std::fixed << std::setprecision(2);

	for(int prim_id : primitives_index){

		node->negative_corner = min_components(Object::primitives_[prim_id]->negative_corner, node->negative_corner);
		node->positive_corner = max_components(Object::primitives_[prim_id]->positive_corner, node->positive_corner);

	}

	node->center = (node->negative_corner + node->positive_corner) * 0.5f;
	
	
	if(primitives_index.size() < 5){
		node->primitives_index = new std::vector<int>(primitives_index.size());
		*(node->primitives_index) = primitives_index;
		node->left = nullptr;
		node->right = nullptr;
		//std::cout << "#" << depth << " : " << primitives_index.size() << "\n";
		depth--;
		return;
	}
	
	glm::vec3 b_size = node->positive_corner - node->negative_corner;
	float aux = std::max(b_size.x, std::max(b_size.y, b_size.z));
	int axis;

	if(aux == b_size.x) axis = 0;
	else if(aux == b_size.y) axis = 1;
	else axis = 2;

	std::vector< int > left_prim;
	std::vector< int > right_prim;

	int trying = 0;
	int init_axis = axis;
	float min_diff = FLT_MAX;
	glm::vec3 best_center;
	int best_axis = init_axis;
	glm::vec3 aux_size;
	float area1, area2;
	glm::vec3 current_negative_corner;
	glm::vec3 current_positive_corner;

	do{
		while( trying++ < 100 ){

			for(int prim_id : primitives_index){

				if(Object::primitives_[prim_id]->center_[axis] < node->center[axis])
					left_prim.push_back(prim_id);
				else
					right_prim.push_back(prim_id);
			}


			if(left_prim.size() > 0){
				current_negative_corner = min_components(Object::primitives_[ left_prim[0] ]->negative_corner, current_negative_corner);
				current_positive_corner = max_components(Object::primitives_[ left_prim[0] ]->positive_corner, current_positive_corner);

				for(int prim_id : left_prim){

					current_negative_corner = min_components(Object::primitives_[prim_id]->negative_corner, current_negative_corner);
					current_positive_corner = max_components(Object::primitives_[prim_id]->positive_corner, current_positive_corner);
				}

				aux_size = (current_positive_corner - current_negative_corner);
				area1 = (aux_size.x*aux_size.y + aux_size.y*aux_size.z + aux_size.x*aux_size.z) * 2.0f;
			}
			else{
				area1 = 0.0f;
			}

			if(right_prim.size() > 0){
				current_negative_corner = min_components(Object::primitives_[ right_prim[0] ]->negative_corner, current_negative_corner);
				current_positive_corner = max_components(Object::primitives_[ right_prim[0] ]->positive_corner, current_positive_corner);

				for(int prim_id : right_prim){

					current_negative_corner = min_components(Object::primitives_[prim_id]->negative_corner, current_negative_corner);
					current_positive_corner = max_components(Object::primitives_[prim_id]->positive_corner, current_positive_corner);
				}

				aux_size = (current_positive_corner - current_negative_corner);
				area2 = (aux_size.x*aux_size.y + aux_size.y*aux_size.z + aux_size.x*aux_size.z) * 2.0f;
			}
			else{
				area2 = 0.0f;
			}


			if(min_diff > glm::abs(area2-area1)){
				min_diff = glm::abs(area2-area1);
				best_center = node->center;
				best_axis = axis;
			}

			if(area1 > area2)
				node->center[axis] = (node->center[axis] + node->negative_corner[axis]) * 0.5f;
			else
				node->center[axis] = (node->center[axis] + node->positive_corner[axis]) * 0.5f;

			left_prim.clear();
			right_prim.clear();
			
		}
		axis = (axis+1)%3;
		trying = 0;
	} while(axis != init_axis);


	node->center = best_center;
	axis = best_axis;

	for(int prim_id : primitives_index){

		if(Object::primitives_[prim_id]->center_[axis] < node->center[axis])
			left_prim.push_back(prim_id);
		else
			right_prim.push_back(prim_id);
	}


	if(left_prim.empty() || right_prim.empty()){
		node->primitives_index = new std::vector<int>(primitives_index.size());
		*(node->primitives_index) = primitives_index;
		node->left = nullptr;
		node->right = nullptr;
		//std::cout << "#" << depth << " : " << primitives_index.size() << "\n";
		depth--;
		return;
	}

	node->left = new BBox;
	constructNode(node->left, left_prim);

	node->right = new BBox;
	constructNode(node->right, right_prim);
	
	depth--;
	
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

	std::cout << node->center.x << "{ ";

	if(node->left != nullptr) print(node->left);

	if(node->right != nullptr) print(node->right);

	if(node->left == nullptr && node->right == nullptr){
		for(int i : *node->primitives_index)
			std::cout << i << " ";
		return;
	}

	std::cout << " }";
}