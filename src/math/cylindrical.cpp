//
// Created by Willie on 2022/10/3.
//
#include "cylindrical.h"
#include "vector3.h"

 Cylindrical& Cylindrical::setFromVector3( Vector3& v ) {
    return setFromCartesianCoords( v.x, v.y, v.z );
 }


