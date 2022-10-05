//
// Created by Willie on 2022/10/4.
//

#ifndef THREE_CPP_LINE3_H
#define THREE_CPP_LINE3_H

#include "vector3.h"
#include "math_utils.h"
#include "matrix4.h"

class Line3 {
public:
    Vector3 start,end;

    Line3(Vector3 start = Vector3(),Vector3 end = Vector3()):start(start),end(end){}

    //clone() = copy constructor
    Line3(const Line3& l):start(l.start),end(l.end){};

    Line3& set(Vector3& s,Vector3& e){
        start.copy(s);
        end.copy(e);

        return *this;
    }

    Line3& copy( const Line3& line ) {
        start.copy( line.start );
        end.copy( line.end );

        return *this;
    }

    Vector3& getCenter( Vector3& target ) {
        return target.addVectors( start, end ).multiplyScalar( 0.5 );
    }

    Vector3& delta( Vector3& target ) {
        return target.subVectors( end, start );
    }

    double distanceSq() {
        return start.distanceToSquared( end );
    }

    double distance() {
        return start.distanceTo( end );
    }

    Vector3& at( double t, Vector3& target ) {
        return delta( target ).multiplyScalar( t ).add( start );
    }

    bool equals( const Line3& line ) {
        return line.start.equals( start ) && line.end.equals( end );
    }

    Vector3& closestPointToPoint( Vector3& point, bool clampToLine, Vector3& target );

    double closestPointToPointParameter( Vector3& point, bool clampToLine );

    Line3& applyMatrix4( Matrix4& matrix ) {
        start.applyMatrix4( matrix );
        end.applyMatrix4( matrix );

        return *this;
    }

};


#endif //THREE_CPP_LINE3_H
