//
// Created by Willie on 2022/10/2.
//

#include "triangle.h"
#include "math_variables.h"

//const _triangle_v0 = /*@__PURE__*/ new Vector3();
//const _triangle_v1 = /*@__PURE__*/ new Vector3();
//const _triangle_v2 = /*@__PURE__*/ new Vector3();
//const _triangle_v3 = /*@__PURE__*/ new Vector3();
Vector3 _triangle_v0,_triangle_v1,_triangle_v2,_triangle_v3;
//
//const _vab = /*@__PURE__*/ new Vector3();
//const _vac = /*@__PURE__*/ new Vector3();
//const _vbc = /*@__PURE__*/ new Vector3();
//const _vap = /*@__PURE__*/ new Vector3();
//const _vbp = /*@__PURE__*/ new Vector3();
//const _vcp = /*@__PURE__*/ new Vector3();
Vector3 _vab,_vac,_vbc,_vap,_vbp,_vcp;

Vector3& Triangle::getNormal( Vector3& a, Vector3& b, Vector3& c,Vector3& target ) {
    target.subVectors( c, b );
    _triangle_v0.subVectors(a, b );
    target.cross(_triangle_v0 );

    const double targetLengthSq = target.lengthSq();
    if ( targetLengthSq > 0 ) {
        return target.multiplyScalar( 1 / sqrt( targetLengthSq ) );
    }

    return target.set( 0, 0, 0 );
}


// static/instance method to calculate barycentric coordinates
// based on: http://www.blackpawn.com/texts/pointinpoly/default.html
Vector3& Triangle::getBarycoord( Vector3& point, Vector3& a, Vector3& b, Vector3& c, Vector3& target ){
    _triangle_v0.subVectors(c, a );
    _triangle_v1.subVectors(b, a );
    _triangle_v2.subVectors(point, a );

    const double dot00 = _triangle_v0.dot(_triangle_v0 );
    const double dot01 = _triangle_v0.dot(_triangle_v1 );
    const double dot02 = _triangle_v0.dot(_triangle_v2 );
    const double dot11 = _triangle_v1.dot(_triangle_v1 );
    const double dot12 = _triangle_v1.dot(_triangle_v2 );

    const double denom = ( dot00 * dot11 - dot01 * dot01 );

    // collinear or singular triangle
    if ( denom == 0 ) {
        // arbitrary location outside of triangle?
        // not sure if this is the best idea, maybe should be returning undefined
        return target.set( - 2, - 1, - 1 );
    }

    const double invDenom = 1 / denom;
    const double u = ( dot11 * dot02 - dot01 * dot12 ) * invDenom;
    const double v = ( dot00 * dot12 - dot01 * dot02 ) * invDenom;

    // barycentric coordinates must always sum to 1
    return target.set( 1 - u - v, v, u );
}

bool Triangle::containsPoint( Vector3& point,Vector3& a,Vector3& b,Vector3& c ){
    getBarycoord(point, a, b, c, _triangle_v3 );

    return (_triangle_v3.x >= 0 ) && (_triangle_v3.y >= 0 ) && ((_triangle_v3.x + _triangle_v3.y ) <= 1 );
}

Vector2& Triangle::getUV( Vector3& point, Vector3& p1, Vector3& p2, Vector3& p3, Vector2& uv1, Vector2& uv2, Vector2& uv3, Vector2& target ){
    getBarycoord(point, p1, p2, p3, _triangle_v3 );

    target.set( 0, 0 );
    target.addScaledVector(uv1, _triangle_v3.x );
    target.addScaledVector(uv2, _triangle_v3.y );
    target.addScaledVector(uv3, _triangle_v3.z );

    return target;
}

bool Triangle::isFrontFacing( Vector3& a, Vector3& b, Vector3& c, Vector3& direction ){
    _triangle_v0.subVectors(c, b );
    _triangle_v1.subVectors(a, b );

    // strictly front facing
    return (_triangle_v0.cross(_triangle_v1 ).dot(direction ) < 0 ) ? true : false;
}

double Triangle::getArea() {
    _triangle_v0.subVectors(c, b );
    _triangle_v1.subVectors(a, b );

    return _triangle_v0.cross(_triangle_v1 ).length() * 0.5;
}


