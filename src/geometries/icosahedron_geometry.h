//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_ICOSAHEDRON_GEOMETRY_H
#define THREE_CPP_ICOSAHEDRON_GEOMETRY_H

#include "polyhedron_geometry.h"

/***
 * twenty faces polyhedron
 */
class IcosahedronBufferGeometry : public PolyhedronBufferGeometry {
public:
    using ptr = std::shared_ptr<IcosahedronBufferGeometry>;
    IcosahedronBufferGeometry(float radius, float detail = std::numeric_limits<float>::quiet_NaN());
};
class IcosahedronGeometry : public Geometry {
public :
    using ptr = std::shared_ptr<IcosahedronGeometry>;
    IcosahedronGeometry(float radius, float detail = std::numeric_limits<float>::quiet_NaN());
};

#endif //THREE_CPP_ICOSAHEDRON_GEOMETRY_H
