#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include <memory>

#include "ray.h"
#include "intersection_record.h"

class Primitive
{
public:

    typedef std::unique_ptr< Primitive > PrimitiveUniquePtr;

    glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f);
    bool emiter = false;

    Primitive( void );

    virtual ~Primitive( void );

    virtual bool intersect( const Ray &ray,
                            IntersectionRecord &intersection_record ) const = 0;
};

#endif /* PRIMITIVE_H_ */

