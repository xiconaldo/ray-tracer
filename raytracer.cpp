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
	this->num_threads = num_threads;
	progress = new int[num_threads];

	num_blocks_h = buffer_.h_resolution_/block_size_h;
	num_blocks_v = buffer_.v_resolution_/block_size_v;
	num_blocks = num_blocks_h * num_blocks_v;

	std::thread **threads = new std::thread*[num_threads];
	
	for(int i = 0; i < num_threads; i++){
		progress[i] = 0;
		threads[i] = new std::thread{&RayTracer::integrate_parallel, this, num_rays, i};
	}

	std::thread control{&RayTracer::print_progress, this};

	for(int i = 0; i < num_threads; i++){
		threads[i]->join();
		delete threads[i];
	}
	delete[] threads;

	control.join();

	delete[] progress;
}

void RayTracer::integrate_parallel( const int num_rays, const int thread_id )
{

	std::mt19937 generator;
	std::uniform_real_distribution<float> dist_x(0.0f, 1.0f);
	std::uniform_real_distribution<float> dist_y(0.0f, 1.0f);
	std::uniform_real_distribution<float> dist_theta(0.0f, 2.0f * M_PI);
	std::uniform_real_distribution<float> dist_phi(0.0f, 1.0f);
	
	int init_x;
	int init_y;
	int max_x;
	int max_y;

	int work_block;
  
	while(true){
		work_block = block++;

		if(work_block >= num_blocks) break;

		init_x = (work_block % num_blocks_h) * block_size_h;
		init_y = (work_block / num_blocks_h) * block_size_v;

		if( (work_block % num_blocks_h) < num_blocks_h-1 )
			max_x = init_x + block_size_h;			
		else
			max_x = int(buffer_.h_resolution_);
			
		if( (work_block / num_blocks_h) < num_blocks_v-1 )
			max_y = init_y + block_size_v;
		else
			max_y = int(buffer_.v_resolution_);


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

				buffer_.buffer_data_[x][y] = glm::clamp(buffer_.buffer_data_[x][y], 
														glm::vec3{0.0f, 0.0f, 0.0f}, 
														glm::vec3{1.0f, 1.0f, 1.0f});

				progress[thread_id]++;
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
	ONB rotation;
	float n_in = 1.0f;
	float n_out = 1.458f;
	bool in_out;

	if(depth < 10 && scene_.intersect( r, intersection_record ) ){


		/////////////////////////////////////////////////
		// DESCOBRIR SE O RAIO ESTÁ ENTRANDO OU SAINDO //
		/////////////////////////////////////////////////
		glm::vec3 n = intersection_record.normal_;
		float cosThetaIn = -glm::dot(r.direction_, n);
		in_out = (cosThetaIn < 0) ? OUT : IN;
		cosThetaIn = (cosThetaIn < 0) ? -cosThetaIn : cosThetaIn;
		/////////////////////////////////////////////////

		Material material = Object::material_list[intersection_record.object->material_index];

		if(!material.brdf_pointer && !material.btdf_pointer){
			Lo = material.emittance_;
		}

		/////////////////////////////////
		// MATERIAL APENAS REFLETE LUZ //
		/////////////////////////////////
		else if( !material.btdf_pointer ){

			if(material.mode == Material::UNIFORM){

				float theta, phi, x, y, z;
				
				theta = dist_theta(generator);
				phi = acos(dist_phi(generator));
				
				x = sin(phi) * cos(theta);
				z = sin(phi) * sin(theta);
				y = cos(phi);

				glm::vec3 new_ray = glm::vec3(x, y, z);

				rotation.setFromV(n);
				new_ray = rotation.getBasisMatrix() * new_ray;

				float cosTheta = glm::dot(new_ray, n);
				

				Ray reflect{ intersection_record.position_ + 0.001f * n, new_ray };

				Lo = material.emittance_ +
					 2.0f * float(M_PI) * 
					 material.brdf() * 
					 L(reflect, ++depth, dist_theta, dist_phi, generator) * 
					 cosTheta;
			}

			else if(material.mode == Material::DIRECTIONAL){

				glm::vec3 new_ray = r.direction_ - 
									2.0f * n * 
									glm::dot(r.direction_, n);		

				Ray reflect{ intersection_record.position_ + 0.001f * n, new_ray };

				Lo = material.emittance_ +
					 material.brdf() * 
					 L(reflect, ++depth, dist_theta, dist_phi, generator);
			}

		}


		//////////////////////////////////////
		// MATERIAL REFLETE E REFRATA A LUZ //
		//////////////////////////////////////
		else{

			if(in_out == OUT){
				std::swap(n_in, n_out);
				n = -n;
			}

			float cosThetaOut2 = 1 - (float)pow( n_in / n_out , 2) * (1 - (float)pow(cosThetaIn, 2));
			float Ro = pow( (n_in - n_out) / (n_in + n_out), 2.0f);
			float fresnel;

			if(n_in < n_out){
				fresnel = Ro + (1.0f - Ro) * pow(1.0f - cosThetaIn, 5.0f);  // Fresnel correto
			}
			else{
				float cosThetaOut = (cosThetaOut2 >= 0.0f) ? sqrt(cosThetaOut2) : -1.0f;
				fresnel = (cosThetaOut < 0.0f) ? 1.0f : (Ro + (1.0f - Ro) * pow(1.0f - cosThetaOut, 5.0f));
			}

			//////////////
			// REFLEXÃO //
			//////////////
			if( rand() / (float)RAND_MAX < fresnel){
				
				glm::vec3 new_ray = r.direction_ - 
									2.0f * 
									n * 
									glm::dot(r.direction_, n);

				Ray reflect{ intersection_record.position_ + 0.001f * n, new_ray };

				Lo = material.emittance_ +
					 material.brdf() * 
					 L(reflect, ++depth, dist_theta, dist_phi, generator);
			}

			//////////////
			// REFRAÇÃO //
			//////////////
			else{

				glm::vec3 new_ray = (n_in / n_out) * 
									(cosThetaIn * n - r.direction_) -
									n * 
									(float)sqrt(cosThetaOut2);

				Ray reflect{ intersection_record.position_ - 0.001f * n, new_ray };

				Lo = material.emittance_ +
					 L(reflect, ++depth, dist_theta, dist_phi, generator);

			}


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
	int total_progress;
	int num_pixels = buffer_.h_resolution_ * buffer_.v_resolution_;

	while(true){

		work_block = block;

		completed_blocks = work_block > num_threads-1 ? work_block-num_threads : 0;

		total_progress = 0;
		for(int i = 0; i < num_threads; i++)
			total_progress += progress[i];

		std::stringstream progress_stream;
		progress_stream << "\rProgress .........................: "
						<< std::fixed << std::setw( 6 )
						<< std::setprecision( 2 )
						<< 100.0 * total_progress / num_pixels
						<< "% | "
						<< completed_blocks << "/" << num_blocks;

		if(total_progress){

			prev_total_time = num_pixels * difftime(time(NULL), initial_time) / total_progress;
			remaining_time =  prev_total_time - difftime(time(NULL), initial_time);

			progress_stream << " | "
							<< remaining_time/3600 << " h " 
			 				<< (remaining_time/60)%60 << " min "
			 				<< remaining_time%60 << " s   ";
		}

		std::clog << progress_stream.str();

		if(work_block == num_blocks+num_threads) break;

		//std::this_thread::sleep_for (std::chrono::seconds(prev_total_time * 0.05 < 2 ? 2 : prev_total_time * 0.05 > 5 ? 5 : int(prev_total_time * 0.05)));
		std::this_thread::sleep_for (std::chrono::seconds(2));
	}	
	
	std::clog << std::endl;	
}
