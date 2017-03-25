#include "raytracer.h"

RayTracer::RayTracer( Camera &camera,
					const Scene &scene,
					const glm::vec3 background_color,
					Buffer &buffer ) :
		camera_( camera ),
		scene_( scene ),
		background_color_{ background_color },
		buffer_( buffer )
{}

void RayTracer::integrate( int thread_id, int num_threads)
{
	IntersectionRecord intersection_record;
	std::default_random_engine generator;
	int inc_x = num_threads/int(sqrt(num_threads));
	int inc_y = num_threads/inc_x;
	int init_x = thread_id%inc_x;
	int init_y = thread_id/inc_y;

	// Image space origin (i.e. x = 0 and y = 0) at the top left corner.

	// Loops over image rows
	for ( std::size_t y = init_y; y < buffer_.v_resolution_; y += inc_y )
	{
		if(thread_id == 0){
			std::stringstream progress_stream;
			progress_stream << "\rprogress .........................: "
							<< std::fixed << std::setw( 6 )
							<< std::setprecision( 2 )
							<< 100.0 * y / ( buffer_.v_resolution_ - 1 )
							<< "%";

			std::clog << progress_stream.str();
		}

		const int num_rays = 1000; // number of rays

		// Loops over image columns
		for ( std::size_t x = init_x; x < buffer_.h_resolution_; x += inc_x ){

			buffer_.buffer_data_[x][y] = glm::vec3{0.0f};

			std::uniform_real_distribution<float> distribution_x(0.0f, 1.0f);
			std::uniform_real_distribution<float> distribution_y(0.0f, 1.0f);

			for(int i = 0; i < num_rays; i++){

				float x_pad = distribution_x(generator);
				float y_pad = distribution_y(generator);

				Ray ray{ camera_.getWorldSpaceRay( glm::vec2{ x + x_pad, y + y_pad } ) };

				buffer_.buffer_data_[x][y] += L(ray, 0);

				// intersection_record.t_ = std::numeric_limits< double >::max();
			
				// if ( scene_.intersect( ray, intersection_record ) )
				// 	buffer_.buffer_data_[x][y] += intersection_record.object->color;
					
			}

			buffer_.buffer_data_[x][y] = buffer_.buffer_data_[x][y] / float(num_rays);
			glm::clamp(buffer_.buffer_data_[x][y], glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f});
		}
	}

	std::clog << std::endl;
}

glm::vec3 RayTracer::L(const Ray& r, int depth){

	glm::vec3 Lo = glm::vec3{0.0f};
	IntersectionRecord intersection_record;

	if(depth < 5)
		if ( scene_.intersect( r, intersection_record ) ){

			if(intersection_record.object->emiter){
				Lo = intersection_record.object->color;
			}

			else{

				float a, b, x, y, z;
				
				a = 2 * M_PI * rand() / float(RAND_MAX);
				b = acos(1 - rand() / float(RAND_MAX));
				x = sin(a) * sin(b);
				y = sin(a) * cos(b);
				z = sin(b);

				glm::vec3 new_ray = glm::normalize(glm::vec3(x, y, z));

				float cosTheta = glm::dot(new_ray, intersection_record.normal_);
				
				if(cosTheta < 0){
					new_ray = -new_ray;
					cosTheta = -cosTheta;
				}

				Ray reflect{ intersection_record.position_, new_ray };

				Lo = 2.0f * intersection_record.object->color * L(reflect, ++depth) * cosTheta;
			}
		}

	return Lo;
}

