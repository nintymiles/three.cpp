#ifndef SPHERE_H
#define SPHERE_H

#include "box3.h"
#include "vector3.h"

#include <memory>

using std::shared_ptr;

class Matrix4;

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

		Sphere& setFromPoints(vector<Vector3>& points, shared_ptr<Vector3> optionalCenter);


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

		Sphere& applyMatrix4(Matrix4& matrix);

		Sphere& translate( Vector3& offset ) {
			center.add( offset );
			return *this;
		}

		Sphere& expandByPoint( Vector3& point );

		Sphere& unionSphere( Sphere& sphere );

		bool equals( Sphere& sphere ) {
			return sphere.center.equals( center ) && ( sphere.radius == radius );
		}

		Sphere clone() {
			return *this;
		}

};

#endif /* SPHERE_H */
