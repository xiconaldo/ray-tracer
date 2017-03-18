#include "main.h"

int main( void )
{
	unsigned int x_resolution = 512;
	unsigned int y_resolution = 512;

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
	
	std::thread t0(&RayTracer::integrate, &rt, 0, 4);
	std::thread t1(&RayTracer::integrate, &rt, 1, 4);
	std::thread t2(&RayTracer::integrate, &rt, 2, 4);
	std::thread t3(&RayTracer::integrate, &rt, 3, 4);

	t0.join();
	t1.join();
	t2.join();
	t3.join();

	// Save the rendered image to a .ppm file.
	rendering_buffer.save( "output_image.ppm" );

	return EXIT_SUCCESS;
}

