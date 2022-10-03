//
// Created by Willie on 2022/10/3.
//
#include "spherical.h"
#include "vector3.h"

Spherical& Spherical::setFromVector3 ( Vector3& v ) {
    return setFromCartesianCoords( v.x, v.y, v.z );
}
