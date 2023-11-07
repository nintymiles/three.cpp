//
// Created by SeanR on 2023/11/6.
//

#include "catmull_rom_cuve3.h"


namespace catmull_rom_curve3{
    Vector3 tmp;
    CubicPoly px;
    CubicPoly py;
    CubicPoly pz;
}

using namespace catmull_rom_curve3;

Vector3 CatmullRomCurve3::getPoint( float t, Vector3 *optionalTarget){
    Vector3 point = optionalTarget == nullptr ? Vector3(): *optionalTarget;

    std::vector<Vector3>& points = this->points;
    const int l = points.size();

    const float p = ( l - ( this->closed ? 0 : 1 ) ) * t;
    int intPoint = std::floor( p );
    float weight = p - intPoint;

    if ( this->closed ) {
        intPoint += intPoint > 0 ? 0 : ( std::floor( std::abs( intPoint ) / l ) + 1 ) * l;
    } else if ( weight == 0 && intPoint == l - 1 ) {
        intPoint = l - 2;
        weight = 1;
    }

    Vector3 p0, p3; // 4 points (p1 & p2 defined below)

    if ( this->closed || intPoint > 0 ) {
        p0 = points[ ( intPoint - 1 ) % l ];
    } else {
        // extrapolate first point
        tmp.subVectors( points[ 0 ], points[ 1 ] ).add( points[ 0 ] );
        p0 = tmp;
    }

    Vector3 p1 = points[ intPoint % l ];
    Vector3 p2 = points[ ( intPoint + 1 ) % l ];

    if ( this->closed || intPoint + 2 < l ) {
        p3 = points[ ( intPoint + 2 ) % l ];
    } else {
        // extrapolate last point
        tmp.subVectors( points[ l - 1 ], points[ l - 2 ] ).add( points[ l - 1 ] );
        p3 = tmp;
    }

    if ( this->curveType == "centripetal" || this->curveType == "chordal" ) {
        // init Centripetal / Chordal Catmull-Rom
        const float pow = this->curveType == "chordal" ? 0.5 : 0.25;
        float dt0 = math::pow( p0.distanceToSquared( p1 ), pow );
        float dt1 = math::pow( p1.distanceToSquared( p2 ), pow );
        float dt2 = math::pow( p2.distanceToSquared( p3 ), pow );

        // safety check for repeated points
        if ( dt1 < 1e-4 ) dt1 = 1.0;
        if ( dt0 < 1e-4 ) dt0 = dt1;
        if ( dt2 < 1e-4 ) dt2 = dt1;

        px.initNonuniformCatmullRom( p0.x, p1.x, p2.x, p3.x, dt0, dt1, dt2 );
        py.initNonuniformCatmullRom( p0.y, p1.y, p2.y, p3.y, dt0, dt1, dt2 );
        pz.initNonuniformCatmullRom( p0.z, p1.z, p2.z, p3.z, dt0, dt1, dt2 );

    } else if ( this->curveType == "catmullrom" ) {
        px.initCatmullRom( p0.x, p1.x, p2.x, p3.x, this->tension );
        py.initCatmullRom( p0.y, p1.y, p2.y, p3.y, this->tension );
        pz.initCatmullRom( p0.z, p1.z, p2.z, p3.z, this->tension );

    }

    point.set(
            px.calc( weight ),
            py.calc( weight ),
            pz.calc( weight )
    );

    return point;
}