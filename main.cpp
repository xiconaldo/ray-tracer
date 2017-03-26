#include "main.h"

int main( void )
{
	unsigned int x_resolution = 512;
	unsigned int y_resolution = 512;
	
	PerspectiveCamera camera{  -1.0f, 
								1.0f, 
							   -1.0f, 
								1.0f,
								glm::ivec2{ x_resolution, y_resolution }, 
								glm::vec3{ 0.0f, 0.0f,  2.0f },	 // position
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
	
	const int num_threads = 4;
	const int num_rays = 100;

	rt.integrate( num_threads, num_rays); // Renders the final image.
	
	// Save the rendered image to a .ppm file.
	rendering_buffer.save( "images/output_image.ppm" );

	return EXIT_SUCCESS;
}

