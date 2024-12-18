//
// Created by Willie on 2022/10/3.
//
#include "cylindrical.h"

#include "vector3.h"

 Cylindrical& Cylindrical::setFromVector3( Vector3& v ) {
    return setFromCartesianCoords( v.x, v.y, v.z );
 }

Cylindrical& Cylindrical::clone(Cylindrical* c){
    c->copy(*this);
    return *this;
}

Cylindrical& Cylindrical::copy(const Cylindrical& other){
    radius = other.radius;
    theta = other.theta;
    y = other.y;

    return *this;
}


