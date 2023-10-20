//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_TORUS_KNOT_GEOMETRY_H
#define THREE_CPP_TORUS_KNOT_GEOMETRY_H

#include "buffer_geometry.h"

class TorusKnotBufferGeometry : public BufferGeometry {
private :
    void calculatePositionOnCurve(float u, float p, float q, float radius, Vector3& position);
public:
    using ptr = std::shared_ptr<TorusKnotBufferGeometry>;

    TorusKnotBufferGeometry(float radius = 0, float tube = 0, float tubularSegments = 0, float radialSegments = 0, float p = 0, float q = 0);
};

class TorusKnotGeometry : public Geometry {
public:
    using ptr = std::shared_ptr<TorusKnotGeometry>;

    TorusKnotGeometry(float radius = 0, float tube = 0, float tubularSegments = 0, float radialSegments = 0, float p = 0, float q = 0);
};

#endif //THREE_CPP_TORUS_KNOT_GEOMETRY_H
