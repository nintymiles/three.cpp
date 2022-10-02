//
// Created by SeanRen on 2022/10/2.
//

#ifndef THREE_CPP_TRIANGLE_H
#define THREE_CPP_TRIANGLE_H

#include <gsl/gsl>

#include "vector3.h"
#include "vector2.h"
#include "plane.h"

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

    Triangle& setFromPointsAndIndices( gsl::span<Vector3> points, int i0, int i1, int i2 ) {
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

//    intersectsBox( box ) {
//
//            return box.intersectsTriangle( this );
//
//    }
//
//    closestPointToPoint( p, target ) {
//
//        const a = this.a, b = this.b, c = this.c;
//        let v, w;
//
//        // algorithm thanks to Real-Time Collision Detection by Christer Ericson,
//        // published by Morgan Kaufmann Publishers, (c) 2005 Elsevier Inc.,
//        // under the accompanying license; see chapter 5.1.5 for detailed explanation.
//        // basically, we're distinguishing which of the voronoi regions of the triangle
//        // the point lies in with the minimum amount of redundant computation.
//
//        _vab.subVectors( b, a );
//        _vac.subVectors( c, a );
//        _vap.subVectors( p, a );
//        const d1 = _vab.dot( _vap );
//        const d2 = _vac.dot( _vap );
//        if ( d1 <= 0 && d2 <= 0 ) {
//
//            // vertex region of A; barycentric coords (1, 0, 0)
//            return target.copy( a );
//
//        }
//
//        _vbp.subVectors( p, b );
//        const d3 = _vab.dot( _vbp );
//        const d4 = _vac.dot( _vbp );
//        if ( d3 >= 0 && d4 <= d3 ) {
//
//            // vertex region of B; barycentric coords (0, 1, 0)
//            return target.copy( b );
//
//        }
//
//        const vc = d1 * d4 - d3 * d2;
//        if ( vc <= 0 && d1 >= 0 && d3 <= 0 ) {
//
//            v = d1 / ( d1 - d3 );
//            // edge region of AB; barycentric coords (1-v, v, 0)
//            return target.copy( a ).addScaledVector( _vab, v );
//
//        }
//
//        _vcp.subVectors( p, c );
//        const d5 = _vab.dot( _vcp );
//        const d6 = _vac.dot( _vcp );
//        if ( d6 >= 0 && d5 <= d6 ) {
//
//            // vertex region of C; barycentric coords (0, 0, 1)
//            return target.copy( c );
//
//        }
//
//        const vb = d5 * d2 - d1 * d6;
//        if ( vb <= 0 && d2 >= 0 && d6 <= 0 ) {
//
//            w = d2 / ( d2 - d6 );
//            // edge region of AC; barycentric coords (1-w, 0, w)
//            return target.copy( a ).addScaledVector( _vac, w );
//
//        }
//
//        const va = d3 * d6 - d5 * d4;
//        if ( va <= 0 && ( d4 - d3 ) >= 0 && ( d5 - d6 ) >= 0 ) {
//
//            _vbc.subVectors( c, b );
//            w = ( d4 - d3 ) / ( ( d4 - d3 ) + ( d5 - d6 ) );
//            // edge region of BC; barycentric coords (0, 1-w, w)
//            return target.copy( b ).addScaledVector( _vbc, w ); // edge region of BC
//
//        }
//
//        // face region
//        const denom = 1 / ( va + vb + vc );
//        // u = va * denom
//        v = vb * denom;
//        w = vc * denom;
//
//        return target.copy( a ).addScaledVector( _vab, v ).addScaledVector( _vac, w );
//
//    }
//
//    equals( triangle ) {
//
//            return triangle.a.equals( this.a ) && triangle.b.equals( this.b ) && triangle.c.equals( this.c );
//
//    }

};


#endif //THREE_CPP_TRIANGLE_H
