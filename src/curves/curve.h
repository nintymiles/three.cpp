//
// Created by SeanR on 2023/11/6.
//

#ifndef THREE_CPP_CURVE_H
#define THREE_CPP_CURVE_H

#include "vector2.h"
#include "vector3.h"
#include "matrix4.h"
#include "math_utils.h"

/**
 * Extensible curve object.
 *
 * Some common of curve methods:
 * .getPoint( t, optionalTarget ), .getTangent( t, optionalTarget )
 * .getPointAt( u, optionalTarget ), .getTangentAt( u, optionalTarget )
 * .getPoints(), .getSpacedPoints()
 * .getLength()
 * .updateArcLengths()
 *
 * This following curves inherit from THREE.Curve:
 *
 * -- 2D curves --
 * THREE.ArcCurve
 * THREE.CubicBezierCurve
 * THREE.EllipseCurve
 * THREE.LineCurve
 * THREE.QuadraticBezierCurve
 * THREE.SplineCurve
 *
 * -- 3D curves --
 * THREE.CatmullRomCurve3
 * THREE.CubicBezierCurve3
 * THREE.LineCurve3
 * THREE.QuadraticBezierCurve3
 *
 * A series of curves can be represented as a THREE.CurvePath.
 *
 **/
class Curve {
    std::string type = "Curve";
    int arcLengthDivisions = 200;
    std::vector<float> cacheArcLengths;
    bool needsUpdate = false;

public:
    struct FrenetFrames{
        std::vector<Vector3> tangents;
        std::vector<Vector3> normals;
        std::vector<Vector3> binormals;
    };

    Curve(){}

    // Virtual base class method to overwrite and implement in subclasses
    //	- t [0 .. 1]
    virtual Vector3 getPoint( float t,Vector3 *optionalTarget = nullptr ) {
        std::cout << 'THREE.Curve: .getPoint() not implemented.' << std::endl;
    }

    // Get point at relative position in curve according to arc length
    // - u [0 .. 1]
    Vector3 getPointAt( float u, Vector3 optionalTarget = Vector3{0,0,0} ) {
        const float t = getUtoTmapping(u);
        return getPoint( t, &optionalTarget );
    }

    // Given u ( 0 .. 1 ), get a t to find p. This gives you points which are equidistant
    float getUtoTmapping( float u, float distance = 0 ) {
        std::vector<float> arcLengths = getLengths();

        int i = 0;
        const int il = arcLengths.size();

        float targetArcLength; // The targeted u distance value to get

        if ( distance ) {
            targetArcLength = distance;
        } else {
            targetArcLength = u * arcLengths[ il - 1 ];
        }

        // binary search for the index with largest value smaller than target u distance
        int low = 0, high = il - 1, comparison;
        while ( low <= high ) {
            i = std::floor( low + ( high - low ) / 2 ); // less likely to overflow, though probably not issue here, JS doesn't really have integers, all numbers are floats
            comparison = arcLengths[ i ] - targetArcLength;

            if ( comparison < 0 ) {
                low = i + 1;
            } else if ( comparison > 0 ) {
                high = i - 1;
            } else {
                high = i;
                break;
                // DONE
            }
        }

        i = high;
        if ( arcLengths[ i ] == targetArcLength ) {
            return i / ( il - 1 );
        }

        // we could get finer grain at lengths, or use simple interpolation between two points
        const float lengthBefore = arcLengths[ i ];
        const float lengthAfter = arcLengths[ i + 1 ];

        const float segmentLength = lengthAfter - lengthBefore;

        // determine where we are between the 'before' and 'after' points
        const float segmentFraction = ( targetArcLength - lengthBefore ) / segmentLength;

        // add that fractional amount to t
        const float t = ( i + segmentFraction ) / ( il - 1 );

        return t;
    }

    std::vector<float> getLengths( int divisions = 200 ) {
        if ( cacheArcLengths.size() == divisions + 1  &&
             !needsUpdate ) {
            return cacheArcLengths;
        }

        needsUpdate = false;

        std::vector<float> cache{};
        Vector3 current, last = getPoint( 0 );
        int sum = 0;

        cache.push_back( 0 );

        for ( int p = 1; p <= divisions; p ++ ) {
            current = getPoint( p / divisions );
            sum += current.distanceTo( last );
            cache.push_back( sum );
            last = current;
        }

        cacheArcLengths = cache;

        return cache; // { sums: cache, sum: sum }; Sum is in the last element.
    }

    // Get sequence of points using getPoint( t )
    std::vector<Vector3> getPoints( int divisions = 5 ) {
        std::vector<Vector3> points{};

        for ( int d = 0; d <= divisions; d ++ ) {
            points.push_back( this->getPoint( d / divisions ) );
        }

        return points;
    }


    // Get sequence of points using getPointAt( u )
    std::vector<Vector3> getSpacedPoints( int divisions = 5 ) {
        std::vector<Vector3> points{};

        for ( int d = 0; d <= divisions; d ++ ) {

            points.push_back( this->getPointAt( d / divisions ) );
        }

        return points;
    }

    // Get total curve arc length
    float getLength() {
        std::vector<float> lengths = this->getLengths();
        return lengths[ lengths.size() - 1 ];
    }

    std::vector<float> updateArcLengths() {
        this->needsUpdate = true;
        return this->getLengths();
    }

    // Returns a unit vector tangent at t
    // In case any sub curve does not implement its tangent derivation,
    // 2 points a small delta apart will be used to find its gradient
    // which seems to give a reasonable approximation
    Vector3 getTangent( float t, Vector3 optionalTarget ) {
        const float delta = 0.0001;
        float t1 = t - delta;
        float t2 = t + delta;

        // Capping in case of danger
        if ( t1 < 0 ) t1 = 0;
        if ( t2 > 1 ) t2 = 1;

        Vector3 pt1 = this->getPoint( t1 );
        Vector3 pt2 = this->getPoint( t2 );

        //todo:fix this
        Vector3 tangent = optionalTarget; //|| Vector3() );

        tangent.copy( pt2 ).sub( pt1 ).normalize();

        return tangent;
    }

    Vector3 getTangentAt( float u, Vector3 optionalTarget ) {
        const float t = this->getUtoTmapping( u );
        return this->getTangent( t, optionalTarget );
    }

    FrenetFrames computeFrenetFrames( int segments, bool  closed ) {
        // see http://www.cs.indiana.edu/pub/techreports/TR425.pdf
        const normal = new Vector3();

        const tangents = [];
        const normals = [];
        const binormals = [];

        const vec = new Vector3();
        const mat = new Matrix4();

        // compute the tangent vectors for each segment on the curve

        for ( let i = 0; i <= segments; i ++ ) {

            const u = i / segments;

            tangents[ i ] = this.getTangentAt( u, new Vector3() );

        }

        // select an initial normal vector perpendicular to the first tangent vector,
        // and in the direction of the minimum tangent xyz component

        normals[ 0 ] = new Vector3();
        binormals[ 0 ] = new Vector3();
        let min = Number.MAX_VALUE;
        const tx = Math.abs( tangents[ 0 ].x );
        const ty = Math.abs( tangents[ 0 ].y );
        const tz = Math.abs( tangents[ 0 ].z );

        if ( tx <= min ) {

            min = tx;
            normal.set( 1, 0, 0 );

        }

        if ( ty <= min ) {

            min = ty;
            normal.set( 0, 1, 0 );

        }

        if ( tz <= min ) {

            normal.set( 0, 0, 1 );

        }

        vec.crossVectors( tangents[ 0 ], normal ).normalize();

        normals[ 0 ].crossVectors( tangents[ 0 ], vec );
        binormals[ 0 ].crossVectors( tangents[ 0 ], normals[ 0 ] );


        // compute the slowly-varying normal and binormal vectors for each segment on the curve

        for ( let i = 1; i <= segments; i ++ ) {

            normals[ i ] = normals[ i - 1 ].clone();

            binormals[ i ] = binormals[ i - 1 ].clone();

            vec.crossVectors( tangents[ i - 1 ], tangents[ i ] );

            if ( vec.length() > Number.EPSILON ) {

                vec.normalize();

                const theta = Math.acos( MathUtils.clamp( tangents[ i - 1 ].dot( tangents[ i ] ), - 1, 1 ) ); // clamp for floating pt errors

                normals[ i ].applyMatrix4( mat.makeRotationAxis( vec, theta ) );

            }

            binormals[ i ].crossVectors( tangents[ i ], normals[ i ] );

        }

        // if the curve is closed, postprocess the vectors so the first and last normal vectors are the same

        if ( closed === true ) {

            let theta = Math.acos( MathUtils.clamp( normals[ 0 ].dot( normals[ segments ] ), - 1, 1 ) );
            theta /= segments;

            if ( tangents[ 0 ].dot( vec.crossVectors( normals[ 0 ], normals[ segments ] ) ) > 0 ) {

                theta = - theta;

            }

            for ( let i = 1; i <= segments; i ++ ) {

                // twist a little...
                normals[ i ].applyMatrix4( mat.makeRotationAxis( tangents[ i ], theta * i ) );
                binormals[ i ].crossVectors( tangents[ i ], normals[ i ] );

            }

        }

        return {
                tangents: tangents,
                normals: normals,
                binormals: binormals
        };

    }


    Curve& copy( const Curve& source ) {
        this->arcLengthDivisions = source.arcLengthDivisions;

        return *this;
    }

    Curve clone() {
        return Curve().copy( *this );
    }





};



//class Curve {







//
//    toJSON() {
//
//        const data = {
//                metadata: {
//                        version: 4.5,
//                        type: 'Curve',
//                        generator: 'Curve.toJSON'
//                }
//        };
//
//        data.arcLengthDivisions = this.arcLengthDivisions;
//        data.type = this.type;
//
//        return data;
//
//    }
//
//    fromJSON( json ) {
//
//            this.arcLengthDivisions = json.arcLengthDivisions;
//
//            return this;
//
//    }
//
//}
//
//
//        export { Curve };


#endif //THREE_CPP_CURVE_H
