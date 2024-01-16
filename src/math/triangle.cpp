//
// Created by Willie on 2022/10/2.
//

#include "triangle.h"

#include "Plane.h"
#include "Vector2.h"
#include "Box3.h"

namespace triangle {
Vector3 _v0;
Vector3 _v1;
Vector3 _v2;
Vector3 _v3;

Vector3 _vab;
Vector3 _vac;
Vector3 _vbc;
Vector3 _vap;
Vector3 _vbp;
Vector3 _vcp;
}
using namespace triangle;

Triangle& Triangle::set(Vector3 a, Vector3 b, Vector3 c){
    this->a.copy(a);
    this->b.copy(b);
    this->c.copy(c);

    return *this;
}

Triangle& Triangle::setFromPointsAndIndices(Vector3* points, unsigned i0, unsigned i1, unsigned i2){
    a.copy(points[i0]);
    b.copy(points[i1]);
    c.copy(points[i2]);

    return *this;
}

Triangle& Triangle::clone(Triangle* triangle){
    triangle->copy(*this);

    return *triangle;
}

Triangle& Triangle::copy(Triangle& triangle){
    a.copy(triangle.a);
    b.copy(triangle.b);
    c.copy(triangle.c);

    return *this;
}

float Triangle::getArea(){
    _v0.subVectors(c, b);
    _v1.subVectors(a, b);

    return _v0.cross(_v1).length() * 0.5f;
}

Vector3& Triangle::getMidpoint(Vector3* target){
    target->addVectors(a, b).add(c).multiplyScalar(1.0f / 3.0f);

    return *target;
}

Vector3& Triangle::getNormal(Vector3* target){
    return getNormal(a, b, c, *target);
}

Plane& Triangle::getPlane(Plane* target){
    target->setFromCoplanarPoints(a, b, c);

    return *target;
}

bool Triangle::containsPoint(Vector3& point, Vector3& a, Vector3& b, Vector3& c){
    getBarycoord(point, a, b, c, _v3);

    return (_v3.x >= 0) && (_v3.y >= 0) && ((_v3.x + _v3.y) <= 1);
}

Vector3& Triangle::getBarycoord(Vector3& point, Vector3* target){
    return getBarycoord(point,a, b, c, *target);
}

Vector2& Triangle::getUV(Vector3& point, Vector2& uv1, Vector2& uv2, Vector2& uv3, Vector2* target){
    return getUV(point, a, b, c, uv1, uv2, uv3, *target);

}

bool Triangle::containsPoint(Vector3& point){
    return containsPoint(point, a, b, c);
}

bool Triangle::intersectsBox(Box3& box3){
    return box3.intersectsTriangle(*this);
}

bool Triangle::isFrontFacing(Vector3& direction){
    _v0.subVectors(c, b);
    _v1.subVectors(a, b);

    // strictly front facing
    return (_v0.cross(_v1).dot(direction) < 0) ? true : false;
}

Vector3& Triangle::closestPointToPoint(Vector3& p, Vector3* target){

    float v, w;

    // algorithm thanks to Real-Time Collision Detection by Christer Ericson,
    // published by Morgan Kaufmann Publishers, (c) 2005 Elsevier Inc.,
    // under the accompanying license; see chapter 5.1.5 for detailed explanation.
    // basically, we're distinguishing which of the voronoi regions of the triangle
    // the point lies in with the minimum amount of redundant computation.

    _vab.subVectors(b, a);
    _vac.subVectors(c, a);
    _vap.subVectors(p, a);
    float d1 = _vab.dot(_vap);
    float d2 = _vac.dot(_vap);
    if (d1 <= 0 && d2 <= 0) {

        // vertex region of A; barycentric coords (1, 0, 0)
        return target->copy(a);

    }

    _vbp.subVectors(p, b);
    float d3 = _vab.dot(_vbp);
    float d4 = _vac.dot(_vbp);
    if (d3 >= 0 && d4 <= d3) {

        // vertex region of B; barycentric coords (0, 1, 0)
        return target->copy(b);

    }

    float vc = d1 * d4 - d3 * d2;
    if (vc <= 0 && d1 >= 0 && d3 <= 0) {

        v = d1 / (d1 - d3);
        // edge region of AB; barycentric coords (1-v, v, 0)
        return target->copy(a).addScaledVector(_vab, v);

    }

    _vcp.subVectors(p, c);
    float d5 = _vab.dot(_vcp);
    float d6 = _vac.dot(_vcp);
    if (d6 >= 0 && d5 <= d6) {

        // vertex region of C; barycentric coords (0, 0, 1)
        return target->copy(c);

    }

    float vb = d5 * d2 - d1 * d6;
    if (vb <= 0 && d2 >= 0 && d6 <= 0) {

        w = d2 / (d2 - d6);
        // edge region of AC; barycentric coords (1-w, 0, w)
        return target->copy(a).addScaledVector(_vac, w);

    }

    float va = d3 * d6 - d5 * d4;
    if (va <= 0 && (d4 - d3) >= 0 && (d5 - d6) >= 0) {

        _vbc.subVectors(c, b);
        w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        // edge region of BC; barycentric coords (0, 1-w, w)
        return target->copy(b).addScaledVector(_vbc, w); // edge region of BC

    }

    // face region
    float denom = 1 / (va + vb + vc);
    // u = va * denom
    v = vb * denom;
    w = vc * denom;

    return target->copy(a).addScaledVector(_vab, v).addScaledVector(_vac, w);
}

bool Triangle::equals(Triangle& triangle)
{
    return triangle.a.equals(a) && triangle.b.equals(b) && triangle.c.equals(c);
}

bool Triangle::isFrontFacing(Vector3& a, Vector3& b, Vector3& c, Vector3& direction){
    return false;
}

Vector2& Triangle::getUV(Vector3& point, Vector3& p1, Vector3& p2, Vector3& p3, Vector2& uv1, Vector2& uv2, Vector2& uv3, Vector2& target){
    getBarycoord(point, p1, p2, p3, _v3);

    target.set(0, 0);
    target.addScaledVector(uv1, _v3.x);
    target.addScaledVector(uv2, _v3.y);
    target.addScaledVector(uv3, _v3.z);

    return target;
}

Vector3& Triangle::getBarycoord(Vector3& point, Vector3& a, Vector3& b, Vector3& c, Vector3& target){
    _v0.subVectors(c, a);
    _v1.subVectors(b, a);
    _v2.subVectors(point, a);

    float dot00 = _v0.dot(_v0);
    float dot01 = _v0.dot(_v1);
    float dot02 = _v0.dot(_v2);
    float dot11 = _v1.dot(_v1);
    float dot12 = _v1.dot(_v2);

    float denom = (dot00 * dot11 - dot01 * dot01);



    // collinear or singular triangle
    if (denom == 0) {

        // arbitrary location outside of triangle?
        // not sure if this is the best idea, maybe should be returning undefined
        return target.set(-2, -1, -1);

    }

    float invDenom = 1 / denom;
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // barycentric coordinates must always sum to 1
    return target.set(1 - u - v, v, u);
}

Vector3& Triangle::getNormal(Vector3& a, Vector3& b, Vector3& c, Vector3& target){
    target.subVectors(c, b);
    _v0.subVectors(a, b);
    target.cross(_v0);

    float targetLengthSq = target.lengthSq();
    if (targetLengthSq > 0) {
        return target.multiplyScalar(1 / std::sqrt(targetLengthSq));
    }

    return target.set(0, 0, 0);
}



////const _triangle_v0 = /*@__PURE__*/ new Vector3();
////const _triangle_v1 = /*@__PURE__*/ new Vector3();
////const _triangle_v2 = /*@__PURE__*/ new Vector3();
////const _triangle_v3 = /*@__PURE__*/ new Vector3();
//Vector3 _triangle_v0,_triangle_v1,_triangle_v2,_triangle_v3;
////
////const _vab = /*@__PURE__*/ new Vector3();
////const _vac = /*@__PURE__*/ new Vector3();
////const _vbc = /*@__PURE__*/ new Vector3();
////const _vap = /*@__PURE__*/ new Vector3();
////const _vbp = /*@__PURE__*/ new Vector3();
////const _vcp = /*@__PURE__*/ new Vector3();
//Vector3 _vab,_vac,_vbc,_vap,_vbp,_vcp;
//
//Vector3& Triangle::getNormal( Vector3& a, Vector3& b, Vector3& c,Vector3& target ) {
//    target.subVectors( c, b );
//    _triangle_v0.subVectors(a, b );
//    target.cross(_triangle_v0 );
//
//    const double targetLengthSq = target.lengthSq();
//    if ( targetLengthSq > 0 ) {
//        return target.multiplyScalar( 1 / sqrt( targetLengthSq ) );
//    }
//
//    return target.set( 0, 0, 0 );
//}
//
//
//// static/instance method to calculate barycentric coordinates
//// based on: http://www.blackpawn.com/texts/pointinpoly/default.html
//Vector3& Triangle::getBarycoord( Vector3& point, Vector3& a, Vector3& b, Vector3& c, Vector3& target ){
//    _triangle_v0.subVectors(c, a );
//    _triangle_v1.subVectors(b, a );
//    _triangle_v2.subVectors(point, a );
//
//    const double dot00 = _triangle_v0.dot(_triangle_v0 );
//    const double dot01 = _triangle_v0.dot(_triangle_v1 );
//    const double dot02 = _triangle_v0.dot(_triangle_v2 );
//    const double dot11 = _triangle_v1.dot(_triangle_v1 );
//    const double dot12 = _triangle_v1.dot(_triangle_v2 );
//
//    const double denom = ( dot00 * dot11 - dot01 * dot01 );
//
//    // collinear or singular triangle
//    if ( denom == 0 ) {
//        // arbitrary location outside of triangle?
//        // not sure if this is the best idea, maybe should be returning undefined
//        return target.set( - 2, - 1, - 1 );
//    }
//
//    const double invDenom = 1 / denom;
//    const double u = ( dot11 * dot02 - dot01 * dot12 ) * invDenom;
//    const double v = ( dot00 * dot12 - dot01 * dot02 ) * invDenom;
//
//    // barycentric coordinates must always sum to 1
//    return target.set( 1 - u - v, v, u );
//}
//
//bool Triangle::containsPoint( Vector3& point,Vector3& a,Vector3& b,Vector3& c ){
//    getBarycoord(point, a, b, c, _triangle_v3 );
//
//    return (_triangle_v3.x >= 0 ) && (_triangle_v3.y >= 0 ) && ((_triangle_v3.x + _triangle_v3.y ) <= 1 );
//}
//
//Vector2& Triangle::getUV( Vector3& point, Vector3& p1, Vector3& p2, Vector3& p3, Vector2& uv1, Vector2& uv2, Vector2& uv3, Vector2& target ){
//    getBarycoord(point, p1, p2, p3, _triangle_v3 );
//
//    target.set( 0, 0 );
//    target.addScaledVector(uv1, _triangle_v3.x );
//    target.addScaledVector(uv2, _triangle_v3.y );
//    target.addScaledVector(uv3, _triangle_v3.z );
//
//    return target;
//}
//
//bool Triangle::isFrontFacing( Vector3& a, Vector3& b, Vector3& c, Vector3& direction ){
//    _triangle_v0.subVectors(c, b );
//    _triangle_v1.subVectors(a, b );
//
//    // strictly front facing
//    return (_triangle_v0.cross(_triangle_v1 ).dot(direction ) < 0 ) ? true : false;
//}
//
//double Triangle::getArea() {
//    _triangle_v0.subVectors(c, b );
//    _triangle_v1.subVectors(a, b );
//
//    return _triangle_v0.cross(_triangle_v1 ).length() * 0.5;
//}
//
//Vector3& Triangle::closestPointToPoint( Vector3& p,Vector3& target ){
////    const Vector3& a = this->a, b = this->b, c = this->c;
//    double v, w;
//
//    // algorithm thanks to Real-Time Collision Detection by Christer Ericson,
//    // published by Morgan Kaufmann Publishers, (c) 2005 Elsevier Inc.,
//    // under the accompanying license; see chapter 5.1.5 for detailed explanation.
//    // basically, we're distinguishing which of the voronoi regions of the triangle
//    // the point lies in with the minimum amount of redundant computation.
//    _vab.subVectors( b, a );
//    _vac.subVectors( c, a );
//    _vap.subVectors( p, a );
//    const double d1 = _vab.dot( _vap );
//    const double d2 = _vac.dot( _vap );
//    if ( d1 <= 0 && d2 <= 0 ) {
//        // vertex region of A; barycentric coords (1, 0, 0)
//        return target.copy( a );
//    }
//
//    _vbp.subVectors( p, b );
//    const double d3 = _vab.dot( _vbp );
//    const double d4 = _vac.dot( _vbp );
//    if ( d3 >= 0 && d4 <= d3 ) {
//        // vertex region of B; barycentric coords (0, 1, 0)
//        return target.copy( b );
//    }
//
//    const double vc = d1 * d4 - d3 * d2;
//    if ( vc <= 0 && d1 >= 0 && d3 <= 0 ) {
//        v = d1 / ( d1 - d3 );
//        // edge region of AB; barycentric coords (1-v, v, 0)
//        return target.copy( a ).addScaledVector( _vab, v );
//    }
//
//    _vcp.subVectors( p, c );
//    const double d5 = _vab.dot( _vcp );
//    const double d6 = _vac.dot( _vcp );
//    if ( d6 >= 0 && d5 <= d6 ) {
//        // vertex region of C; barycentric coords (0, 0, 1)
//        return target.copy( c );
//    }
//
//    const double vb = d5 * d2 - d1 * d6;
//    if ( vb <= 0 && d2 >= 0 && d6 <= 0 ) {
//        w = d2 / ( d2 - d6 );
//        // edge region of AC; barycentric coords (1-w, 0, w)
//        return target.copy( a ).addScaledVector( _vac, w );
//    }
//
//    const double va = d3 * d6 - d5 * d4;
//    if ( va <= 0 && ( d4 - d3 ) >= 0 && ( d5 - d6 ) >= 0 ) {
//        _vbc.subVectors( c, b );
//        w = ( d4 - d3 ) / ( ( d4 - d3 ) + ( d5 - d6 ) );
//        // edge region of BC; barycentric coords (0, 1-w, w)
//        return target.copy( b ).addScaledVector( _vbc, w ); // edge region of BC
//    }
//
//    // face region
//    const double denom = 1 / ( va + vb + vc );
//    // u = va * denom
//    v = vb * denom;
//    w = vc * denom;
//
//    return target.copy( a ).addScaledVector( _vab, v ).addScaledVector( _vac, w );
//}
//
////todo:fix this
//bool operator==(const Triangle& lhs,const Triangle& rhs){
//    //return lhs.a.equals(rhs.a) && lhs.b.equals(rhs.b) && lhs.c.equals(rhs.c);
//    return lhs.a==rhs.a && lhs.b==rhs.b && lhs.c==rhs.c;
//}
//
//bool operator!=(const Triangle& lhs,const Triangle& rhs){
//    return !(lhs==rhs);
//}



