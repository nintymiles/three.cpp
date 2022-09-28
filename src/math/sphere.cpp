//
// Created by SeanR on 2022/9/26.
//
#include "sphere.h"

#include "box3.h"
#include "vector3.h"
#include "matrix4.h"

Box3 _box;
Vector3 _sphere_v1;
Vector3 _toFarthestPoint;
Vector3 _toPoint;

Sphere& Sphere::setFromPoints(vector<Vector3>& points, shared_ptr<Vector3> optionalCenter) {
    if(optionalCenter != nullptr){
        center.copy( *optionalCenter );
    }else {
        _box.setFromPoints( points ).getCenter( center );
    }

    double maxRadiusSq = 0;

    for ( int i = 0, il = points.size(); i < il; i ++ ) {
        maxRadiusSq = fmax( maxRadiusSq, center.distanceToSquared( points[ i ] ) );
    }

    radius = sqrt( maxRadiusSq );

    return *this;
}

Sphere& Sphere::applyMatrix4(Matrix4& matrix){
    center.applyMatrix4( matrix );
    radius = radius * matrix.getMaxScaleOnAxis();

    return *this;
}

Sphere& Sphere::expandByPoint( Vector3& point ) {
    // from https://github.com/juj/MathGeoLib/blob/2940b99b99cfe575dd45103ef20f4019dee15b54/src/Geometry/Sphere.cpp#L649-L671
    _toPoint.subVectors( point, center );

    const double lengthSq = _toPoint.lengthSq();
    if ( lengthSq > ( radius * radius ) ) {

        const double length = sqrt( lengthSq );
        const double missingRadiusHalf = ( length - radius ) * 0.5;

        // Nudge this sphere towards the target point. Add half the missing distance to radius,
        // and the other half to position. This gives a tighter enclosure, instead of if
        // the whole missing distance were just added to radius.
        center.add( _toPoint.multiplyScalar( missingRadiusHalf / length ) );
        radius += missingRadiusHalf;

    }

    return *this;
}


Sphere& Sphere::unionSphere( Sphere& sphere ) {
    // from https://github.com/juj/MathGeoLib/blob/2940b99b99cfe575dd45103ef20f4019dee15b54/src/Geometry/Sphere.cpp#L759-L769

    // To enclose another sphere into this sphere, we only need to enclose two points:
    // 1) Enclose the farthest point on the other sphere into this sphere.
    // 2) Enclose the opposite point of the farthest point into this sphere.
    if ( center.equals( sphere.center ) == true ) {
        _toFarthestPoint.set( 0, 0, 1 ).multiplyScalar( sphere.radius );
    } else {
        _toFarthestPoint.subVectors( sphere.center, center ).normalize().multiplyScalar( sphere.radius );
    }

    expandByPoint(_sphere_v1.copy(sphere.center ).add(_toFarthestPoint ) );
    expandByPoint(_sphere_v1.copy(sphere.center ).sub(_toFarthestPoint ) );

    return *this;
}

bool Sphere::intersectsBox( Box3& box ){
    return box.intersectsSphere( *this );
}

Box3& Sphere::getBoundingBox( Box3& target ) {
    if ( isEmpty() ) {
        // Empty sphere produces empty bounding box
        target.makeEmpty();
        return target;
    }

    target.set( center, center );
    target.expandByScalar( radius );

    return target;
}

