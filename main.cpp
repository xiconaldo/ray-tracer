#include "main.h"

int main( int argc, char* argv[] )
{
	unsigned int x_resolution = 512;
	unsigned int y_resolution = 512;
	int spp = 10;
	int num_threads = 4;
	std::string output_file = "output_image.ppm";

	if(argc > 1)
		x_resolution = strtol(argv[1], NULL, 10);
	if(argc > 2)
		y_resolution = strtol(argv[2], NULL, 10);
	if(argc > 3)
		spp = strtol(argv[3], NULL, 10);
	if(argc > 4)
		num_threads = strtol(argv[4], NULL, 10);
	if(argc > 5)
		output_file = std::string(argv[5]) + ".ppm";

	// MONKEY
	PerspectiveCamera camera{  -1.0f,
								1.0f,
							   -1.0f,
								1.0f,
								glm::ivec2{ x_resolution, y_resolution },
								glm::vec3{ 0.0f, 0.0f,  0.0f },	 // position
								glm::vec3{ 0.0f, 1.0f,  0.0f },	 // up
								glm::vec3{ 0.0f, 0.0f, -1.0f },   // look at
 								1.0f};


 	// CORNELL BOX
 // 	PerspectiveCamera camera{  -1.25f * x_resolution / y_resolution,
	// 							1.25f * x_resolution / y_resolution,
	// 						   -1.25f,
	// 							1.25f,
	// 							glm::ivec2{ x_resolution, y_resolution },
	// 							glm::vec3{ 0.0f, 1.0f,  2.5f },	 // position
	// 							glm::vec3{ 0.0f, 1.0f,  0.0f },	 // up
	// 							glm::vec3{ 0.0f, 1.0f, -1.0f },   // look at
	// 							2.0f};


	Scene scene{};

	scene.load();

	Buffer rendering_buffer{ x_resolution, y_resolution };
	glm::vec3 background_color{ 1.0f, 1.0f, 1.0f };

	// Set up the renderer.
	RayTracer rt( camera,
				  scene,
				  background_color,
				  rendering_buffer );

	rt.integrate( num_threads, spp); // Renders the final image.

	// Save the rendered image to a .ppm file.
	rendering_buffer.save( output_file );

	return EXIT_SUCCESS;
}
