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

void Scene::load( void ) {
/*
	//LEFT
	primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{-2.0f, -2.0f, -2.0f},
														  				glm::vec3{-2.0f,  2.0f,  2.0f},
														  				glm::vec3{-2.0f,  2.0f, -2.0f} }));

	primitives_.back()->color = glm::vec3{ 1.0f, 0.0f, 0.0f};

	primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{-2.0f, -2.0f, -2.0f},
														  				glm::vec3{-2.0f,  2.0f,  2.0f},
														  				glm::vec3{-2.0f, -2.0f,  2.0f} }));

	primitives_.back()->color = glm::vec3{ 1.0f, 0.0f, 0.0f};


	// RIGHT
	primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{ 2.0f, -2.0f, -2.0f},
														  				glm::vec3{ 2.0f,  2.0f,  2.0f},
														  				glm::vec3{ 2.0f,  2.0f, -2.0f} }));

	primitives_.back()->color = glm::vec3{ 0.0f, 0.0f, 1.0f};

	primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{ 2.0f, -2.0f, -2.0f},
														  				glm::vec3{ 2.0f,  2.0f,  2.0f},
														  				glm::vec3{ 2.0f, -2.0f,  2.0f} }));

	primitives_.back()->color = glm::vec3{ 0.0f, 0.0f, 1.0f};


	// BACK
	primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{ 2.0f, -2.0f, -2.0f},
														  				glm::vec3{ 2.0f,  2.0f, -2.0f},
														  				glm::vec3{-2.0f,  2.0f, -2.0f} }));

	primitives_.back()->color = glm::vec3{ 0.8f, 0.8f, 0.8f};

	primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{ 2.0f, -2.0f, -2.0f},
														  				glm::vec3{-2.0f, -2.0f, -2.0f},
														  				glm::vec3{-2.0f,  2.0f, -2.0f} }));

	primitives_.back()->color = glm::vec3{ 0.8f, 0.8f, 0.8f};


	// BOTTOM

	primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{ 2.0f, -2.0f,  2.0f},
														  				glm::vec3{ 2.0f, -2.0f, -2.0f},
														  				glm::vec3{-2.0f, -2.0f, -2.0f} }));

	primitives_.back()->color = glm::vec3{ 0.8f, 0.8f, 0.8f};

	primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{-2.0f, -2.0f, -2.0f},
														  				glm::vec3{ 2.0f, -2.0f,  2.0f},
														  				glm::vec3{-2.0f, -2.0f,  2.0f} }));

	primitives_.back()->color = glm::vec3{ 0.8f, 0.8f, 0.8f};


	// TOP

	primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{ 2.0f,  2.0f,  2.0f},
														  				glm::vec3{ 2.0f,  2.0f, -2.0f},
														  				glm::vec3{-2.0f,  2.0f, -2.0f} }));

	primitives_.back()->color = glm::vec3{ 0.8f, 0.8f, 0.8f};

	primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{-2.0f,  2.0f, -2.0f},
														  				glm::vec3{ 2.0f,  2.0f,  2.0f},
														  				glm::vec3{-2.0f,  2.0f,  2.0f} }));

	primitives_.back()->color = glm::vec3{ 0.8f, 0.8f, 0.8f};


	// LIGHT

	primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{ 0.5f,  1.99f,  0.5f},
														  				glm::vec3{ 0.5f,  1.99f, -0.5f},
														  				glm::vec3{-0.5f,  1.99f, -0.5f} }));

	primitives_.back()->color = glm::vec3{ 60.0f, 60.0f, 60.0f};
	primitives_.back()->emiter = true;

	primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{-0.5f,  1.99f, -0.5f},
														  				glm::vec3{ 0.5f,  1.99f,  0.5f},
														  				glm::vec3{-0.5f,  1.99f,  0.5f} }));

	primitives_.back()->color = glm::vec3{ 60.0f, 60.0f, 60.0f};
	primitives_.back()->emiter = true;
	
*/

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
																	vertices[faces[i].vertices[0]-1] /*+ glm::vec3(0.0f, 0.0f, 1.0f)*/,
																	vertices[faces[i].vertices[1]-1] /*+ glm::vec3(0.0f, 0.0f, 1.0f) */,
																	vertices[faces[i].vertices[2]-1] /*+ glm::vec3(0.0f, 0.0f, 1.0f) */}));
		
		primitives_.back()->color = glm::vec3{ 0.5f, 0.5f, 0.5f};
	}
}

