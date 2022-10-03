//
// Created by Willie on 2022/10/3.
//

#ifndef THREE_CPP_SPHERICAL_H
#define THREE_CPP_SPHERICAL_H

#include "number.h"
#include "math_utils.h"

class Vector3;
/**
 * @author bhouston / http://clara.io
 * @author WestLangley / http://github.com/WestLangley
 *
 * Ref: https://en.wikipedia.org/wiki/Spherical_coordinate_system
 *
 * The polar angle (phi) is measured from the positive y-axis. The positive y-axis is up.
 * The azimuthal angle (theta) is measured from the positive z-axiz.
 */

class Spherical{
public:
    double radius,phi,theta;

    Spherical(double radius = 1.0,double phi = 0,double theta = 0):radius(radius),phi(phi),theta(theta){};

    Spherical& set( double radius, double phi, double theta ) {
        this->radius = radius;
        this->phi = phi;
        this->theta = theta;

        return *this;
    }

    // restrict phi to be betwee EPS and PI-EPS
    Spherical& makeSafe() {
        double EPS = Number::EPSILON;
        phi = fmax( EPS, fmin( Number::PI - EPS, phi ) );

        return *this;
    }

    Spherical& setFromVector3 ( Vector3& v );

    Spherical& setFromCartesianCoords ( double x, double y, double z ) {
        radius = sqrt( x * x + y * y + z * z );

        if ( radius == 0 ) {
            theta = 0;
            phi = 0;
        } else {
            theta = atan2( x, z );
            phi = acos( MathUtils::clamp<double>( y / radius, - 1, 1 ) );
        }

        return *this;
    }

};


//clone: function () {
//
//    return new this.constructor().copy( this );
//
//},
//
//copy: function ( other ) {
//
//        this.radius = other.radius;
//        this.phi = other.phi;
//        this.theta = other.theta;
//
//        return this;
//
//},





#endif //THREE_CPP_SPHERICAL_H
