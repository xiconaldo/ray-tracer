#ifndef OBJECT_H_
#define OBJECT_H_

#include "primitive.h"
#include "material.h"

class Object
{
public:

	typedef std::unique_ptr< Object > ObjectUniquePtr;

    std::vector< Primitive::PrimitiveUniquePtr > primitives_;

    std::vector< Material > material_list;

};

#endif /* OBJECT_H_ */