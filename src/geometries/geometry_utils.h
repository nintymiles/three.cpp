//
// Created by SeanR on 2023/11/6.
//

#ifndef THREE_CPP_GEOMETRY_UTILS_H
#define THREE_CPP_GEOMETRY_UTILS_H

#include "vector3.h"

namespace geometry_utils{

     /**
     * Generates 3D-Coordinates in a very fast way.
     *
     * Based on work by:
     * @link https://openprocessing.org/user/5654
     *
     * @param center     Center of Hilbert curve.
     * @param size       Total width of Hilbert curve.
     * @param iterations Number of subdivisions.
     * @param v0         Corner index -X, +Y, -Z.
     * @param v1         Corner index -X, +Y, +Z.
     * @param v2         Corner index -X, -Y, +Z.
     * @param v3         Corner index -X, -Y, -Z.
     * @param v4         Corner index +X, -Y, -Z.
     * @param v5         Corner index +X, -Y, +Z.
     * @param v6         Corner index +X, +Y, +Z.
     * @param v7         Corner index +X, +Y, -Z.
     */
     std::vector<Vector3> hilbert3D(Vector3 center = Vector3{0,0,0},size_t size = 10, int iterations = 1,
                                    int v0=0,int v1=1,int v2=2,int v3=3,int v4=4,int v5=5,int v6=6,int v7=7);


}

#endif //THREE_CPP_GEOMETRY_UTILS_H
