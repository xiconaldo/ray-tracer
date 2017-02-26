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
   	// primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ 
	 		// 														glm::vec3{-0.5f, -0.5f, -1.0f},
	 		// 														glm::vec3{ 0.5f, -0.5f, -1.0f},
	 		// 														glm::vec3{ 0.0f,  0.5f, -5.0f} }));

   	 // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ 
	 		 // 														glm::vec3{ 1.0f,  0.5f, -6.0f},
	 		 // 														glm::vec3{ 0.5f, -0.5f, -1.0f},
	 		 // 														glm::vec3{ 0.0f,  0.5f, -5.0f} }));
	 
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ -0.6f, 0.6f, -2.0f},
		// 														0.5f} ));
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ -0.6f, -0.6f, -2.0f},
		// 														0.5f} ));
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ 0.6f, -0.6f, -2.0f},
		// 														0.5f} ));
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ 0.6f, 0.6f, -2.0f},
		// 														0.5f} ));

	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ -0.6f, 0.6f, -1.0f},
		// 														0.5f} ));
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ -0.6f, -0.6f, -1.0f},
		// 														0.5f} ));
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ 0.6f, -0.6f, -1.0f},
		// 														0.5f} ));
	 // primitives_.push_back(Primitive::PrimitiveUniquePtr(new Sphere{
		// 														glm::vec3{ 0.6f, 0.6f, -1.0f},
		// 														0.5f} ));

	 // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ 
	 // 		 														glm::vec3{  1.0f, -1.0f,  1.0f},
	 // 		 														glm::vec3{ -1.0f, -1.0f,  1.0f},
	 // 		 														glm::vec3{  -1.0f,  -1.0f, -1.0f} }));


	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<Indexed_Face> faces;
	int vertex_count;
	int normal_count; 
	int uv_count;
	int face_count;

	load_data(	"cube45.obj",
				vertices, 
				normals, 
				uvs, 
				faces, 
				vertex_count, 
				normal_count, 
				uv_count, 
				face_count);

	for(int i = 0; i < vertex_count; i++)
		std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;

	std::cout << std::endl;	 

	for(int i = 0; i < face_count; i++){
		primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ 
																	vertices[faces[i].vertices[0]-1],
																	vertices[faces[i].vertices[1]-1],
																	vertices[faces[i].vertices[2]-1] }));

		std::cout << face_count << ": " << faces[i].vertices[0] << " " << faces[i].vertices[1] << " " << faces[i].vertices[2] << std::endl;
		std::cout << "{" << vertices[faces[i].vertices[0]-1].x << ", "
						 << vertices[faces[i].vertices[0]-1].y << ", "
						 << vertices[faces[i].vertices[0]-1].z << "}" << std::endl

				  << "{" << vertices[faces[i].vertices[1]-1].x << ", "
						 << vertices[faces[i].vertices[1]-1].y << ", "
						 << vertices[faces[i].vertices[1]-1].z << "}" << std::endl

				  << "{" << vertices[faces[i].vertices[2]-1].x << ", "
						 << vertices[faces[i].vertices[2]-1].y << ", "
						 << vertices[faces[i].vertices[2]-1].z << "}" << std::endl;

		// primitives_.back()->color = glm::vec3{(vertices[faces[i].vertices[0]-1].x + 1.0f )*255.0, 
		// 									  (vertices[faces[i].vertices[1]-1].x + 1.0f )*255.0,
		// 									  (vertices[faces[i].vertices[2]-1].x + 1.0f )*255.0};
		// 									  
		
		primitives_.back()->color = glm::vec3{ rand()%256/255.0, 
											  rand()%256/255.0,
											  rand()%256/255.0};
	}
}

