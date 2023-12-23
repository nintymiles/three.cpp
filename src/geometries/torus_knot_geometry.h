//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_TORUS_KNOT_GEOMETRY_H
#define THREE_CPP_TORUS_KNOT_GEOMETRY_H

#include "buffer_geometry.h"

class TorusKnotGeometry : public BufferGeometry {
private :
    void calculatePositionOnCurve(float u, float p, float q, float radius, Vector3& position);
public:
    using sptr = std::shared_ptr<TorusKnotGeometry>;

    TorusKnotGeometry(float radius = 1.f, float tube = .4f, float tubularSegments = 64.f, float radialSegments = 8.f, float p = 2.f, float q = 3.f);

    static sptr create(float radius = 1.f, float tube = .4f, float tubularSegments = 64.f, float radialSegments = 8.f, float p = 2.f, float q = 3.f){
        return std::make_shared<TorusKnotGeometry>(radius,tube,tubularSegments,radialSegments,p,q);
    }
};

class TorusKnotGeometry2 : public Geometry {
public:
    using sptr = std::shared_ptr<TorusKnotGeometry2>;

    TorusKnotGeometry2(float radius = 0, float tube = 0, float tubularSegments = 0, float radialSegments = 0, float p = 0, float q = 0);
};

#endif //THREE_CPP_TORUS_KNOT_GEOMETRY_H
