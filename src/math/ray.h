#ifndef SRC_MATH_RAY_H_
#define SRC_MATH_RAY_H_

#include "vector3.h"
#include <memory>

class Ray {
	private:
		// const _buffer_attribute_vector = /*@__PURE__*/ new Vector3();
		// const _segCenter = /*@__PURE__*/ new Vector3();
		// const _segDir = /*@__PURE__*/ new Vector3();
		// const _diff = /*@__PURE__*/ new Vector3();
		static Vector3 _vector;
		static Vector3 _segCenter;
		static Vector3 _segDir;
		static Vector3 _diff;

		// const _edge1 = /*@__PURE__*/ new Vector3();
		// const _edge2 = /*@__PURE__*/ new Vector3();
		// const _normal = /*@__PURE__*/ new Vector3();
		static Vector3 _edge1;
		static Vector3 _edge2;
		static Vector3 _normal; 

    public:
		Vector3 origin;
        Vector3 direction;

        Ray(Vector3 origin = Vector3(),Vector3 direction = Vector3(0,0,-1)):origin(origin),direction(direction){}
        Ray& set(Vector3& origin,Vector3& direction){
            this->origin.copy(origin);
            this->direction.copy(direction);

            return *this;
        }

		//copy constructor
		Ray(const Ray& ray):origin(ray.origin),direction(ray.direction){}

        // Ray& copy(Vector3& ray){
        //     this->origin.copy(ray.origin);
        //     this->direction.copy(ray.direction)

        //     return *this;
        // }

		Vector3& at(double t, Vector3& target){
			return target.copy( this->direction ).multiplyScalar( t ).add( this->origin );
		}

		Ray& lookAt(Vector3& v) {
			this->direction.copy(v).sub( this->origin ).normalize();

			return *this;
		}

		Ray& recast(double t) {
			origin.copy(at( t, _vector ));

			return *this;
		}

		Vector3& closestPointToPoint( Vector3& point, Vector3& target ) {
			target.subVectors( point, origin );
			const double directionDistance = target.dot( direction );

			if ( directionDistance < 0 ) {
				return target.copy( origin );
			}

			return target.copy( direction ).multiplyScalar( directionDistance ).add( origin );
		}

		double distanceToPoint(Vector3& point) {
			return sqrt( distanceSqToPoint( point ) );
		}

		double distanceSqToPoint(Vector3& point) {
			const double directionDistance = _vector.subVectors( point, origin ).dot( direction );

			// point behind the ray
			if ( directionDistance < 0 ) {
				return origin.distanceToSquared( point );
			}

			_vector.copy( direction ).multiplyScalar( directionDistance ).add( origin );

			return _vector.distanceToSquared( point );
		}

		double distanceSqToSegment(Vector3& v0,Vector3& v1,shared_ptr<Vector3> optionalPointOnRay,shared_ptr<Vector3> optionalPointOnSegment) {
			// from https://github.com/pmjoniak/GeometricTools/blob/master/GTEngine/Include/Mathematics/GteDistRaySegment.h
			// It returns the min distance between the ray and the segment
			// defined by v0 and v1
			// It can also set two optional targets :
			// - The closest point on the ray
			// - The closest point on the segment

			_segCenter.copy( v0 ).add( v1 ).multiplyScalar( 0.5 );
			_segDir.copy( v1 ).sub( v0 ).normalize();
			_diff.copy( origin ).sub( _segCenter );

			const double segExtent = v0.distanceTo( v1 ) * 0.5;
			const double a01 = - direction.dot( _segDir );
			const double b0 = _diff.dot( direction );
			const double b1 = - _diff.dot( _segDir );
			const double c = _diff.lengthSq();
			const double det = abs( 1 - a01 * a01 );
			double s0, s1, sqrDist, extDet;

			if ( det > 0 ) {

				// The ray and segment are not parallel.

				s0 = a01 * b1 - b0;
				s1 = a01 * b0 - b1;
				extDet = segExtent * det;

				if ( s0 >= 0 ) {

					if ( s1 >= - extDet ) {

						if ( s1 <= extDet ) {

							// region 0
							// Minimum at interior points of ray and segment.

							const double invDet = 1 / det;
							s0 *= invDet;
							s1 *= invDet;
							sqrDist = s0 * ( s0 + a01 * s1 + 2 * b0 ) + s1 * ( a01 * s0 + s1 + 2 * b1 ) + c;

						} else {

							// region 1

							s1 = segExtent;
							s0 = fmax( 0, - ( a01 * s1 + b0 ) );
							sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

						}

					} else {

						// region 5

						s1 = - segExtent;
						s0 = fmax( 0, - ( a01 * s1 + b0 ) );
						sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

					}

				} else {

					if ( s1 <= - extDet ) {
						// region 4
						s0 = fmax( 0, - ( - a01 * segExtent + b0 ) );
						s1 = ( s0 > 0 ) ? - segExtent : fmin( fmax( - segExtent, - b1 ), segExtent );
						sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

					} else if ( s1 <= extDet ) {
						// region 3
						s0 = 0;
						s1 = fmin( fmax( - segExtent, - b1 ), segExtent );
						sqrDist = s1 * ( s1 + 2 * b1 ) + c;

					} else {
						// region 2
						s0 = fmax( 0, - ( a01 * segExtent + b0 ) );
						s1 = ( s0 > 0 ) ? segExtent : fmin( fmax( - segExtent, - b1 ), segExtent );
						sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

					}

				}

			} else {
				// Ray and segment are parallel.
				s1 = ( a01 > 0 ) ? - segExtent : segExtent;
				s0 = fmax( 0, - ( a01 * s1 + b0 ) );
				sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

			}

			if ( optionalPointOnRay != nullptr ) {
				optionalPointOnRay->copy( direction ).multiplyScalar( s0 ).add( origin );
			}

			if ( optionalPointOnSegment != nullptr ) {
				optionalPointOnSegment->copy( _segDir ).multiplyScalar( s1 ).add( _segCenter );
			}

			return sqrDist;
		}

		shared_ptr<Vector3> intersectSphere( Sphere& sphere, shared_ptr<Vector3> target ) {
			_vector.subVectors( sphere.center, origin );
			const double tca = _vector.dot( direction );
			const double d2 = _vector.dot( _vector ) - tca * tca;
			const double radius2 = sphere.radius * sphere.radius;

			if ( d2 > radius2 ) return nullptr;

			const double thc = sqrt( radius2 - d2 );

			// t0 = first intersect point - entrance on front of sphere
			const double t0 = tca - thc;

			// t1 = second intersect point - exit point on back of sphere
			const double t1 = tca + thc;

			// test to see if both t0 and t1 are behind the ray - if so, return null
			if ( t0 < 0 && t1 < 0 ) return nullptr;

			
			// test to see if t0 is behind the ray:
			// if it is, the ray is inside the sphere, so return the second exit point scaled by t1,
			// in order to always return an intersect point that is in front of the ray.
			if ( t0 < 0 ) return std::make_shared<Vector3>(at( t1, *target ));

			// else t0 is in front of the ray, so return the first collision point scaled by t0
			return std::make_shared<Vector3>(at( t0, *target ));
		}

		bool intersectsSphere( Sphere& sphere ) {
			return distanceSqToPoint( sphere.center ) <= ( sphere.radius * sphere.radius );
		}

	// distanceToPlane( plane ) {

	// 	const denominator = plane.normal.dot( this.direction );

	// 	if ( denominator === 0 ) {

	// 		// line is coplanar, return origin
	// 		if ( plane.distanceToPoint( this.origin ) === 0 ) {

	// 			return 0;

	// 		}

	// 		// Null is preferable to undefined since undefined means.... it is undefined

	// 		return null;

	// 	}

	// 	const t = - ( this.origin.dot( plane.normal ) + plane.constant ) / denominator;

	// 	// Return if the ray never intersects the plane

	// 	return t >= 0 ? t : null;

	// }

	// intersectPlane( plane, target ) {

	// 	const t = this.distanceToPlane( plane );

	// 	if ( t === null ) {

	// 		return null;

	// 	}

	// 	return this.at( t, target );

	// }

	// intersectsPlane( plane ) {

	// 	// check if the ray lies on the plane first

	// 	const distToPoint = plane.distanceToPoint( this.origin );

	// 	if ( distToPoint === 0 ) {

	// 		return true;

	// 	}

	// 	const denominator = plane.normal.dot( this.direction );

	// 	if ( denominator * distToPoint < 0 ) {

	// 		return true;

	// 	}

	// 	// ray origin is behind the plane (and is pointing behind it)

	// 	return false;

	// }

		shared_ptr<Vector3> intersectBox( Box3& box, shared_ptr<Vector3> target ) {
			double tmin, tmax, tymin, tymax, tzmin, tzmax;

			const double invdirx = 1 / direction.x,
				invdiry = 1 / direction.y,
				invdirz = 1 / direction.z;

			const Vector3 origin = this->origin;

			if ( invdirx >= 0 ) {
				tmin = ( box.min.x - origin.x ) * invdirx;
				tmax = ( box.max.x - origin.x ) * invdirx;
			} else {
				tmin = ( box.max.x - origin.x ) * invdirx;
				tmax = ( box.min.x - origin.x ) * invdirx;
			}

			if ( invdiry >= 0 ) {
				tymin = ( box.min.y - origin.y ) * invdiry;
				tymax = ( box.max.y - origin.y ) * invdiry;
			} else {
				tymin = ( box.max.y - origin.y ) * invdiry;
				tymax = ( box.min.y - origin.y ) * invdiry;
			}

			if ( ( tmin > tymax ) || ( tymin > tmax ) ) return nullptr;

			// These lines also handle the case where tmin or tmax is NaN
			// (result of 0 * Infinity). x !== x returns true if x is NaN

			if ( tymin > tmin || tmin != tmin ) tmin = tymin;

			if ( tymax < tmax || tmax != tmax ) tmax = tymax;

			if ( invdirz >= 0 ) {
				tzmin = ( box.min.z - origin.z ) * invdirz;
				tzmax = ( box.max.z - origin.z ) * invdirz;
			} else {
				tzmin = ( box.max.z - origin.z ) * invdirz;
				tzmax = ( box.min.z - origin.z ) * invdirz;
			}

			if ( ( tmin > tzmax ) || ( tzmin > tmax ) ) return nullptr;

			if ( tzmin > tmin || tmin != tmin ) tmin = tzmin;

			if ( tzmax < tmax || tmax != tmax ) tmax = tzmax;

			//return point closest to the ray (positive side)

			if ( tmax < 0 ) return nullptr;

			return std::make_shared<Vector3>(at( tmin >= 0 ? tmin : tmax, *target ));
		}

		bool intersectsBox( Box3& box ) {
			return intersectBox( box, std::make_shared<Vector3>(_vector) ) != nullptr;
		}

	// intersectTriangle( a, b, c, backfaceCulling, target ) {

	// 	// Compute the offset origin, edges, and normal.

	// 	// from https://github.com/pmjoniak/GeometricTools/blob/master/GTEngine/Include/Mathematics/GteIntrRay3Triangle3.h

	// 	_edge1.subVectors( b, a );
	// 	_edge2.subVectors( c, a );
	// 	_normal.crossVectors( _edge1, _edge2 );

	// 	// Solve Q + t*D = b1*E1 + b2*E2 (Q = kDiff, D = ray direction,
	// 	// E1 = kEdge1, E2 = kEdge2, N = Cross(E1,E2)) by
	// 	//   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
	// 	//   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
	// 	//   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
	// 	let DdN = this.direction.dot( _normal );
	// 	let sign;

	// 	if ( DdN > 0 ) {

	// 		if ( backfaceCulling ) return null;
	// 		sign = 1;

	// 	} else if ( DdN < 0 ) {

	// 		sign = - 1;
	// 		DdN = - DdN;

	// 	} else {

	// 		return null;

	// 	}

	// 	_diff.subVectors( this.origin, a );
	// 	const DdQxE2 = sign * this.direction.dot( _edge2.crossVectors( _diff, _edge2 ) );

	// 	// b1 < 0, no intersection
	// 	if ( DdQxE2 < 0 ) {

	// 		return null;

	// 	}

	// 	const DdE1xQ = sign * this.direction.dot( _edge1.cross( _diff ) );

	// 	// b2 < 0, no intersection
	// 	if ( DdE1xQ < 0 ) {

	// 		return null;

	// 	}

	// 	// b1+b2 > 1, no intersection
	// 	if ( DdQxE2 + DdE1xQ > DdN ) {

	// 		return null;

	// 	}

	// 	// Line intersects triangle, check if ray does.
	// 	const QdN = - sign * _diff.dot( _normal );

	// 	// t < 0, no intersection
	// 	if ( QdN < 0 ) {

	// 		return null;

	// 	}

	// 	// Ray intersects triangle.
	// 	return this.at( QdN / DdN, target );

	// }

		Ray& applyMatrix4( Matrix4& matrix4 ) {
			origin.applyMatrix4( matrix4 );
			direction.transformDirection( matrix4 );

			return *this;
		}

		bool equals( Ray& ray ) {
			return ray.origin.equals( origin ) && ray.direction.equals( direction );
		}

		Ray clone() {
			return *this;
		}

};



#endif /* SRC_MATH_RAY_H_ */
