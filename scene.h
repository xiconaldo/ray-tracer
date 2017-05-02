#ifndef SCENE_H_
#define SCENE_H_

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "primitive.h"
#include "sphere.h" 
#include "triangle.h"
#include "loader.h"
#include "object.h"

class Scene
{
public:

	Scene( void );

	~Scene( void );

	bool intersect( const Ray &ray,
					IntersectionRecord &intersection_record ) const;

	void load( void );

	void loadObject(const std::string file_name, int material_index, glm::vec3 translation = glm::vec3{0.0f});

	std::vector< Primitive::PrimitiveUniquePtr > primitives_;

	std::vector< Object::ObjectUniquePtr > objects_;

};

#endif /* SCENE_H_ */

