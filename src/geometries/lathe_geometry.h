//
// Created by SeanR on 2023/9/17.
//

#ifndef THREE_CPP_LATHE_GEOMETRY_H
#define THREE_CPP_LATHE_GEOMETRY_H

#include "buffer_geometry.h"

class LatheBufferGeometry : public BufferGeometry {
protected:
    std::vector<Vector3> points;
    float segments;
    float phiStart;
    float phiLength;
public:
    using ptr = std::shared_ptr<LatheBufferGeometry>;
    LatheBufferGeometry(const std::vector<Vector3>& points, float segments = std::numeric_limits<float>::quiet_NaN(), float phiStart = std::numeric_limits<float>::quiet_NaN(), float phiLength = std::numeric_limits<float>::quiet_NaN());

};
class LatheGeometry : public Geometry {
protected:
    std::vector<Vector3> points;
    float segments;
    float phiStart;
    float phiLength;
public:
    using ptr = std::shared_ptr<LatheGeometry>;
    LatheGeometry(const std::vector<Vector3>& points, float segments = std::numeric_limits<float>::quiet_NaN(), float phiStart = std::numeric_limits<float>::quiet_NaN(), float phiLength = std::numeric_limits<float>::quiet_NaN());
};

#endif //THREE_CPP_LATHE_GEOMETRY_H
