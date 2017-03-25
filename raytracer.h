#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include <sstream>
#include <iomanip>
#include <cmath>
#include <random>
#include <atomic>
#include <chrono>
#include <thread>

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

    void integrate( void );

    glm::vec3 L(const Ray& r, int depth);

    void print_progress();

private:

    const Camera &camera_;

    const Scene &scene_;

    glm::dvec3 background_color_;

    Buffer &buffer_;

    std::atomic<int> block{0};

};

#endif /* RAYTRACER_H_ */

