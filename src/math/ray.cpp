//
// Created by SeanR on 2022/6/12.
//

#include "ray.h"

#include "box3.h"
#include "plane.h"

namespace ray__{
    // const _buffer_attribute_vector = /*@__PURE__*/ new Vector3();
    // const _segCenter = /*@__PURE__*/ new Vector3();
    // const _segDir = /*@__PURE__*/ new Vector3();
    // const _diff = /*@__PURE__*/ new Vector3();
    static Vector3 _vector;
    static Vector3 _segCenter;
    static Vector3 _segDir;
    static Vector3 _diff;

    // const _edge1 = /*@__PURE__*/ new Vector3();
    // const _edge2 = /*@__PURE__*/ new Vector3();
    // const _normal = /*@__PURE__*/ new Vector3();
    static Vector3 _edge1;
    static Vector3 _edge2;
    static Vector3 _normal;
}

using namespace ray__;

Ray& Ray::recast(double t) {
    origin.copy(at( t, _vector ));

    return *this;
}

double Ray::distanceSqToPoint(Vector3& point) {
    const double directionDistance = _vector.subVectors( point, origin ).dot( direction );

    // point behind the ray
    if ( directionDistance < 0 ) {
        return origin.distanceToSquared( point );
    }

    _vector.copy( direction ).multiplyScalar( directionDistance ).add( origin );

    return _vector.distanceToSquared( point );
}

double Ray::distanceSqToSegment(Vector3& v0,Vector3& v1,std::shared_ptr<Vector3> optionalPointOnRay,std::shared_ptr<Vector3> optionalPointOnSegment) {
    // from https://github.com/pmjoniak/GeometricTools/blob/master/GTEngine/Include/Mathematics/GteDistRaySegment.h
    // It returns the min distance between the ray and the segment
    // defined by v0 and v1
    // It can also set two optional targets :
    // - The closest point on the ray
    // - The closest point on the segment

    _segCenter.copy( v0 ).add( v1 ).multiplyScalar( 0.5 );
    _segDir.copy( v1 ).sub( v0 ).normalize();
    _diff.copy( origin ).sub( _segCenter );

    const double segExtent = v0.distanceTo( v1 ) * 0.5;
    const double a01 = - direction.dot( _segDir );
    const double b0 = _diff.dot( direction );
    const double b1 = - _diff.dot( _segDir );
    const double c = _diff.lengthSq();
    const double det = abs( 1 - a01 * a01 );
    double s0, s1, sqrDist, extDet;

    if ( det > 0 ) {

        // The ray and segment are not parallel.

        s0 = a01 * b1 - b0;
        s1 = a01 * b0 - b1;
        extDet = segExtent * det;

        if ( s0 >= 0 ) {

            if ( s1 >= - extDet ) {

                if ( s1 <= extDet ) {

                    // region 0
                    // Minimum at interior points of ray and segment.

                    const double invDet = 1 / det;
                    s0 *= invDet;
                    s1 *= invDet;
                    sqrDist = s0 * ( s0 + a01 * s1 + 2 * b0 ) + s1 * ( a01 * s0 + s1 + 2 * b1 ) + c;

                } else {

                    // region 1

                    s1 = segExtent;
                    s0 = fmax( 0, - ( a01 * s1 + b0 ) );
                    sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

                }

            } else {

                // region 5

                s1 = - segExtent;
                s0 = fmax( 0, - ( a01 * s1 + b0 ) );
                sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

            }

        } else {

            if ( s1 <= - extDet ) {
                // region 4
                s0 = fmax( 0, - ( - a01 * segExtent + b0 ) );
                s1 = ( s0 > 0 ) ? - segExtent : fmin( fmax( - segExtent, - b1 ), segExtent );
                sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

            } else if ( s1 <= extDet ) {
                // region 3
                s0 = 0;
                s1 = fmin( fmax( - segExtent, - b1 ), segExtent );
                sqrDist = s1 * ( s1 + 2 * b1 ) + c;

            } else {
                // region 2
                s0 = fmax( 0, - ( a01 * segExtent + b0 ) );
                s1 = ( s0 > 0 ) ? segExtent : fmin( fmax( - segExtent, - b1 ), segExtent );
                sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

            }

        }

    } else {
        // Ray and segment are parallel.
        s1 = ( a01 > 0 ) ? - segExtent : segExtent;
        s0 = fmax( 0, - ( a01 * s1 + b0 ) );
        sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

    }

    if ( optionalPointOnRay != nullptr ) {
        optionalPointOnRay->copy( direction ).multiplyScalar( s0 ).add( origin );
    }

    if ( optionalPointOnSegment != nullptr ) {
        optionalPointOnSegment->copy( _segDir ).multiplyScalar( s1 ).add( _segCenter );
    }

    return sqrDist;
}

bool Ray::intersectSphere( Sphere& sphere, Vector3& target ) {
    _vector.subVectors( sphere.center, origin );
    const double tca = _vector.dot( direction );
    const double d2 = _vector.dot( _vector ) - tca * tca;
    const double radius2 = sphere.radius * sphere.radius;

    if ( d2 > radius2 ) return false;

    const double thc = sqrt( radius2 - d2 );

    // t0 = first intersect point - entrance on front of sphere
    const double t0 = tca - thc;

    // t1 = second intersect point - exit point on back of sphere
    const double t1 = tca + thc;

    // test to see if both t0 and t1 are behind the ray - if so, return null
    if ( t0 < 0 && t1 < 0 ) return false;

    // test to see if t0 is behind the ray:
    // if it is, the ray is inside the sphere, so return the second exit point scaled by t1,
    // in order to always return an intersect point that is in front of the ray.
    if ( t0 < 0 ) {
        at(t1, target);
        return true;
    }

    // else t0 is in front of the ray, so return the first collision point scaled by t0
    at( t0, target );
    return true;
}

bool Ray::intersectBox( Box3& box, Vector3& target ) {
    double tmin, tmax, tymin, tymax, tzmin, tzmax;

    const double invdirx = 1 / direction.x,
            invdiry = 1 / direction.y,
            invdirz = 1 / direction.z;

    const Vector3 origin = this->origin;

    if ( invdirx >= 0 ) {
        tmin = ( box.min.x - origin.x ) * invdirx;
        tmax = ( box.max.x - origin.x ) * invdirx;
    } else {
        tmin = ( box.max.x - origin.x ) * invdirx;
        tmax = ( box.min.x - origin.x ) * invdirx;
    }

    if ( invdiry >= 0 ) {
        tymin = ( box.min.y - origin.y ) * invdiry;
        tymax = ( box.max.y - origin.y ) * invdiry;
    } else {
        tymin = ( box.max.y - origin.y ) * invdiry;
        tymax = ( box.min.y - origin.y ) * invdiry;
    }

    if ( ( tmin > tymax ) || ( tymin > tmax ) ) return false;

    // These lines also handle the case where tmin or tmax is NaN
    // (result of 0 * Infinity). x !== x returns true if x is NaN

    if ( tymin > tmin || tmin != tmin ) tmin = tymin;

    if ( tymax < tmax || tmax != tmax ) tmax = tymax;

    if ( invdirz >= 0 ) {
        tzmin = ( box.min.z - origin.z ) * invdirz;
        tzmax = ( box.max.z - origin.z ) * invdirz;
    } else {
        tzmin = ( box.max.z - origin.z ) * invdirz;
        tzmax = ( box.min.z - origin.z ) * invdirz;
    }

    if ( ( tmin > tzmax ) || ( tzmin > tmax ) ) return false;

    if ( tzmin > tmin || tmin != tmin ) tmin = tzmin;

    if ( tzmax < tmax || tmax != tmax ) tmax = tzmax;

    //return point closest to the ray (positive side)

    if ( tmax < 0 ) return false;

    this->at( tmin >= 0 ? tmin : tmax, target );

    return true;
}

bool Ray::intersectsBox( Box3& box ) {
    return intersectBox( box, _vector );
}

bool Ray::intersectTriangle( Vector3& a, Vector3& b, Vector3& c, bool backfaceCulling, Vector3& target ) {

    // Compute the offset origin, edges, and normal.

    // from https://github.com/pmjoniak/GeometricTools/blob/master/GTEngine/Include/Mathematics/GteIntrRay3Triangle3.h
    _edge1.subVectors( b, a );
    _edge2.subVectors( c, a );
    _normal.crossVectors( _edge1, _edge2 );

    // Solve Q + t*D = b1*E1 + b2*E2 (Q = kDiff, D = ray direction,
    // E1 = kEdge1, E2 = kEdge2, N = Cross(E1,E2)) by
    //   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
    //   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
    //   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
    auto DdN = this->direction.dot( _normal );
    int sign;

    if ( DdN > 0 ) {
        if ( backfaceCulling ) return false;
        sign = 1;
    } else if ( DdN < 0 ) {
        sign = - 1;
        DdN = - DdN;
    } else {
        return false;
    }

    _diff.subVectors( this->origin, a );
    auto DdQxE2 = sign * this->direction.dot( _edge2.crossVectors( _diff, _edge2 ) );

    // b1 < 0, no intersection
    if ( DdQxE2 < 0 ) {
        return false;
    }

    auto DdE1xQ = sign * this->direction.dot( _edge1.cross( _diff ) );

    // b2 < 0, no intersection
    if ( DdE1xQ < 0 ) {
        return false;
    }

    // b1+b2 > 1, no intersection
    if ( DdQxE2 + DdE1xQ > DdN ) {
        return false;
    }

    // Line intersects triangle, check if ray does.
    auto QdN = - sign * _diff.dot( _normal );

    // t < 0, no intersection
    if ( QdN < 0 ) {
        return false;
    }

    // Ray intersects triangle.
    this->at( QdN / DdN, target );
    return true;

}

float Ray::distanceToPlane( Plane& plane ) {

    auto denominator = plane.normal.dot( this->direction );

    if ( denominator == 0 ) {
        // line is coplanar, return origin
        if ( plane.distanceToPoint( this->origin ) == 0 ) {
            return 0;
        }

        // Null is preferable to undefined since undefined means.... it is undefined
        return 0;
    }

    auto t = - ( this->origin.dot( plane.normal ) + plane.constant ) / denominator;

    // Return if the ray never intersects the plane

    return t >= 0 ? t : 0;

}

bool Ray::intersectPlane( Plane& plane, Vector3& target ) {
    auto t = this->distanceToPlane( plane );

    if ( t == 0 ) {
        return false;
    }

    this->at( t, target );
    return true;
}

bool Ray::intersectsPlane( Plane& plane ) {

    // check if the ray lies on the plane first
    auto distToPoint = plane.distanceToPoint( this->origin );

    if ( distToPoint == 0 ) {
        return true;
    }

    const auto denominator = plane.normal.dot( this->direction );

    if ( denominator * distToPoint < 0 ) {
        return true;
    }

    // ray origin is behind the plane (and is pointing behind it)
    return false;

}


