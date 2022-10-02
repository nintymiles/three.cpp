#include "box3.h"
#include "vector3.h"
// #include "object_3d.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"

//std::shared_ptr<Box3> Box3::_box = std::make_shared<Box3>();
//source of segment fault
//Vector3 Box3::_points[6]= {Vector3(),Vector3(),Vector3(),Vector3(),Vector3(),Vector3()}; 

//以静态变量方式存在于动态内存中，似乎是一种合理的用法	
//static std::shared_ptr<Box3> _box;
// =  std::make_shared<Box3>();

static Box3 _box;

// static shared_ptr<Vector3> _vector = make_shared<Vector3>();
static Vector3 _vector;

// triangle centered vertices
// static shared_ptr<Vector3> _triangle_v0 = make_shared<Vector3>();
// static shared_ptr<Vector3> _triangle_v1 = make_shared<Vector3>();
// static shared_ptr<Vector3> _triangle_v2 = make_shared<Vector3>();
static Vector3 _triangle_v0;
static Vector3 _triangle_v1;
static Vector3 _triangle_v2;

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

template <typename T>
Box3& Box3::setFromBufferAttribute( BufferAttribute<T>& attribute ){
	double minX = DBL_MAX;
	double minY = DBL_MAX;
	double minZ = DBL_MAX;

	double maxX = DBL_MIN;
	double maxY = DBL_MIN;
	double maxZ = DBL_MIN;

	for ( int i = 0, l = attribute.count; i < l; i ++ ) {

		const double x = attribute.getX( i );
		const double y = attribute.getY( i );
		const double z = attribute.getZ( i );

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

Box3& Box3::setFromCenterAndSize(Vector3& center, Vector3& size) {
	Vector3 halfSize = _vector.copy( size ).multiplyScalar( 0.5 );

	min.copy( center ).sub( halfSize );
	max.copy( center ).add( halfSize );

	return *this;
}

double Box3::distanceToPoint(Vector3& point) {
	Vector3& clampedPoint = _vector.copy(point).clamp(min,max);

	return clampedPoint.sub(point).length();
}

Box3& Box3::applyMatrix4(Matrix4& matrix) {
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

bool Box3::intersectsSphere(Sphere& sphere){
 	// Find the point on the AABB closest to the sphere center.
 	clampPoint( sphere.center, _vector );

 	// If that point is inside the sphere, the AABB and sphere intersect.
 	return _vector.distanceToSquared( sphere.center ) <= ( sphere.radius * sphere.radius );
 }

 Sphere& Box3::getBoundingSphere( Sphere& target ) {
 	target.center = getCenter( target.center );
 	target.radius = getSize( _vector ).length() * 0.5;

 	return target;
 }

bool Box3::intersectsPlane( const Plane& plane ){
 	// We compute the minimum and maximum dot product values. If those values
 	// are on the same side (back or front) of the plane, then there is no intersection.
 	double min, max;

 	if ( plane.normal.x > 0 ) {
 		min = plane.normal.x * this->min.x;
 		max = plane.normal.x * this->max.x;
 	} else {

 		min = plane.normal.x * this->max.x;
 		max = plane.normal.x * this->min.x;

 	}

 	if ( plane.normal.y > 0 ) {

 		min += plane.normal.y * this->min.y;
 		max += plane.normal.y * this->max.y;

 	} else {

 		min += plane.normal.y * this->max.y;
 		max += plane.normal.y * this->min.y;

 	}

 	if ( plane.normal.z > 0 ) {

 		min += plane.normal.z * this->min.z;
 		max += plane.normal.z * this->max.z;

 	} else {

 		min += plane.normal.z * this->max.z;
 		max += plane.normal.z * this->min.z;

 	}

 	return ( min <= - plane.constant && max >= - plane.constant );
 }

 bool Box3::intersectsTriangle(Triangle& triangle ) {
 	if ( isEmpty() ) {
 		return false;
 	}

 	// compute box center and extents
 	getCenter( _center );
 	_extents.subVectors( max, _center );

 	// translate triangle to aabb origin
 	_triangle_v0.subVectors(triangle.a, _center );
 	_triangle_v1.subVectors(triangle.b, _center );
 	_triangle_v2.subVectors(triangle.c, _center );

 	// compute edge vectors for triangle
 	_f0.subVectors(_triangle_v1, _triangle_v0 );
 	_f1.subVectors(_triangle_v2, _triangle_v1 );
 	_f2.subVectors(_triangle_v0, _triangle_v2 );

 	// test against axes that are given by cross product combinations of the edges of the triangle and the edges of the aabb
 	// make an axis testing of each of the 3 sides of the aabb against each of the 3 sides of the triangle = 9 axis of separation
 	// axis_ij = u_i x f_j (u0, u1, u2 = face normals of aabb = x,y,z axes vectors since aabb is axis aligned)
 	double axes[] = {
            0, -_f0.z, _f0.y, 0, -_f1.z, _f1.y, 0, -_f2.z, _f2.y,
            _f0.z, 0, -_f0.x, _f1.z, 0, -_f1.x, _f2.z, 0, -_f2.x,
            -_f0.y, _f0.x, 0, -_f1.y, _f1.x, 0, -_f2.y, _f2.x, 0
    };
 	if ( ! satForAxes(axes, 27, _triangle_v0, _triangle_v1, _triangle_v2, _extents ) ) {
 		return false;
 	}

 	// test 3 face normals from the aabb
 	double axes2[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
 	if ( ! satForAxes(axes2, 9, _triangle_v0, _triangle_v1, _triangle_v2, _extents ) ) {
 		return false;
 	}

 	// finally testing the face normal of the triangle
 	// use already existing triangle edge vectors here
 	_triangleNormal.crossVectors( _f0, _f1 );
 	double axes3[] = { _triangleNormal.x, _triangleNormal.y, _triangleNormal.z };

 	return satForAxes(axes3, 3, _triangle_v0, _triangle_v1, _triangle_v2, _extents );
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

//error: 'static' can only be specified inside the class definition
bool Box3::satForAxes(double axes[],int length,Vector3& v0,Vector3& v1,Vector3& v2,Vector3& extents) {
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
