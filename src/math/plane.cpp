//
// Created by SeanR on 2022/9/28.
//

#include "plane.h"
#include "matrix3.h"
#include "vector3.h"

Vector3 _vector1;
Vector3 _plane_vector;
Matrix3 _normalMatrix;

Plane& Plane::setFromCoplanarPoints( Vector3& a, Vector3& b, Vector3& c ) {
    Vector3 newNormal = _vector1.subVectors( c, b ).cross(_plane_vector.subVectors(a, b ) ).normalize();

    // Q: should an error be thrown if normal is zero (e.g. degenerate plane)?
    setFromNormalAndCoplanarPoint( newNormal, a );

    return *this;
}

Plane& Plane::applyMatrix4( Matrix4& matrix ) {
    // const Matrix3& normalMatrix =  _normalMatrix.getNormalMatrix( matrix );
    // const Vector3& referencePoint = coplanarPoint( _vector1 ).applyMatrix4( matrix );
    // const Vector3& normal = normal.applyMatrix3( normalMatrix ).normalize();

    // constant = - referencePoint.dot( normal );
    Matrix3& normalMatrix =  _normalMatrix.getNormalMatrix( matrix );

    return applyMatrix4(matrix,normalMatrix);
}

Plane& Plane::applyMatrix4( Matrix4& matrix,Matrix3& normalMatrix ) {
    Vector3& referencePoint = coplanarPoint( _vector1 ).applyMatrix4( matrix );

    normal.applyMatrix3( normalMatrix ).normalize();
    constant = - referencePoint.dot( normal );

    return *this;
}