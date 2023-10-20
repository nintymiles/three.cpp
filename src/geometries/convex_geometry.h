//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_CONVEX_GEOMETRY_H
#define THREE_CPP_CONVEX_GEOMETRY_H

#include "buffer_attribute.h"
#include "buffer_geometry.h"


class ConvexBufferGeometry : public BufferGeometry {
public:
    ConvexBufferGeometry() : BufferGeometry() {}

    ConvexBufferGeometry(std::vector<Vector3>& points);
};

class ConvexGeometry : public Geometry {
public:
    ConvexGeometry() : Geometry() {}

    ConvexGeometry(std::vector<Vector3>& points);
};


#endif //THREE_CPP_CONVEX_GEOMETRY_H
