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

	Material m1 = {glm::vec3{0.0f}, glm::vec3{0.725f, 0.71f, 0.68f}, 0.0f, Material::diffuse, nullptr, Material::UNIFORM};
	Material m2 = {glm::vec3{0.0f}, glm::vec3{0.63f, 0.065f, 0.05f}, 0.0f, Material::diffuse, nullptr, Material::UNIFORM};
	Material m3 = {glm::vec3{0.0f}, glm::vec3{0.14f, 0.45f, 0.091f}, 0.0f, Material::diffuse, nullptr, Material::UNIFORM};
	Material m4 = {glm::vec3{12.0f, 12.0f, 12.0f}, glm::vec3{0.78f}, 0.0f, Material::diffuse, nullptr, Material::UNIFORM}; //24.0f
	Material m5 = {glm::vec3{0.0f}, glm::vec3{0.1f, 0.9f, 0.9f}, 0.0f, Material::specular, Material::mirror, Material::DIRECTIONAL};
	Material m6 = {glm::vec3{0.0f}, glm::vec3{0.06f, 0.065f, 0.84f}, 0.0f, Material::diffuse, nullptr, Material::UNIFORM};
	Material m7 = {glm::vec3{0.0f}, glm::vec3{0.1f, 0.9f, 0.9f}, 0.0f, Material::specular, nullptr, Material::DIRECTIONAL};
	Material m8 = {glm::vec3{0.0f}, glm::vec3{1.0f, 0.71f, 0.29f}, 0.05f, Material::glossy, nullptr, Material::GLOSSY};//gold
	// glm::vec3{37.0f, 32.0f, 24.0f}
	Material m9 = {glm::vec3{45.0f, 45.0f, 23.0f}, glm::vec3{0.78f}, 0.0f, Material::diffuse, nullptr, Material::UNIFORM}; //24.0f
	Material m10 = {glm::vec3{0.0f}, glm::vec3{0.05f, 0.05f, 0.05f}, 0.3f, Material::glossy, nullptr, Material::GLOSSY};//plastic
	Material m11 = {glm::vec3{0.0f}, glm::vec3{0.56f, 0.57f, 0.58f}, 0.2f, Material::glossy, nullptr, Material::GLOSSY};//iron
	Material m12 = {glm::vec3{0.0f}, glm::vec3{0.95f, 0.93f, 0.88f}, 0.05f, Material::glossy, nullptr, Material::GLOSSY};//silver
	Material m13 = {glm::vec3{0.0f}, glm::vec3{0.05f, 0.05f, 0.05f}, 0.15f, Material::glossy, nullptr, Material::GLOSSY};//plastic
	Material m14 = {glm::vec3{0.0f}, glm::vec3{0.93f, 0.96f, 0.64f}, 0.35f, Material::glossy, nullptr, Material::GLOSSY};//estante

	Material m15 = {glm::vec3{0.0f}, glm::vec3{0.918f, 0.9f, 0.79f}, 0.0f, Material::diffuse, nullptr, Material::UNIFORM}; //branco ostra
	Material m16 = {glm::vec3{0.0f}, glm::vec3{0.88f, 0.8f, 0.31f}, 0.35f, Material::glossy, nullptr, Material::GLOSSY}; //marfim
	Material m17 = {glm::vec3{0.0f}, glm::vec3{0.278f, 0.251f, 0.18f}, 0.35f, Material::glossy, nullptr, Material::GLOSSY}; //oliveira
	Material m18 = {glm::vec3{0.0f}, glm::vec3{0.219f, 0.17f, 0.117f}, 0.2f, Material::glossy, nullptr, Material::GLOSSY}; //sépia

	Object::material_list.push_back(m1);
	Object::material_list.push_back(m2);
	Object::material_list.push_back(m3);
	Object::material_list.push_back(m4);
	Object::material_list.push_back(m5);
	Object::material_list.push_back(m6);
	Object::material_list.push_back(m7);
	Object::material_list.push_back(m8);
	Object::material_list.push_back(m9);
	Object::material_list.push_back(m10);
	Object::material_list.push_back(m11);
	Object::material_list.push_back(m12);
	Object::material_list.push_back(m13);
	Object::material_list.push_back(m14);
	Object::material_list.push_back(m15);
	Object::material_list.push_back(m16);
	Object::material_list.push_back(m17);
	Object::material_list.push_back(m18);

	// loadObject("objects/final/som.obj", 9);
	// loadObject("objects/final/sofa2.obj", 9);
	// loadObject("objects/final/tv.obj", 12);
	// loadObject("objects/final/bolinhas_luz.obj", 6);
	// loadObject("objects/final/estante_nova.obj", 17); //antes 13
	// loadObject("objects/final/estante_parede.obj", 0); //antes 13
	// loadObject("objects/final/janela1.obj", 14); //antes 10
	// loadObject("objects/final/janela2.obj", 14);
	// loadObject("objects/final/luminaria_chao.obj", 11);
	// loadObject("objects/final/mesinha.obj", 6);
	// loadObject("objects/final/parafusos_mesinha.obj", 10);
	// loadObject("objects/final/pernas_mesinha.obj", 11);
	// loadObject("objects/final/persianas.obj", 14); // antes 7
	// loadObject("objects/final/prateleiras.obj", 17);
	// loadObject("objects/final/rack_suportes.obj", 7);
	// loadObject("objects/final/suportes_das_luzes.obj", 7);
	// loadObject("objects/final/luz_externa.obj", 3);
	// loadObject("objects/final/fonte_de_luz_luminaria_chao.obj", 8);
	// loadObject("objects/final/paredes_isoladas.obj", 16);
	// loadObject("objects/final/teto_isolado.obj", 0);
	// loadObject("objects/final/estatua_da_liberdade.obj", 7);
	// loadObject("objects/final/vaso_de_vidro.obj", 6);
	// loadObject("objects/final/vasos_grandes.obj", 12);
	// loadObject("objects/final/rack_prateleiras.obj", 17);

	// loadObject("objects/cornell_box05.obj", 0);
	// loadObject("objects/cornell_box02.obj", 1);
	// loadObject("objects/cornell_box03.obj", 2);
	// loadObject("objects/cornell_box04.obj", 3);
	//loadObject("objects/cornell_box_cube00.obj", 4, glm::vec3{-0.2f, 0.2f, -0.5f});
	// loadObject("objects/cornell_box_cube00.obj", 4, glm::vec3{0.1f, 0.0f, -0.3f});
	// loadObject("objects/cornell_box_cube01.obj", 4);

	// objects_.push_back(Object::ObjectUniquePtr(new Object));
	// Object::primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ 0.5f, 0.31f, 0.1f}, 0.3f } ) );
	// Object::primitives_.back()->material_index = 4;
	//
	// objects_.push_back(Object::ObjectUniquePtr(new Object));
	// Object::primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ -0.3f, 0.31f, -0.3f}, 0.3f } ) );
	// Object::primitives_.back()->material_index = 6;

	//loadObject("objects/monkey90.obj", 4);
	// loadObject("objects/monkey_cornell.obj", 4);
	//loadObject("objects/light.obj", 3);

	// loadObject("objects/tree.obj", 0);
	//loadObject("objects/ground.obj", 0);
	// //loadObject("objects/light_tree2.obj", 3);
	//
	// objects_.push_back(Object::ObjectUniquePtr(new Object));
	// Object::primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ -1.0f, 10.0f, 2.0f}, 1.2f } ) );
	// Object::primitives_.back()->material_index = 3;

	//loadObject("objects/buda_cornell_box.obj", 7);
	loadObject("objects/buda.obj", 7);
	loadObject("objects/buda_ground.obj", 0);

	objects_.push_back(Object::ObjectUniquePtr(new Object));
	Object::primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ -0.1f, 0.45f, 0.1f}, 0.12f } ) );
	Object::primitives_.back()->material_index = 3;

	std::vector< int > primitives_index(Object::primitives_.size()) ;
	for(unsigned int i = 0; i < primitives_index.size(); i++)
		primitives_index[i] = i;

	time_t initial_time = time(NULL);
	bvh.construct(primitives_index);
	int total_time = difftime(time(NULL), initial_time);

	std::stringstream progress_stream;
	progress_stream << "BVH generation time: "
					<< total_time/3600 << " h "
					<< (total_time/60)%60 << " min "
					<< total_time%60 << " s   \n"
					<< "BVH cost: " << bvh.cost() << "\n\n";

	std::clog << progress_stream.str();

	//bvh.print(bvh.root);
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
