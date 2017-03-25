#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include <sstream>
#include <iomanip>
#include <cmath>
#include <random>

#include "camera.h"
#include "scene.h"
#include "buffer.h"
#include <cmath>

class RayTracer
{
public:

    RayTracer( Camera &camera,
               const Scene &scene,
               const glm::vec3 background_color,
               Buffer &buffer );

    void integrate( int thread_id = 0,
    				int num_threads = 1 );

    glm::vec3 L(const Ray& r, int depth);

private:

    const Camera &camera_;

    const Scene &scene_;

    glm::dvec3 background_color_;

    Buffer &buffer_;

};

#endif /* RAYTRACER_H_ */

