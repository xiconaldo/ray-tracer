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

void RayTracer::integrate( const int num_threads, const int num_rays)
{
	std::thread **threads = new std::thread*[num_threads];
	std::thread control{&RayTracer::print_progress, this};
	
	for(int i = 0; i < num_threads; i++)
		threads[i] = new std::thread{&RayTracer::integrate_parallel, this, num_rays};

	for(int i = 0; i < num_threads; i++){
		threads[i]->join();
		delete threads[i];
	}
	delete[] threads;

	control.join();
}

void RayTracer::integrate_parallel( const int num_rays )
{
	//std::default_random_engine generator;
	std::mt19937 generator;
	std::uniform_real_distribution<float> dist_x(0.0f, 1.0f);
	std::uniform_real_distribution<float> dist_y(0.0f, 1.0f);
	std::uniform_real_distribution<float> dist_theta(0.0f, 2.0f * M_PI);
	std::uniform_real_distribution<float> dist_phi(0.0f, 1.0f);

	// IntersectionRecord intersection_record;
	
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

					float x_pad = dist_x(generator);
					float y_pad = dist_y(generator);

					Ray ray{ camera_.getWorldSpaceRay( glm::vec2{ x + x_pad, y + y_pad } ) };

					buffer_.buffer_data_[x][y] += L(ray, 0, dist_theta, dist_phi, generator);			
				}

				buffer_.buffer_data_[x][y] /= float(num_rays);

				glm::clamp(buffer_.buffer_data_[x][y], glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f});
			}
		}

	}
}

glm::vec3 RayTracer::L(const Ray& r, int depth, 
					   std::uniform_real_distribution<float>& dist_theta,
					   std::uniform_real_distribution<float>& dist_phi,
					   std::mt19937& generator){

	glm::vec3 Lo = glm::vec3{0.0f};
	IntersectionRecord intersection_record;

	if(depth < 5)
		if ( scene_.intersect( r, intersection_record ) ){

			Material material = Object::material_list[intersection_record.object->material_index];

			if(!material.brdf_pointer){
				//Lo = intersection_record.object->color;
				Lo = material.emittance_;
			}

			else{

				float theta, phi, x, y, z;
				
				theta = dist_theta(generator);
				phi = acos(dist_phi(generator));

				// theta = 2*M_PI*rand()/float(RAND_MAX);
				// phi = acos(1 - rand()/float(RAND_MAX));

				
				x = sin(phi) * cos(theta);
				y = sin(phi) * sin(theta);
				z = cos(phi);

				// WRONG!!!
				// x = sin(theta) * sin(phi);
				// y = sin(theta) * cos(phi);
				// z = sin(phi);

				// glm::vec3 new_ray = glm::normalize(glm::vec3(x, y, z));
				glm::vec3 new_ray = glm::vec3(x, y, z);

				float cosTheta = glm::dot(new_ray, intersection_record.normal_);
				
				if(cosTheta < 0){
					new_ray = -new_ray;
					cosTheta = -cosTheta;
				}

				Ray reflect{ intersection_record.position_ + 0.001f * intersection_record.normal_, new_ray };

				Lo = material.emittance_ +
					 2.0f * float(M_PI) * 
					 material.brdf() * 
					 L(reflect, ++depth, dist_theta, dist_phi, generator) * 
					 cosTheta;

				//Lo = 2.0f * intersection_record.object->color * L(reflect, ++depth, dist_theta, dist_phi, generator) * cosTheta;
			}
		}

	return Lo;
}

void RayTracer::print_progress(){

	int work_block;
	int completed_blocks;
	time_t initial_time = time(NULL);
	int remaining_time = 0;
	int prev_total_time = 0;

	while(true){

		work_block = block;
		completed_blocks = work_block > 3 ? work_block-4 : 0;

		std::stringstream progress_stream;
		progress_stream << "\r\rProgress .........................: "
						<< std::fixed << std::setw( 6 )
						<< std::setprecision( 2 )
						<< 0.390625 * completed_blocks
						<< "% | "
						<< completed_blocks << "/" << 256;

		if(completed_blocks){

			prev_total_time = 256 * difftime(time(NULL), initial_time) / completed_blocks;
			remaining_time =  prev_total_time - difftime(time(NULL), initial_time);

			progress_stream << " | "
							<< remaining_time/3600 << " h " 
			 				<< (remaining_time/60)%60 << " min "
			 				<< remaining_time%60 << " s   ";
		}

		std::clog << progress_stream.str();

		if(work_block == 260) break;
		std::this_thread::sleep_for (std::chrono::seconds(prev_total_time * 0.05 < 2 ? 2 : prev_total_time * 0.05 > 5 ? 5 : int(prev_total_time * 0.05)));
		//std::this_thread::sleep_for (std::chrono::seconds(2));
	}	
	
	std::clog << std::endl;	
}
