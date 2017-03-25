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

void RayTracer::integrate( void )
{
	const int num_rays = 100; // number of rays

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution_x(0.0f, 1.0f);
	std::uniform_real_distribution<float> distribution_y(0.0f, 1.0f);

	IntersectionRecord intersection_record;
	
	int init_x;
	int init_y;
	int max_x;
	int max_y;

	int work_block;
	int block_size_h = buffer_.h_resolution_ / 15; // /16
	int block_size_v = buffer_.v_resolution_ / 15; // /16
 
	while(true){
		work_block = block++;

		if(work_block > 255) break;

		init_x = (work_block & 0x0f) * block_size_h;
		init_y = ((work_block & 0xf0) >> 4) * block_size_v;

		max_x = std::min(init_x + block_size_h, int(buffer_.h_resolution_));
		max_y = std::min(init_y + block_size_v, int(buffer_.v_resolution_));


		for ( int y = init_y; y < max_y; y++ ){

			for ( int x = init_x; x < max_x; x++){

				buffer_.buffer_data_[x][y] = glm::vec3{0.0f};

				for(int i = 0; i < num_rays; i++){

					float x_pad = distribution_x(generator);
					float y_pad = distribution_y(generator);

					Ray ray{ camera_.getWorldSpaceRay( glm::vec2{ x + x_pad, y + y_pad } ) };

					buffer_.buffer_data_[x][y] += L(ray, 0);			
				}

				buffer_.buffer_data_[x][y] = buffer_.buffer_data_[x][y] / float(num_rays);
				glm::clamp(buffer_.buffer_data_[x][y], glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f});
			}
		}

	}
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

void RayTracer::print_progress(){

	int work_block;

	while(true){

		work_block = block;
		if(work_block > 255) break;

		std::stringstream progress_stream;
		progress_stream << "\rprogress .........................: "
						<< std::fixed << std::setw( 6 )
						<< std::setprecision( 2 )
						<< 100.0 * (work_block+1) / 256
						<< "%";

		std::clog << progress_stream.str();

		std::this_thread::sleep_for (std::chrono::milliseconds(100));
	}
	
}
