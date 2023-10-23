//
// Created by SeanRen on 2022/10/2.
//

#ifndef THREE_CPP_TRIANGLE_H
#define THREE_CPP_TRIANGLE_H

//#include <gsl/gsl>

#include "vector3.h"
#include "vector2.h"
#include "plane.h"
#include "box3.h"

class Triangle;
bool operator==(const Triangle& lhs,const Triangle& rhs);
bool operator!=(const Triangle& lhs,const Triangle& rhs);

class Triangle {
public:
    Vector3 a,b,c;

    Triangle(Vector3 a = Vector3(),Vector3 b = Vector3(),Vector3 c = Vector3()):a(a),b(b),c(c){}

    Triangle(const Triangle& t):a(t.a),b(t.b),c(t.c){}

    static Vector3& getNormal( Vector3& a, Vector3& b, Vector3& c,Vector3& target );

    // static/instance method to calculate barycentric coordinates
    // based on: http://www.blackpawn.com/texts/pointinpoly/default.html
    static Vector3& getBarycoord( Vector3& point, Vector3& a, Vector3& b, Vector3& c, Vector3& target );

    static bool containsPoint( Vector3& point,Vector3& a,Vector3& b,Vector3& c );

    //get a UV coord of a point in triangle from triangle's points and UVs
    static Vector2& getUV( Vector3& point, Vector3& p1, Vector3& p2, Vector3& p3, Vector2& uv1, Vector2& uv2, Vector2& uv3, Vector2& target );

    static bool isFrontFacing( Vector3& a, Vector3& b, Vector3& c, Vector3& direction );

    Triangle& set( Vector3& a, Vector3& b, Vector3& c ) {
        this->a.copy( a );
        this->b.copy( b );
        this->c.copy( c );

        return *this;
    }

    Triangle& setFromPointsAndIndices( std::vector<Vector3> points, int i0, int i1, int i2 ) {
        this->a.copy( points[ i0 ] );
        this->b.copy( points[ i1 ] );
        this->c.copy( points[ i2 ] );

        return *this;
    }

//    Triangle& setFromAttributeAndIndices( attribute, i0, i1, i2 ) {
//
//        this.a.fromBufferAttribute( attribute, i0 );
//        this.b.fromBufferAttribute( attribute, i1 );
//        this.c.fromBufferAttribute( attribute, i2 );
//
//        return *this;
//    }

//    clone() {
//
//        return new this.constructor().copy( this );
//
//    }

    Triangle& copy( Triangle& triangle ) {
       *this = triangle;

       return *this;
    }

    double getArea();

    Vector3& getMidpoint( Vector3& target ) {
        return target.addVectors( a, b ).add( c ).multiplyScalar( 1 / 3 );
    }

    Vector3& getNormal( Vector3& target ) {
        return Triangle::getNormal( a, b, c, target );
    }

    Plane& getPlane( Plane& target ) {
        return target.setFromCoplanarPoints( a, b, c );
    }

    Vector3& getBarycoord( Vector3& point, Vector3& target ) {
        return Triangle::getBarycoord( point, a, b, c, target );
    }

    Vector2& getUV(Vector3& point,Vector2& uv1,Vector2& uv2,Vector2& uv3,Vector2& target ) {
        return Triangle::getUV( point, a, b, c, uv1, uv2, uv3, target );
    }

    bool containsPoint( Vector3& point ) {
        return Triangle::containsPoint( point, a, b, c );
    }

    bool isFrontFacing( Vector3& direction ) {
        return Triangle::isFrontFacing( a, b, c, direction );
    }

    bool intersectsBox( Box3& box ) {
        return box.intersectsTriangle( *this );
    }

    Vector3& closestPointToPoint( Vector3& p,Vector3& target );

    bool equals( Triangle& triangle ) {
        return *this == triangle;
    }

};


#endif //THREE_CPP_TRIANGLE_H
