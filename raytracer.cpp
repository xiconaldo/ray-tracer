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

	// Image space origin (i.e. x = 0 and y = 0) at the top left corner.

	// Loops over image rows
	for ( std::size_t y = 0; y < buffer_.v_resolution_; y++ )
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

		const int num_rays = 10; // number of intervals

		// Loops over image columns
		for ( std::size_t x = thread_id; x < buffer_.h_resolution_; x += num_threads ){

			buffer_.buffer_data_[x][y] = glm::vec3{0.0f};

			std::default_random_engine generator;
			std::uniform_real_distribution<float> distribution_x(0.0f, 1.0f);
			std::uniform_real_distribution<float> distribution_y(0.0f, 1.0f);

			for(int i = 0; i < num_rays; i++){

				intersection_record.t_ = std::numeric_limits< double >::max();

				float x_pad = distribution_x(generator);
				float y_pad = distribution_y(generator);

				Ray ray{ camera_.getWorldSpaceRay( glm::vec2{ x + x_pad, y + y_pad } ) };
			
				if ( scene_.intersect( ray, intersection_record ) )
					buffer_.buffer_data_[x][y] += intersection_record.object->color;
					
			}

			buffer_.buffer_data_[x][y] = buffer_.buffer_data_[x][y] / float(num_rays);
		}
	}

	std::clog << std::endl;
}

