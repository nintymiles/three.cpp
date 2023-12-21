//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_TETRAHEDRON_GEOMETRY_H
#define THREE_CPP_TETRAHEDRON_GEOMETRY_H

#include "polyhedron_geometry.h"

class TetrahedronGeometry : public PolyhedronBufferGeometry {
public:
    using sptr = std::shared_ptr<TetrahedronGeometry>;

    TetrahedronGeometry(float radius = 0, float detail = 0);

    static sptr create(float radius = 0, float detail = 0){
        return std::make_shared<TetrahedronGeometry>(radius,detail);
    }
};

#endif //THREE_CPP_TETRAHEDRON_GEOMETRY_H
