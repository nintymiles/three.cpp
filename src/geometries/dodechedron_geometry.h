//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_DODECHEDRON_GEOMETRY_H
#define THREE_CPP_DODECHEDRON_GEOMETRY_H

#include "polyhedron_geometry.h"

class DodecahedronBufferGeometry : public PolyhedronBufferGeometry {
public:
    float radius;
    float detail;
    using sptr = std::shared_ptr<DodecahedronBufferGeometry>;
    DodecahedronBufferGeometry(float radius = 0, float detail = 0);

    static const sptr create(float radius = 0, float detail = 0) {
        return std::make_shared<DodecahedronBufferGeometry>(radius, detail);
    }
};

class DodecahedronGeometry : public Geometry {
public:
    using sptr = std::shared_ptr<DodecahedronGeometry>;
    float radius;
    float detail;
    DodecahedronGeometry(float radius = 0, float detail = 0);

    static const sptr create(float radius = 0, float detail = 0) {
        return std::make_shared<DodecahedronGeometry>(radius, detail);
    }
};

#endif //THREE_CPP_DODECHEDRON_GEOMETRY_H
