//
// Created by SeanR on 2023/11/6.
//

#ifndef THREE_CPP_CATMULL_ROM_CUVE_H
#define THREE_CPP_CATMULL_ROM_CUVE_H

#include "curve.h"

#include "vector3.h"

#include <string>
#include <vector>

/*
Based on an optimized c++ solution in
 - http://stackoverflow.com/questions/9489736/catmull-rom-curve-with-no-cusps-and-no-self-intersections/
 - http://ideone.com/NoEbVM

This CubicPoly class could be used for reusing some variables and calculations,
but for three.js curve use, it could be possible inlined and flatten into a single function call
which can be placed in CurveUtils.
*/

class CubicPoly{
    float c0=0,c1=0,c2=0,c3=0;

    /*
     * Compute coefficients for a cubic polynomial
     *   p(s) = c0 + c1*s + c2*s^2 + c3*s^3
     * such that
     *   p(0) = x0, p(1) = x1
     *  and
     *   p'(0) = t0, p'(1) = t1.
     */
    void init( float x0, float x1, float t0, float t1 ) {
        c0 = x0;
        c1 = t0;
        c2 = - 3 * x0 + 3 * x1 - 2 * t0 - t1;
        c3 = 2 * x0 - 2 * x1 + t0 + t1;
    }

public:
    CubicPoly& initCatmullRom( float x0, float x1, float x2, float x3, float tension ) {
        init( x1, x2, tension * ( x2 - x0 ), tension * ( x3 - x1 ) );
        return *this;
    }


    CubicPoly& initNonuniformCatmullRom( float x0, float x1, float x2, float x3, float dt0, float dt1, float dt2 ) {
        // compute tangents when parameterized in [t1,t2]
        float t1 = ( x1 - x0 ) / dt0 - ( x2 - x0 ) / ( dt0 + dt1 ) + ( x2 - x1 ) / dt1;
        float t2 = ( x2 - x1 ) / dt1 - ( x3 - x1 ) / ( dt1 + dt2 ) + ( x3 - x2 ) / dt2;

        // rescale tangents for parametrization in [0,1]
        t1 *= dt1;
        t2 *= dt1;

        init( x1, x2, t1, t2 );

        return *this;
    }


    float calc( float t ) {
        const float t2 = t * t;
        const float t3 = t2 * t;
        return c0 + c1 * t + c2 * t2 + c3 * t3;
    }

};


/**
 * Centripetal CatmullRom Curve - which is useful for avoiding
 * cusps and self-intersections in non-uniform catmull rom curves.
 * http://www.cemyuksel.com/research/catmullrom_param/catmullrom.pdf
 *
 * curve.type accepts centripetal(default), chordal and catmullrom
 * curve.tension is used for catmullrom which defaults to 0.5
 */


class CatmullRomCurve3: public Curve{
    bool isCatmullRomCurve3;
    std::string type = "CatmullRomCurve3";
    std::vector<Vector3> points;
    bool closed;
    std::string curveType;
    float tension;
public:
    CatmullRomCurve3( std::vector<Vector3> &points, bool closed = false, std::string curveType = "centripetal", float tension = 0.5 ):points(points),closed(closed),curveType(curveType),tension(tension) {}

    virtual Vector3 getPoint( float t, Vector3 *optionalTarget = nullptr ) override;

    CatmullRomCurve3& copy( const CatmullRomCurve3 &source ) {
        Curve::copy( source );

        this->points.clear();

        for ( int i = 0, l = source.points.size(); i < l; i ++ ) {
            Vector3 point = source.points[ i ];

            this->points.push_back( point.clone() );
        }

        this->closed = source.closed;
        this->curveType = source.curveType;
        this->tension = source.tension;

        return *this;
    }

    ~CatmullRomCurve3() = default;
};






//class CatmullRomCurve3 extends Curve {

//
//toJSON() {
//
//    const data = super.toJSON();
//
//    data.points = [];
//
//    for ( let i = 0, l = this.points.length; i < l; i ++ ) {
//
//        const point = this.points[ i ];
//        data.points.push( point.toArray() );
//
//    }
//
//    data.closed = this.closed;
//    data.curveType = this.curveType;
//    data.tension = this.tension;
//
//    return data;
//
//}
//
//fromJSON( json ) {
//
//        super.fromJSON( json );
//
//        this.points = [];
//
//        for ( let i = 0, l = json.points.length; i < l; i ++ ) {
//
//            const point = json.points[ i ];
//            this.points.push( new Vector3().fromArray( point ) );
//
//        }
//
//        this.closed = json.closed;
//        this.curveType = json.curveType;
//        this.tension = json.tension;
//
//        return this;
//
//}
//
//}
//
//export { CatmullRomCurve3 };


#endif //THREE_CPP_CATMULL_ROM_CUVE_H
