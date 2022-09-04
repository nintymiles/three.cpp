#ifndef SPHERE_H
#define SPHERE_H

#include "box3.h"
#include "vector3.h"

#include <memory>

using std::shared_ptr;

// const _box = /*@__PURE__*/ new Box3();
// const _v1 = /*@__PURE__*/ new Vector3();
// const _toFarthestPoint = /*@__PURE__*/ new Vector3();
// const _toPoint = /*@__PURE__*/ new Vector3();
static Box3 _box;
static Vector3 _v1;
static Vector3 _toFarthestPoint;
static Vector3 _toPoint;

class Sphere {
	public:
		Vector3 center;
		double radius;

		Sphere(Vector3 centerV = Vector3(),double r = -1):center(centerV),radius(r){}

		//copy constructor
		Sphere(const Sphere& sphere):center(sphere.center),radius(sphere.radius){}
		// copy( sphere ) {
		// 	this.center.copy( sphere.center );
		// 	this.radius = sphere.radius;
		// 	return this;
		// }

		Sphere& set(Vector3& center,double radius) {
			center.copy( center );
			radius = radius;

			return *this;
		}

		Sphere& setFromPoints(vector<Vector3>& points, shared_ptr<Vector3> optionalCenter) {
			if(optionalCenter != nullptr){
				center.copy( *optionalCenter );
			}else {
				_box.setFromPoints( points ).getCenter( center );
			}

			double maxRadiusSq = 0;

			for ( int i = 0, il = points.size(); i < il; i ++ ) {
				maxRadiusSq = fmax( maxRadiusSq, center.distanceToSquared( points[ i ] ) );
			}

			radius = sqrt( maxRadiusSq );

			return *this;
		}


		bool isEmpty() {
			return ( radius < 0 );
		}

		Sphere& makeEmpty() {
			center.set( 0, 0, 0 );
			radius = - 1;

			return *this;
		}

		bool containsPoint( Vector3& point ) {
			return ( point.distanceToSquared( center ) <= ( radius * radius ) );
		}

		double distanceToPoint( Vector3& point ) {
			return ( point.distanceTo( center ) - radius );
		}

		bool intersectsSphere( Sphere& sphere ) {
			const double radiusSum = radius + sphere.radius;
			return sphere.center.distanceToSquared( center ) <= ( radiusSum * radiusSum );
		}

		// intersectsBox( box ) {

		// 	return box.intersectsSphere( this );

		// }

		// intersectsPlane( plane ) {

		// 	return Math.abs( plane.distanceToPoint( this.center ) ) <= this.radius;

		// }

		Vector3& clampPoint( Vector3& point, Vector3& target ) {
			const double deltaLengthSq = center.distanceToSquared( point );
			target.copy( point );

			if ( deltaLengthSq > ( radius * radius ) ) {
				target.sub( center ).normalize();
				target.multiplyScalar( radius ).add( center );
			}

			return target;
		}

		Box3& getBoundingBox( Box3& target ) {
			if ( isEmpty() ) {
				// Empty sphere produces empty bounding box
				target.makeEmpty();
				return target;
			}

			target.set( center, center );
			target.expandByScalar( radius );

			return target;
		}

		Sphere& applyMatrix4(Matrix4& matrix){
			center.applyMatrix4( matrix );
			radius = radius * matrix.getMaxScaleOnAxis();

			return *this;
		}

		Sphere& translate( Vector3& offset ) {
			center.add( offset );
			return *this;
		}

		Sphere& expandByPoint( Vector3& point ) {
			// from https://github.com/juj/MathGeoLib/blob/2940b99b99cfe575dd45103ef20f4019dee15b54/src/Geometry/Sphere.cpp#L649-L671
			_toPoint.subVectors( point, center );

			const double lengthSq = _toPoint.lengthSq();
			if ( lengthSq > ( radius * radius ) ) {

				const double length = sqrt( lengthSq );
				const double missingRadiusHalf = ( length - radius ) * 0.5;

				// Nudge this sphere towards the target point. Add half the missing distance to radius,
				// and the other half to position. This gives a tighter enclosure, instead of if
				// the whole missing distance were just added to radius.
				center.add( _toPoint.multiplyScalar( missingRadiusHalf / length ) );
				radius += missingRadiusHalf;

			}

			return *this;
		}

		Sphere& unionSphere( Sphere& sphere ) {
			// from https://github.com/juj/MathGeoLib/blob/2940b99b99cfe575dd45103ef20f4019dee15b54/src/Geometry/Sphere.cpp#L759-L769

			// To enclose another sphere into this sphere, we only need to enclose two points:
			// 1) Enclose the farthest point on the other sphere into this sphere.
			// 2) Enclose the opposite point of the farthest point into this sphere.
			if ( center.equals( sphere.center ) == true ) {
				_toFarthestPoint.set( 0, 0, 1 ).multiplyScalar( sphere.radius );
			} else {
				_toFarthestPoint.subVectors( sphere.center, center ).normalize().multiplyScalar( sphere.radius );
			}

			expandByPoint( _v1.copy( sphere.center ).add( _toFarthestPoint ) );
			expandByPoint( _v1.copy( sphere.center ).sub( _toFarthestPoint ) );

			return *this;
		}

		bool equals( Sphere& sphere ) {
			return sphere.center.equals( center ) && ( sphere.radius == radius );
		}

		Sphere clone() {
			return *this;
		}

};

#endif /* SPHERE_H */
