//
// Created by Willie on 2022/10/4.
//

#include "line3.h"

//const _startP = /*@__PURE__*/ new Vector3();
//const _startEnd = /*@__PURE__*/ new Vector3();
Vector3 _startP;
Vector3 _startEnd;

double Line3::closestPointToPointParameter( Vector3& point, bool clampToLine ) {
    _startP.subVectors( point, start );
    _startEnd.subVectors( end, start );

    const double startEnd2 = _startEnd.dot( _startEnd );
    const double startEnd_startP = _startEnd.dot( _startP );

    double t = startEnd_startP / startEnd2;

    if ( clampToLine ) {
        t = math::clamp<double>( t, 0, 1 );
    }

    return t;
}

Vector3& Line3::closestPointToPoint( Vector3& point, bool clampToLine, Vector3& target ) {
    const double t = closestPointToPointParameter( point, clampToLine );

    return delta( target ).multiplyScalar( t ).add( start );
}