//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_OCTAHEDRON_GEOMETRY_H
#define THREE_CPP_OCTAHEDRON_GEOMETRY_H

#include "polyhedron_geometry.h"

class OctahedronBufferGeometry : public PolyhedronBufferGeometry {
protected:
    float radius;
    float detail;
public:
    using sptr = std::shared_ptr<OctahedronBufferGeometry>;

    OctahedronBufferGeometry(float radius = 0, float detail = 0);
};

class OctahedronGeometry : public Geometry {
protected:
    float radius;
    float detail;
public:
    using sptr = std::shared_ptr<OctahedronGeometry>;

    OctahedronGeometry(float radius = 0, float detail = 0);
};
#endif //THREE_CPP_OCTAHEDRON_GEOMETRY_H
