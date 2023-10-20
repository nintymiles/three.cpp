//
// Created by Willie on 2022/10/3.
//

#ifndef THREE_CPP_CYLINDRICAL_H
#define THREE_CPP_CYLINDRICAL_H

#include <cmath>

/**
 * @author Mugen87 / https://github.com/Mugen87
 *
 * Ref: https://en.wikipedia.org/wiki/Cylindrical_coordinate_system
 *
 */
class Vector3;

 class Cylindrical{
 public:
     double radius = 1.0,theta = 0,y = 0;

     Cylindrical(double radius,double theta,double y):radius(radius),theta(theta),y(y){};

     Cylindrical& set(double r,double t,double y){
         radius = r;
         theta = t;
         this->y = y;

         return *this;
     }

//     clone: function () {
//
//             return new this.constructor().copy( this );
//
//     }
//
//     copy: function ( other ) {
//         this.radius = other.radius;
//         this.theta = other.theta;
//         this.y = other.y;
//
//         return this;
//     }

    Cylindrical& clone(Cylindrical* c);

    Cylindrical& copy(const Cylindrical& other);

     Cylindrical& setFromVector3 ( Vector3& v );

     Cylindrical& setFromCartesianCoords( double x,double y,double z ) {
         this->radius = sqrt( x * x + z * z );
         this->theta = atan2( x, z );
         this->y = y;

         return *this;
     }

 };


#endif //THREE_CPP_CYLINDRICAL_H
