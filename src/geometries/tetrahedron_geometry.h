//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_TETRAHEDRON_GEOMETRY_H
#define THREE_CPP_TETRAHEDRON_GEOMETRY_H

#include "polyhedron_geometry.h"

class TetrahedronBufferGeometry : public PolyhedronBufferGeometry {
public:
    using sptr = std::shared_ptr<TetrahedronBufferGeometry>;

    TetrahedronBufferGeometry(float radius = 0, float detail = 0);
};

class TetrahedronGeometry : public Geometry {
public:
    using sptr = std::shared_ptr<TetrahedronGeometry>;

    TetrahedronGeometry(float radius = 0, float detail = 0);
};
#endif //THREE_CPP_TETRAHEDRON_GEOMETRY_H
