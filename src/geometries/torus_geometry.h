//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_TORUS_GEOMETRY_H
#define THREE_CPP_TORUS_GEOMETRY_H

#include "buffer_geometry.h"

class TorusGeometry : public BufferGeometry {
public:
    using sptr = std::shared_ptr<TorusGeometry>;

    TorusGeometry(float radius = 0, float tube = 0, float radialSegments = 0, float tubularSegments = 0, float arc = 0);

    static sptr create(float radius = 0, float tube = 0, float radialSegments = 0, float tubularSegments = 0, float arc = 0){
        return std::make_shared<TorusGeometry>(radius,tube,radialSegments,tubularSegments,arc);
    }
};


#endif //THREE_CPP_TORUS_GEOMETRY_H
