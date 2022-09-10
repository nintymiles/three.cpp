#ifndef BOX3_H
#define BOX3_H

#include "vector3.h"

#include <climits>
#include <cfloat>
#include <memory>
#include <vector>

template<typename T> class BufferAttribute;
using std::vector;

class Box3 {
    private: 
		//以静态变量方式存在于动态内存中，似乎是一种合理的用法	
		static std::shared_ptr<Box3> _box;
		// =  std::make_shared<Box3>();

		// static shared_ptr<Vector3> _vector = make_shared<Vector3>();
		static Vector3 _vector;

		// triangle centered vertices
		// static shared_ptr<Vector3> _v0 = make_shared<Vector3>();
		// static shared_ptr<Vector3> _v1 = make_shared<Vector3>();
		// static shared_ptr<Vector3> _v2 = make_shared<Vector3>();
		static Vector3 _v0;
		static Vector3 _v1;
		static Vector3 _v2;

		// triangle edge vectors
		// static shared_ptr<Vector3> _f0 = make_shared<Vector3>();
		// static shared_ptr<Vector3> _f1 = make_shared<Vector3>();
		// static shared_ptr<Vector3> _f2 = make_shared<Vector3>();
		static Vector3 _f0;
		static Vector3 _f1;
		static Vector3 _f2;

		// static shared_ptr<Vector3> _center = make_shared<Vector3>();
		// static shared_ptr<Vector3> _extents = make_shared<Vector3>();
		// static shared_ptr<Vector3> _triangleNormal = make_shared<Vector3>();
		// static shared_ptr<Vector3> _testAxis = make_shared<Vector3>();
		static Vector3 _center;
		static Vector3 _extents;
		static Vector3 _triangleNormal;
		static Vector3 _testAxis;

		static Vector3 _points[6]; 

    public:
		Vector3 min,max;
        Box3(Vector3 min = Vector3(DBL_MIN,DBL_MIN,DBL_MIN),Vector3 max = Vector3(DBL_MAX,DBL_MAX,DBL_MAX)):min(min),max(max){}
        
        Box3& set(Vector3 min,Vector3 max){
            min.copy(min);
            max.copy(max);

            return *this;
        }

		//使用数组的地方同时提供vector实现
        Box3& setFromVector(vector<double> vec) {
            //初始值为反方向数值极值
            double minX = DBL_MAX;
            double minY = DBL_MAX;
            double minZ = DBL_MAX;

            double maxX = DBL_MIN;
            double maxY = DBL_MIN;
            double maxZ = DBL_MIN;

            for ( int i = 0, l = vec.size(); i < l; i += 3 ) {

                const double x = vec[ i ];
                const double y = vec[ i + 1 ];
                const double z = vec[ i + 2 ];

                if ( x < minX ) minX = x;
                if ( y < minY ) minY = y;
                if ( z < minZ ) minZ = z;

                if ( x > maxX ) maxX = x;
                if ( y > maxY ) maxY = y;
                if ( z > maxZ ) maxZ = z;

            }

            min.set( minX, minY, minZ );
            max.set( maxX, maxY, maxZ );

            return *this;
        }

		//数组的使用可能引起麻烦
		Box3& setFromArray(double array[],int arrLen) {
			double minX = DBL_MAX;
			double minY = DBL_MAX;
			double minZ = DBL_MAX;
			double maxX = DBL_MIN;
			double maxY = DBL_MIN;
			double maxZ = DBL_MIN;

			for ( int i = 0, l = arrLen; i < l; i += 3 ) {

				const double x = array[ i ];
				const double y = array[ i + 1 ];
				const double z = array[ i + 2 ];

				if ( x < minX ) minX = x;
				if ( y < minY ) minY = y;
				if ( z < minZ ) minZ = z;

				if ( x > maxX ) maxX = x;
				if ( y > maxY ) maxY = y;
				if ( z > maxZ ) maxZ = z;

			}

			min.set( minX, minY, minZ );
			max.set( maxX, maxY, maxZ );

			return *this;
		}

		template <typename T>
		Box3& setFromBufferAttribute( BufferAttribute<T>& attribute ); 

        Box3& setFromPoints(std::vector<Vector3>& points) {
            makeEmpty();

            for ( int i = 0, il = points.size(); i < il; i ++ ) {
                expandByPoint( points[ i ] );
            }

            return *this;
        }

		Box3& setFromPoints(Vector3 points[]) {
            makeEmpty();

            for ( int i = 0, il = 6; i < il; i ++ ) {
                expandByPoint( points[ i ] );
            }

            return *this;
        }

		Box3& setFromCenterAndSize(Vector3& center, Vector3& size) {
			Vector3 halfSize = _vector.copy( size ).multiplyScalar( 0.5 );

			min.copy( center ).sub( halfSize );
			max.copy( center ).add( halfSize );

			return *this;
		}

	// setFromObject( object, precise = false ) {

	// 	this.makeEmpty();

	// 	return this.expandByObject( object, precise );

	// }

	// clone() {

	// 	return new this.constructor().copy( this );

	// }

		Box3& copy(Box3& box) {
			min.copy( box.min );
			max.copy( box.max );

			return *this;
		}

        Box3& makeEmpty() {
            min.x = min.y = min.z = DBL_MAX;
            max.x = max.y = max.z = DBL_MIN;

            return *this;
        }

		bool isEmpty() {
			// this is a more robust check for empty than ( volume <= 0 ) because volume can get positive with two negative axes
			return ( max.x < min.x ) || ( max.y < min.y ) || ( max.z < min.z );

		}

		// getCenter( target ) {

		// 	return this.isEmpty() ? target.set( 0, 0, 0 ) : target.addVectors( this.min, this.max ).multiplyScalar( 0.5 );

		// }

		Vector3& getCenter(Vector3& target) {
			//Vector3 zeroVec(0,0,0);
			return isEmpty() ? target.set(0,0,0) : target.addVectors(min,max).multiplyScalar(0.5);
		}

		Vector3& getSize(Vector3& target) {
			return isEmpty() ? target.set(0, 0, 0) : target.subVectors(max, min);
		}

		Box3& expandByPoint(Vector3& point) {
			min.min(point);
			max.max(point);

			return *this;
		}

		Box3& expandByVector(Vector3& vector) {
			min.sub( vector );
			max.add( vector );

			return *this;
		}

		Box3& expandByScalar(unsigned scalar) {
			min.addScalar( - scalar );
			max.addScalar( scalar );

			return *this;
		}

		// Box3& expandByObject(Object3d& object, bool precise = false ) {
		// 	// Computes the world-axis-aligned bounding box of an object (including its children),
		// 	// accounting for both the object's, and children's, world transforms
		// 	object.updateWorldMatrix( false, false );

		// 	const geometry = object.geometry;

		// 	if ( geometry !== undefined ) {

		// 		if ( precise && geometry.attributes != undefined && geometry.attributes.position !== undefined ) {

		// 			const position = geometry.attributes.position;
		// 			for ( let i = 0, l = position.count; i < l; i ++ ) {

		// 				_vector.fromBufferAttribute( position, i ).applyMatrix4( object.matrixWorld );
		// 				this.expandByPoint( _vector );

		// 			}

		// 		} else {

		// 			if ( geometry.boundingBox === null ) {

		// 				geometry.computeBoundingBox();

		// 			}

		// 			_box.copy( geometry.boundingBox );
		// 			_box.applyMatrix4( object.matrixWorld );

		// 			this.union( _box );

		// 		}

		// 	}

		// 	const children = object.children;

		// 	for ( let i = 0, l = children.length; i < l; i ++ ) {

		// 		this.expandByObject( children[ i ], precise );

		// 	}

		// 	return this;

		// }

		bool containsPoint(Vector3& point) {
			return point.x < min.x || point.x > max.x ||
				point.y < min.y || point.y > max.y ||
				point.z < min.z || point.z > max.z ? false : true;
		}

		bool containsBox(Box3& box) {
			return min.x <= box.min.x && box.max.x <= max.x &&
				min.y <= box.min.y && box.max.y <= max.y &&
				min.z <= box.min.z && box.max.z <= max.z;

		}

		Vector3& getParameter(Vector3& point,Vector3& target) {
			// This can potentially have a divide by zero if the box
			// has a size dimension of 0.
			return target.set(
				( point.x - min.x ) / ( max.x - min.x ),
				( point.y - min.y ) / ( max.y - min.y ),
				( point.z - min.z ) / ( max.z - min.z )
			);
		}

		bool intersectsBox(Box3& box) {
			// using 6 splitting planes to rule out intersections.
			return box.max.x < min.x || box.min.x > max.x ||
				box.max.y < min.y || box.min.y > max.y ||
				box.max.z < min.z || box.min.z > max.z ? false : true;
		}

		// intersectsSphere( sphere ) {
		// 	// Find the point on the AABB closest to the sphere center.
		// 	this.clampPoint( sphere.center, _vector );

		// 	// If that point is inside the sphere, the AABB and sphere intersect.
		// 	return _vector.distanceToSquared( sphere.center ) <= ( sphere.radius * sphere.radius );
		// }

	// intersectsPlane( plane ) {

	// 	// We compute the minimum and maximum dot product values. If those values
	// 	// are on the same side (back or front) of the plane, then there is no intersection.

	// 	let min, max;

	// 	if ( plane.normal.x > 0 ) {

	// 		min = plane.normal.x * this.min.x;
	// 		max = plane.normal.x * this.max.x;

	// 	} else {

	// 		min = plane.normal.x * this.max.x;
	// 		max = plane.normal.x * this.min.x;

	// 	}

	// 	if ( plane.normal.y > 0 ) {

	// 		min += plane.normal.y * this.min.y;
	// 		max += plane.normal.y * this.max.y;

	// 	} else {

	// 		min += plane.normal.y * this.max.y;
	// 		max += plane.normal.y * this.min.y;

	// 	}

	// 	if ( plane.normal.z > 0 ) {

	// 		min += plane.normal.z * this.min.z;
	// 		max += plane.normal.z * this.max.z;

	// 	} else {

	// 		min += plane.normal.z * this.max.z;
	// 		max += plane.normal.z * this.min.z;

	// 	}

	// 	return ( min <= - plane.constant && max >= - plane.constant );

	// }

	// intersectsTriangle( triangle ) {

	// 	if ( this.isEmpty() ) {

	// 		return false;

	// 	}

	// 	// compute box center and extents
	// 	this.getCenter( _center );
	// 	_extents.subVectors( this.max, _center );

	// 	// translate triangle to aabb origin
	// 	_v0.subVectors( triangle.a, _center );
	// 	_v1.subVectors( triangle.b, _center );
	// 	_v2.subVectors( triangle.c, _center );

	// 	// compute edge vectors for triangle
	// 	_f0.subVectors( _v1, _v0 );
	// 	_f1.subVectors( _v2, _v1 );
	// 	_f2.subVectors( _v0, _v2 );

	// 	// test against axes that are given by cross product combinations of the edges of the triangle and the edges of the aabb
	// 	// make an axis testing of each of the 3 sides of the aabb against each of the 3 sides of the triangle = 9 axis of separation
	// 	// axis_ij = u_i x f_j (u0, u1, u2 = face normals of aabb = x,y,z axes vectors since aabb is axis aligned)
	// 	let axes = [
	// 		0, - _f0.z, _f0.y, 0, - _f1.z, _f1.y, 0, - _f2.z, _f2.y,
	// 		_f0.z, 0, - _f0.x, _f1.z, 0, - _f1.x, _f2.z, 0, - _f2.x,
	// 		- _f0.y, _f0.x, 0, - _f1.y, _f1.x, 0, - _f2.y, _f2.x, 0
	// 	];
	// 	if ( ! satForAxes( axes, _v0, _v1, _v2, _extents ) ) {

	// 		return false;

	// 	}

	// 	// test 3 face normals from the aabb
	// 	axes = [ 1, 0, 0, 0, 1, 0, 0, 0, 1 ];
	// 	if ( ! satForAxes( axes, _v0, _v1, _v2, _extents ) ) {

	// 		return false;

	// 	}

	// 	// finally testing the face normal of the triangle
	// 	// use already existing triangle edge vectors here
	// 	_triangleNormal.crossVectors( _f0, _f1 );
	// 	axes = [ _triangleNormal.x, _triangleNormal.y, _triangleNormal.z ];

	// 	return satForAxes( axes, _v0, _v1, _v2, _extents );

	// }

		Vector3& clampPoint(Vector3& point,Vector3& target) {
			return target.copy(point).clamp(min,max);
		}

		//使用静态存储变量做计算的中间媒介，可能存在并发问题
		double distanceToPoint(Vector3& point) {
			Vector3& clampedPoint = _vector.copy(point).clamp(min,max);

			return clampedPoint.sub(point).length();
		}

	// getBoundingSphere( target ) {

	// 	this.getCenter( target.center );

	// 	target.radius = this.getSize( _vector ).length() * 0.5;

	// 	return target;

	// }

		Box3& intersect(Box3& box) {

			min.max( box.min );
			max.min( box.max );

			// ensure that if there is no overlap, the result is fully empty, not slightly empty with non-inf/+inf values that will cause subsequence intersects to erroneously return valid values.
			if ( isEmpty() ) makeEmpty();

			return *this;
		}

		Box3& unionBox(Box3& box) {
			min.min( box.min );
			max.max( box.max );

			return *this;
		}

		Box3& applyMatrix4(Matrix4& matrix) {

			// transform of empty box is an empty box.
			if ( isEmpty() ) return *this;

			// NOTE: I am using a binary pattern to specify all 2^3 combinations below
			_points[ 0 ].set( min.x, min.y, min.z ).applyMatrix4( matrix ); // 000
			_points[ 1 ].set( min.x, min.y, max.z ).applyMatrix4( matrix ); // 001
			_points[ 2 ].set( min.x, max.y, min.z ).applyMatrix4( matrix ); // 010
			_points[ 3 ].set( min.x, max.y, max.z ).applyMatrix4( matrix ); // 011
			_points[ 4 ].set( max.x, min.y, min.z ).applyMatrix4( matrix ); // 100
			_points[ 5 ].set( max.x, min.y, max.z ).applyMatrix4( matrix ); // 101
			_points[ 6 ].set( max.x, max.y, min.z ).applyMatrix4( matrix ); // 110
			_points[ 7 ].set( max.x, max.y, max.z ).applyMatrix4( matrix ); // 111

			setFromPoints( _points );

			return *this;

		}

		Box3& translate(Vector3& offset) {
			min.add( offset );
			max.add( offset );

			return *this;
		}

		bool equals(Box3& box) {

			return box.min.equals( min ) && box.max.equals( max );

		}

		static bool satForAxes(double axes[],int length,Vector3& v0,Vector3& v1,Vector3& v2,Vector3& extents) {
			for ( int i = 0, j = length - 3; i <= j; i += 3 ) {

				_testAxis.fromArray( axes, i );
				// project the aabb onto the separating axis
				const double r = extents.x * abs( _testAxis.x ) + extents.y * abs( _testAxis.y ) + extents.z * abs( _testAxis.z );
				// project all 3 vertices of the triangle onto the separating axis
				const double p0 = v0.dot( _testAxis );
				const double p1 = v1.dot( _testAxis );
				const double p2 = v2.dot( _testAxis );
				// actual test, basically see if either of the most extreme of the triangle points intersects r
				if ( fmax( - fmax( p0, fmaxf(p1, p2) ), fmin( p0, fmin(p1, p2) ) ) > r ) {
					// points of the projected triangle are outside the projected half-length of the aabb
					// the axis is separating and we can exit
					return false;

				}

			}
			return true;
		}

};

#endif /* BOX3_H */
