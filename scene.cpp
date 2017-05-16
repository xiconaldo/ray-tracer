#include "scene.h"

Scene::Scene( void )
{}

Scene::~Scene( void )
{}

struct CheckIntersection{

	CheckIntersection(const Ray &ray,
				   	  IntersectionRecord &intersection_record) :
	ray(ray)
	{

		this->intersection_record = intersection_record;
		this->intersection_record.t_ = std::numeric_limits< double >::max();
	}

	bool intersection_result = false;
	IntersectionRecord tmp_intersection_record;
	IntersectionRecord intersection_record;
	const Ray ray;


	void operator()(BBox *node){

		if(node == nullptr) return;

		if(node->intersect(ray)){

			if(node->primitives_index == nullptr){
				this->operator()(node->left);
				this->operator()(node->right);
				return;
			}


			for ( int primitive_id : *(node->primitives_index) )
				if ( Object::primitives_[primitive_id]->intersect( ray, tmp_intersection_record ) )
					if ( ( tmp_intersection_record.t_ < intersection_record.t_ ) && ( tmp_intersection_record.t_ > 0.0 ) )
					{
						intersection_record = tmp_intersection_record;
						intersection_result = true; // the ray intersects a primitive!
					}
		}

	}
};

bool Scene::intersect( const Ray &ray,
					   IntersectionRecord &intersection_record ) const
{

	CheckIntersection check_intersection{ ray, intersection_record };
	check_intersection(bvh.root);
	intersection_record = check_intersection.intersection_record;

	return check_intersection.intersection_result;
}

void Scene::load( void ) {

	Material m1 = {glm::vec3{0.0f}, glm::vec3{0.725f, 0.71f, 0.68f}, Material::diffuse, nullptr, Material::UNIFORM};
	Material m2 = {glm::vec3{0.0f}, glm::vec3{0.63f, 0.065f, 0.05f}, Material::diffuse, nullptr, Material::UNIFORM};
	Material m3 = {glm::vec3{0.0f}, glm::vec3{0.14f, 0.45f, 0.091f}, Material::diffuse, nullptr, Material::UNIFORM};
	Material m4 = {glm::vec3{37.0f, 32.0f, 24.0f}, glm::vec3{0.78f}, Material::diffuse, nullptr, Material::UNIFORM}; //24.0f
	Material m5 = {glm::vec3{0.0f}, glm::vec3{0.1f, 0.9f, 0.9f}, Material::specular, Material::specular, Material::DIRECTIONAL};
	Material m6 = {glm::vec3{0.0f}, glm::vec3{0.06f, 0.065f, 0.84f}, Material::diffuse, nullptr, Material::UNIFORM};
	Material m7 = {glm::vec3{0.0f}, glm::vec3{0.1f, 0.9f, 0.9f}, Material::specular, nullptr, Material::DIRECTIONAL};

	Object::material_list.push_back(m1);
	Object::material_list.push_back(m2);
	Object::material_list.push_back(m3);
	Object::material_list.push_back(m4);
	Object::material_list.push_back(m5);
	Object::material_list.push_back(m6);
	Object::material_list.push_back(m7);

	// loadObject("objects/cornell_box05.obj", 0);
	// loadObject("objects/cornell_box02.obj", 1);
	// loadObject("objects/cornell_box03.obj", 2);
	// loadObject("objects/cornell_box04.obj", 3);
	//loadObject("objects/cornell_box_cube00.obj", 4, glm::vec3{-0.2f, 0.2f, -0.5f});
	// loadObject("objects/cornell_box_cube00.obj", 4, glm::vec3{0.1f, 0.0f, -0.3f});
	//loadObject("objects/cornell_box_cube01.obj", 4);

	loadObject("objects/monkey90.obj", 0);
	loadObject("objects/light.obj", 3);

	// objects_.push_back(Object::ObjectUniquePtr(new Object));
	// Object::primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ 0.5f, 0.31f, 0.1f}, 0.3f } ) );
	// Object::primitives_.back()->material_index = 4;
	//
	// objects_.push_back(Object::ObjectUniquePtr(new Object));
	// Object::primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ -0.3f, 0.31f, -0.3f}, 0.3f } ) );
	// Object::primitives_.back()->material_index = 6;

	std::vector< int > primitives_index(Object::primitives_.size()) ;
	for(unsigned int i = 0; i < primitives_index.size(); i++)
		primitives_index[i] = i;

	bvh.construct(primitives_index);
	bvh.print(bvh.root);
}

void Scene::loadObject(const std::string file_name, int material_index, glm::vec3 translation){

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<Indexed_Face> faces;
	int vertex_count;
	int normal_count;
	int uv_count;
	int face_count;

	objects_.push_back(Object::ObjectUniquePtr(new Object));

	load_data(	file_name,
				vertices,
				normals,
				uvs,
				faces,
				vertex_count,
				normal_count,
				uv_count,
				face_count);

	for(int i = 0; i < face_count; i++){
		Object::primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{
																	vertices[faces[i].vertices[0]-1] + translation,
																	vertices[faces[i].vertices[1]-1] + translation,
																	vertices[faces[i].vertices[2]-1] + translation}));

		Object::primitives_.back()->material_index = material_index;
	}

}
