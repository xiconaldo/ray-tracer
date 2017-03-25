#include "main.h"

int main( void )
{
	unsigned int x_resolution = 1024;
	unsigned int y_resolution = 1024;

	// OrthographicCamera camera{  -1.0f, 
	// 							 1.0f, 
	// 						    -1.0f, 
	// 							 1.0f,
	// 							 glm::ivec2{ x_resolution, y_resolution }, 
	// 							 glm::vec3{ 0.0f, 0.0f,  1.0f },	 // position
	// 							 glm::vec3{ 0.0f, 1.0f,  0.0f },	 // up
	// 							 glm::vec3{ 0.0f, 0.0f, -1.0f } };   // look at
	
	PerspectiveCamera camera{  -1.0f, 
								1.0f, 
							   -1.0f, 
								1.0f,
								glm::ivec2{ x_resolution, y_resolution }, 
								glm::vec3{ 0.0f, 0.0f,  0.0f },	 // position
								glm::vec3{ 0.0f, 1.0f,  0.0f },	 // up
								glm::vec3{ 0.0f, 0.0f, -1.0f },   // look at
                                1.0f};
	Scene scene{};
	
	scene.load();

	Buffer rendering_buffer{ x_resolution, y_resolution };
	glm::vec3 background_color{ 0.0f, 0.0f, 0.0f };

	// Set up the renderer.
	RayTracer rt( camera,
				  scene,
				  background_color,
				  rendering_buffer );

	// rt.integrate(); // Renders the final image.
	
	const int num_threads = 4;
	std::thread* threads[num_threads];
	
	for(int i = 0; i < num_threads; i++)
		threads[i] = new std::thread{&RayTracer::integrate, &rt, i, num_threads};

	for(int i = 0; i < num_threads; i++){
		threads[i]->join();
		delete threads[i];
	}
	
	// Save the rendered image to a .ppm file.
	rendering_buffer.save( "output_image.ppm" );

	return EXIT_SUCCESS;
}

