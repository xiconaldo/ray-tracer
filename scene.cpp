#include "scene.h"

Scene::Scene( void )
{}

Scene::~Scene( void )
{}

bool Scene::intersect( const Ray &ray,
                       IntersectionRecord &intersection_record ) const
{
    bool intersection_result = false;
    IntersectionRecord tmp_intersection_record;
    intersection_record.t_ = std::numeric_limits< double >::max();
    std::size_t num_primitives = primitives_.size();

    // Loops over the list of primitives, testing the intersection of each primitive against the given ray 
    for ( std::size_t primitive_id = 0; primitive_id < num_primitives; primitive_id++ )
        if ( primitives_[primitive_id]->intersect( ray, tmp_intersection_record ) )
            if ( ( tmp_intersection_record.t_ < intersection_record.t_ ) && ( tmp_intersection_record.t_ > 0.0 ) )
            {
                intersection_record = tmp_intersection_record;
                intersection_result = true; // the ray intersects a primitive!
            }

    return intersection_result;
}

void Scene::load( void ) 
{
	// 1
	// for(int i = 0; i < 3; i++){
	// 	primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ 
	//  																glm::vec3{-0.3f + cos(i/3.0f*2*M_PI)*0.5f, -0.3f + sin(i/3.0f*2*M_PI)*0.5f, -1.4f + i*0.4f},
	//  																glm::vec3{ 0.3f + cos(i/3.0f*2*M_PI)*0.5f, -0.3f + sin(i/3.0f*2*M_PI)*0.5f, -1.4f + i*0.4f},
	//  																glm::vec3{ 0.0f + cos(i/3.0f*2*M_PI)*0.5f,  0.3f + sin(i/3.0f*2*M_PI)*0.5f, -1.4f + i*0.4f} }));

	// 		primitives_.back()->color = glm::vec3{ rand()%256/255.0, 
	// 										  rand()%256/255.0,
	// 										  rand()%256/255.0};
	// }


	 
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ -0.5f, 0.5f, -2.0f},
		// 														0.4f} ));
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ -0.5f, -0.5f, -2.0f},
		// 														0.4f} ));
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ 0.5f, -0.5f, -2.0f},
		// 														0.4f} ));
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ 0.5f, 0.5f, -2.0f},
		// 														0.4f} ));

	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ -0.5f, 0.5f, -1.0f},
		// 														0.4f} ));
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ -0.5f, -0.5f, -1.0f},
		// 														0.4f} ));
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ 0.5f, -0.5f, -1.0f},
		// 														0.4f} ));
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ 0.5f, 0.5f, -1.0f},
		// 														0.4f} ));

	primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
																glm::vec3{ 0.0f, 2.0f, 0.0f},
																1.0f} ));

	primitives_.back()->color = glm::vec3{ 30.0f, 30.0f, 30.0f};
	primitives_.back()->emiter = true;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<Indexed_Face> faces;
	int vertex_count;
	int normal_count; 
	int uv_count;
	int face_count;

	load_data(	"monkey90.obj",
				vertices, 
				normals, 
				uvs, 
				faces, 
				vertex_count, 
				normal_count, 
				uv_count, 
				face_count); 

	for(int i = 0; i < face_count; i++){
		primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ 
																	vertices[faces[i].vertices[0]-1],
																	vertices[faces[i].vertices[1]-1],
																	vertices[faces[i].vertices[2]-1] }));
		
		primitives_.back()->color = glm::vec3{ rand()%256/255.0, 
											  rand()%256/255.0,
											  rand()%256/255.0};
	}
}

