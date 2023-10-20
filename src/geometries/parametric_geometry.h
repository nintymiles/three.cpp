//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_PARAMETRIC_GEOMETRY_H
#define THREE_CPP_PARAMETRIC_GEOMETRY_H

#include "buffer_geometry.h"

class ParametricBufferGeometry : public BufferGeometry {
protected:
    std::function<Vector3& (float, float, Vector3&)> func;
    float slices;
    float stacks;
public:
    using ptr = std::shared_ptr<ParametricBufferGeometry>;

    ParametricBufferGeometry(std::function<Vector3&(float,float,Vector3&)> func, int slices, float stacks);
};

class ParametricGeometry : public Geometry {
protected:
    std::function<Vector3& (float, float, Vector3&)> func;
    float slices;
    float stacks;
public:
    using ptr = std::shared_ptr<ParametricGeometry>;

    ParametricGeometry(std::function<Vector3& (float, float, Vector3&)> func, int slices, float stacks);
};

#endif //THREE_CPP_PARAMETRIC_GEOMETRY_H
