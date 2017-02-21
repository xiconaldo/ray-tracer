#include "perspective_camera.h"

PerspectiveCamera::PerspectiveCamera( void )
{}

PerspectiveCamera::PerspectiveCamera( const float min_x,
                                        const float max_x,
                                        const float min_y,
                                        const float max_y,
                                        const glm::ivec2 &resolution,
                                        const glm::vec3 &position,
                                        const glm::vec3 &up_vector,
                                        const glm::vec3 &look_at,
                                        const float s ) :
        Camera::Camera{ resolution, 
                        position,
                        up_vector,
                        look_at },
        min_x_{ min_x },
        max_x_{ max_x },
        min_y_{ min_y },
        max_y_{ max_y },
        s_{s}
{}

Ray PerspectiveCamera::getWorldSpaceRay( const glm::vec2 &pixel_coord ) const
{
    float width = max_x_ - min_x_;
    float height = max_y_ - min_y_;

    glm::vec3 a{width, 0.0f, 0.0f};
    glm::vec3 b{0.0f, -height, 0.0f};
    glm::vec3 c{min_x_, max_y_, -s_};

    float u = pixel_coord.x/resolution_.x;
    float v = pixel_coord.y/resolution_.y;

    glm::vec3 vec_s = c + u*a + v*b;
    glm::vec3 vec_e{0.0f, 0.0f, 0.0f};

    return Ray{ position_, 
                glm::normalize( onb_.getBasisMatrix() * (vec_s - vec_e) ) };
}

