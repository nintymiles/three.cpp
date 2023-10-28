#include "box3.h"

#include "buffer_attribute.h"
#include "object_3d.h"
#include "plane.h"
#include "triangle.h"

namespace box3 {
Vector3 _points[] = { Vector3(), Vector3(), Vector3(), Vector3(), Vector3(), Vector3(), Vector3(), Vector3() };
Vector3 _vector;
Box3 _box;
Vector3 _center, _extents, _triangleNormal, _testAxis;
Vector3 _v0, _v1, _v2;
Vector3 _f0, _f1, _f2;
}

Box3::Box3(){
    min.set(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
    max.set(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
}

Box3& Box3::set(const Vector3& min, const Vector3& max){
    this->min.copy(min);
    this->max.copy(max);

    return *this;
}

Box3& Box3::setFromArray(const float* array, const unsigned length){
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float minZ = std::numeric_limits<float>::infinity();

    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();
    float maxZ = -std::numeric_limits<float>::infinity();

    for (unsigned i = 0; i < length; i += 3) {

        float x = array[i];
        float y = array[i + 1];
        float z = array[i + 2];

        if (x < minX) minX = x;
        if (y < minY) minY = y;
        if (z < minZ) minZ = z;

        if (x > maxX) maxX = x;
        if (y > maxY) maxY = y;
        if (z > maxZ) maxZ = z;

    }

    this->min.set(minX, minY, minZ);
    this->max.set(maxX, maxY, maxZ);

    return *this;
}

Box3& Box3::setFromBufferAttribute(const BufferAttribute<float>& attribute){
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float minZ = std::numeric_limits<float>::infinity();

    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();
    float maxZ = -std::numeric_limits<float>::infinity();

    for (int i = 0, l = attribute.count; i < l; i++) {

        float x = attribute.getX(i);
        float y = attribute.getY(i);
        float z = attribute.getZ(i);

        if (x < minX) minX = x;
        if (y < minY) minY = y;
        if (z < minZ) minZ = z;

        if (x > maxX) maxX = x;
        if (y > maxY) maxY = y;
        if (z > maxZ) maxZ = z;

    }

    this->min.set(minX, minY, minZ);
    this->max.set(maxX, maxY, maxZ);

    return *this;
}

Box3& Box3::setFromPoints(const std::vector<Vector3> points, const unsigned length){
    makeEmpty();

    for (unsigned i = 0; i < length; i++) {
        expandByPoint(points[i]);
    }

    return *this;
}

Box3& Box3::setFromPoints(Vector3* points, const unsigned length) {
    makeEmpty();

    for (unsigned i = 0; i < length; i++) {
        expandByPoint(points[i]);
    }

    return *this;
}

Box3& Box3::setFromCenterAndSize(const Vector3& center, const Vector3& size){
    Vector3 halfSize = box3::_vector.copy(size).multiplyScalar(0.5f);

    min.copy(center).sub(halfSize);
    max.copy(center).add(halfSize);

    return *this;
}

Box3& Box3::setFromObject(Object3D& object){
    makeEmpty();

    return expandByObject(object);
}

Box3& Box3::clone(Box3* target){
    *target = copy(*this);

    return *target;
}

Box3& Box3::copy(const Box3& box){
    this->min.copy(box.min);
    this->max.copy(box.max);

    return *this;
}

Box3& Box3::makeEmpty(){
    min.x = min.y = min.z = std::numeric_limits<float>::infinity();
    max.x = max.y = max.z = -std::numeric_limits<float>::infinity();

    return *this;
}

bool Box3::isEmpty(){
    // this is a more robust check for empty than ( volume <= 0 ) because volume can get positive with two negative axes

    return (max.x < min.x) || (max.y < min.y) || (max.z < min.z);

}

Vector3& Box3::getCenter(Vector3& target){
    if (isEmpty())
        target.set(0, 0, 0);
    else
        target.addVectors(min, max).multiplyScalar(0.5f);

    return target;

}

Vector3& Box3::getSize(Vector3* target){
    return isEmpty() ? target->set(0, 0, 0) : target->subVectors(max, min);
}

Box3& Box3::expandByPoint(const Vector3& point){
    min.min(point);
    max.max(point);

    return *this;
}

Box3& Box3::expandByVector(const Vector3& vector){
    min.sub(vector);
    max.add(vector);

    return *this;
}

Box3& Box3::expandByScalar(float scalar){
    min.addScalar(-scalar);
    max.addScalar(scalar);

    return *this;
}

Box3& Box3::expandByObject(Object3D& object){
    // Computes the world-axis-aligned bounding box of an object (including its children),
    // accounting for both the object's, and children's, world transforms

    object.updateWorldMatrix(false, false);

    Geometry::sptr geometry = object.geometry;

    if (geometry != nullptr) {

        if (geometry->boundingBox.isEmpty()) {

            geometry->computeBoundingBox();

        }

        box3::_box.copy(geometry->boundingBox);
        box3::_box.applyMatrix4(object.matrixWorld);

        this->Union(box3::_box);

    }

    auto children = object.children;

    for (unsigned i = 0;i<children.size();i++) {

        expandByObject(*children[i]);

    }

    return *this;
}

bool Box3::containsPoint(const Vector3& point){
    return point.x < min.x || point.x > max.x ||
           point.y < min.y || point.y > max.y ||
           point.z < min.z || point.z > max.z ? false : true;
}

bool Box3::containsBox(const Box3& box){
    return min.x <= box.min.x && box.max.x <= max.x &&
           min.y <= box.min.y && box.max.y <= max.y &&
           min.z <= box.min.z && box.max.z <= max.z;
}

void Box3::getParameter(const Vector3& point, Vector3* target){
    target->set(
            (point.x - min.x) / (max.x - min.x),
            (point.y - min.y) / (max.y - min.y),
            (point.z - min.z) / (max.z - min.z)
    );
}

bool Box3::intersectsBox(const Box3& box){
    return box.max.x < min.x || box.min.x > max.x ||
           box.max.y < min.y || box.min.y > max.y ||
           box.max.z < min.z || box.min.z > max.z ? false : true;
}

bool Box3::intersectsSphere(const Sphere& sphere){
    // Find the point on the AABB closest to the sphere center.
    clampPoint(sphere.center, &box3::_vector);

    // If that point is inside the sphere, the AABB and sphere intersect.
    return box3::_vector.distanceToSquared(sphere.center) <= (sphere.radius * sphere.radius);
}

bool Box3::intersectsPlane(const Plane& plane) const{
    // We compute the minimum and maximum dot product values. If those values
    // are on the same side (back or front) of the plane, then there is no intersection.

    float min, max;

    if (plane.normal.x > 0) {

        min = plane.normal.x * this->min.x;
        max = plane.normal.x * this->max.x;

    }
    else {

        min = plane.normal.x * this->max.x;
        max = plane.normal.x * this->min.x;

    }

    if (plane.normal.y > 0) {

        min += plane.normal.y * this->min.y;
        max += plane.normal.y * this->max.y;

    }
    else {

        min += plane.normal.y * this->max.y;
        max += plane.normal.y * this->min.y;

    }

    if (plane.normal.z > 0) {

        min += plane.normal.z * this->min.z;
        max += plane.normal.z * this->max.z;

    }
    else {

        min += plane.normal.z * this->max.z;
        max += plane.normal.z * this->min.z;

    }

    return (min <= -plane.constant && max >= -plane.constant);
}

bool Box3::intersectsTriangle(const Triangle& triangle){
    if (isEmpty()) {

        return false;

    }

    // compute box center and extents
    getCenter(box3::_center);
    box3::_extents.subVectors(max, box3::_center);

    // translate triangle to aabb origin
    box3::_v0.subVectors(triangle.a, box3::_center);
    box3::_v1.subVectors(triangle.b, box3::_center);
    box3::_v2.subVectors(triangle.c, box3::_center);

    // compute edge vectors for triangle
    box3::_f0.subVectors(box3::_v1, box3::_v0);
    box3::_f1.subVectors(box3::_v2, box3::_v1);
    box3::_f2.subVectors(box3::_v0, box3::_v2);

    // test against axes that are given by cross product combinations of the edges of the triangle and the edges of the aabb
    // make an axis testing of each of the 3 sides of the aabb against each of the 3 sides of the triangle = 9 axis of separation
    // axis_ij = u_i x f_j (u0, u1, u2 = face normals of aabb = x,y,z axes vectors since aabb is axis aligned)
    /*float axes[] = {
        0, -_f0.z, _f0.y, 0, -_f1.z, _f1.y, 0, -_f2.z, _f2.y,
            _f0.z, 0, -_f0.x, _f1.z, 0, -_f1.x, _f2.z, 0, -_f2.x,
            -_f0.y, _f0.x, 0, -_f1.y, _f1.x, 0, -_f2.y, _f2.x, 0
    };*/
    std::vector<double> axes = { 0, -box3::_f0.z, box3::_f0.y, 0, -box3::_f1.z, box3::_f1.y, 0, -box3::_f2.z, box3::_f2.y,
                                box3::_f0.z, 0, -box3::_f0.x, box3::_f1.z, 0, -box3::_f1.x, box3::_f2.z, 0, -box3::_f2.x,
                                -box3::_f0.y, box3::_f0.x, 0, -box3::_f1.y, box3::_f1.x, 0, -box3::_f2.y, box3::_f2.x, 0 };

    if (!satForAxes(axes, box3::_v0, box3::_v1, box3::_v2, box3::_extents)) {

        return false;

    }

    // test 3 face normals from the aabb
    axes.clear();
    axes = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    if (!satForAxes(axes, box3::_v0, box3::_v1, box3::_v2, box3::_extents)) {

        return false;

    }

    // finally testing the face normal of the triangle
    // use already existing triangle edge vectors here
    box3::_triangleNormal.crossVectors(box3::_f0, box3::_f1);
    axes.clear();
    axes = { box3::_triangleNormal.x, box3::_triangleNormal.y, box3::_triangleNormal.z };

    return satForAxes(axes, box3::_v0, box3::_v1, box3::_v2, box3::_extents);
}

void Box3::clampPoint(const Vector3& point, Vector3* target){
    target->copy(point);
    target->clamp(min, max);
}

float Box3::distanceToPoint(const Vector3& point){
    Vector3 clampedPoint = box3::_vector.copy(point);
    clampedPoint.clamp(min, max);

    return clampedPoint.sub(point).length();
}

Sphere& Box3::getBoundingSphere(Sphere& target){
    getCenter(target.center);

    target.radius = getSize(&box3::_vector).length() * 0.5f;

    return target;
}

Box3& Box3::intersect(const Box3& box){
    min.max(box.min);
    max.min(box.max);

    // ensure that if there is no overlap, the result is fully empty, not slightly empty with non-inf/+inf values that will cause subsequence intersects to erroneously return valid values.
    if (isEmpty()) makeEmpty();

    return *this;
}

Box3& Box3::Union(const Box3& box){
    min.min(box.min);
    max.max(box.max);

    return *this;
}

Box3& Box3::applyMatrix4(const Matrix4& matrix){
    // transform of empty box is an empty box.
    if (isEmpty()) return *this;

    // NOTE: I am using a binary pattern to specify all 2^3 combinations below
    box3::_points[0].set(min.x, min.y, min.z).applyMatrix4(matrix); // 000
    box3::_points[1].set(min.x, min.y, max.z).applyMatrix4(matrix); // 001
    box3::_points[2].set(min.x, max.y, min.z).applyMatrix4(matrix); // 010
    box3::_points[3].set(min.x, max.y, max.z).applyMatrix4(matrix); // 011
    box3::_points[4].set(max.x, min.y, min.z).applyMatrix4(matrix); // 100
    box3::_points[5].set(max.x, min.y, max.z).applyMatrix4(matrix); // 101
    box3::_points[6].set(max.x, max.y, min.z).applyMatrix4(matrix); // 110
    box3::_points[7].set(max.x, max.y, max.z).applyMatrix4(matrix); // 111

    setFromPoints(box3::_points,8);

    return *this;
}

Box3& Box3::translate(const Vector3& offset){
    min.add(offset);
    max.add(offset);

    return *this;
}

bool Box3::equals(const Box3& box){
    return box.min.equals(min) && box.max.equals(max);
}

bool satForAxes(const std::vector<double> axes, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& extents) {

    for (int i = 0, j = axes.size() - 3; i <= j; i += 3) {
        //todo:fix this
        box3::_testAxis.fromArray((float*)axes.data(), i);
        // project the aabb onto the seperating axis
        float r = extents.x * abs(box3::_testAxis.x) + extents.y * abs(box3::_testAxis.y) + extents.z * abs(box3::_testAxis.z);
        // project all 3 vertices of the triangle onto the seperating axis
        double p0 = v0.dot(box3::_testAxis);
        double p1 = v1.dot(box3::_testAxis);
        double p2 = v2.dot(box3::_testAxis);
        // actual test, basically see if either of the most extreme of the triangle points intersects r
        if (std::max(-std::max(p0, std::max(p1, p2)), std::min(p0, std::min(p1, p2))) > r) {

            // points of the projected triangle are outside the projected half-length of the aabb
            // the axis is seperating and we can exit
            return false;

        }

    }

    return true;
}

//#include "vector3.h"
//#include "sphere.h"
//#include "plane.h"
//#include "triangle.h"
//#include "buffer_attribute.h"
//#include "matrix4.h"
//
////std::shared_ptr<Box3> Box3::_box = std::make_shared<Box3>();
////source of segment fault
////Vector3 Box3::_points[6]= {Vector3(),Vector3(),Vector3(),Vector3(),Vector3(),Vector3()};
//
////以静态变量方式存在于动态内存中，似乎是一种合理的用法
////static std::shared_ptr<Box3> _box;
//// =  std::make_shared<Box3>();
//
//static Box3 _box;
//
//// static shared_ptr<Vector3> _box3_vector = make_shared<Vector3>();
//static Vector3 _box3_vector;
//
//// triangle centered vertices
//// static shared_ptr<Vector3> _triangle_v0 = make_shared<Vector3>();
//// static shared_ptr<Vector3> _triangle_v1 = make_shared<Vector3>();
//// static shared_ptr<Vector3> _triangle_v2 = make_shared<Vector3>();
//static Vector3 _triangle_v0;
//static Vector3 _triangle_v1;
//static Vector3 _triangle_v2;
//
//// triangle edge vectors
//// static shared_ptr<Vector3> _f0 = make_shared<Vector3>();
//// static shared_ptr<Vector3> _f1 = make_shared<Vector3>();
//// static shared_ptr<Vector3> _f2 = make_shared<Vector3>();
//static Vector3 _f0;
//static Vector3 _f1;
//static Vector3 _f2;
//
//// static shared_ptr<Vector3> _center = make_shared<Vector3>();
//// static shared_ptr<Vector3> _extents = make_shared<Vector3>();
//// static shared_ptr<Vector3> _triangleNormal = make_shared<Vector3>();
//// static shared_ptr<Vector3> _testAxis = make_shared<Vector3>();
//static Vector3 _center;
//static Vector3 _extents;
//static Vector3 _triangleNormal;
//static Vector3 _testAxis;
//
//static Vector3 _points[6];
//
//template <typename T>
//Box3& Box3::setFromBufferAttribute( BufferAttribute<T>& attribute ){
//	double minX = DBL_MAX;
//	double minY = DBL_MAX;
//	double minZ = DBL_MAX;
//
//	double maxX = DBL_MIN;
//	double maxY = DBL_MIN;
//	double maxZ = DBL_MIN;
//
//	for ( int i = 0, l = attribute.count; i < l; i ++ ) {
//
//		const double x = attribute.getX( i );
//		const double y = attribute.getY( i );
//		const double z = attribute.getZ( i );
//
//		if ( x < minX ) minX = x;
//		if ( y < minY ) minY = y;
//		if ( z < minZ ) minZ = z;
//
//		if ( x > maxX ) maxX = x;
//		if ( y > maxY ) maxY = y;
//		if ( z > maxZ ) maxZ = z;
//	}
//
//	min.set( minX, minY, minZ );
//	max.set( maxX, maxY, maxZ );
//
//	return *this;
//}
//
//Box3& Box3::setFromCenterAndSize(Vector3& center, Vector3& size) {
//	Vector3 halfSize = _box3_vector.copy(size ).multiplyScalar(0.5 );
//
//	min.copy( center ).sub( halfSize );
//	max.copy( center ).add( halfSize );
//
//	return *this;
//}
//
//double Box3::distanceToPoint(Vector3& point) {
//	Vector3& clampedPoint = _box3_vector.copy(point).clamp(min, max);
//
//	return clampedPoint.sub(point).length();
//}
//
//Box3& Box3::applyMatrix4(Matrix4& matrix) {
//	// transform of empty box is an empty box.
//	if ( isEmpty() ) return *this;
//
//	// NOTE: I am using a binary pattern to specify all 2^3 combinations below
//	_points[ 0 ].set( min.x, min.y, min.z ).applyMatrix4( matrix ); // 000
//	_points[ 1 ].set( min.x, min.y, max.z ).applyMatrix4( matrix ); // 001
//	_points[ 2 ].set( min.x, max.y, min.z ).applyMatrix4( matrix ); // 010
//	_points[ 3 ].set( min.x, max.y, max.z ).applyMatrix4( matrix ); // 011
//	_points[ 4 ].set( max.x, min.y, min.z ).applyMatrix4( matrix ); // 100
//	_points[ 5 ].set( max.x, min.y, max.z ).applyMatrix4( matrix ); // 101
//	_points[ 6 ].set( max.x, max.y, min.z ).applyMatrix4( matrix ); // 110
//	_points[ 7 ].set( max.x, max.y, max.z ).applyMatrix4( matrix ); // 111
//
//	setFromPoints( _points );
//
//	return *this;
//}
//
//bool Box3::intersectsSphere(Sphere& sphere){
// 	// Find the point on the AABB closest to the sphere center.
// 	clampPoint(sphere.center, _box3_vector );
//
// 	// If that point is inside the sphere, the AABB and sphere intersect.
// 	return _box3_vector.distanceToSquared(sphere.center ) <= ( sphere.radius * sphere.radius );
// }
//
// Sphere& Box3::getBoundingSphere( Sphere& target ) {
// 	target.center = getCenter( target.center );
// 	target.radius = getSize(_box3_vector ).length() * 0.5;
//
// 	return target;
// }
//
//bool Box3::intersectsPlane( const Plane& plane ){
// 	// We compute the minimum and maximum dot product values. If those values
// 	// are on the same side (back or front) of the plane, then there is no intersection.
// 	double min, max;
//
// 	if ( plane.normal.x > 0 ) {
// 		min = plane.normal.x * this->min.x;
// 		max = plane.normal.x * this->max.x;
// 	} else {
//
// 		min = plane.normal.x * this->max.x;
// 		max = plane.normal.x * this->min.x;
//
// 	}
//
// 	if ( plane.normal.y > 0 ) {
//
// 		min += plane.normal.y * this->min.y;
// 		max += plane.normal.y * this->max.y;
//
// 	} else {
//
// 		min += plane.normal.y * this->max.y;
// 		max += plane.normal.y * this->min.y;
//
// 	}
//
// 	if ( plane.normal.z > 0 ) {
//
// 		min += plane.normal.z * this->min.z;
// 		max += plane.normal.z * this->max.z;
//
// 	} else {
//
// 		min += plane.normal.z * this->max.z;
// 		max += plane.normal.z * this->min.z;
//
// 	}
//
// 	return ( min <= - plane.constant && max >= - plane.constant );
// }
//
// bool Box3::intersectsTriangle(Triangle& triangle ) {
// 	if ( isEmpty() ) {
// 		return false;
// 	}
//
// 	// compute box center and extents
// 	getCenter( _center );
// 	_extents.subVectors( max, _center );
//
// 	// translate triangle to aabb origin
// 	_triangle_v0.subVectors(triangle.a, _center );
// 	_triangle_v1.subVectors(triangle.b, _center );
// 	_triangle_v2.subVectors(triangle.c, _center );
//
// 	// compute edge vectors for triangle
// 	_f0.subVectors(_triangle_v1, _triangle_v0 );
// 	_f1.subVectors(_triangle_v2, _triangle_v1 );
// 	_f2.subVectors(_triangle_v0, _triangle_v2 );
//
// 	// test against axes that are given by cross product combinations of the edges of the triangle and the edges of the aabb
// 	// make an axis testing of each of the 3 sides of the aabb against each of the 3 sides of the triangle = 9 axis of separation
// 	// axis_ij = u_i x f_j (u0, u1, u2 = face normals of aabb = x,y,z axes vectors since aabb is axis aligned)
// 	double axes[] = {
//            0, -_f0.z, _f0.y, 0, -_f1.z, _f1.y, 0, -_f2.z, _f2.y,
//            _f0.z, 0, -_f0.x, _f1.z, 0, -_f1.x, _f2.z, 0, -_f2.x,
//            -_f0.y, _f0.x, 0, -_f1.y, _f1.x, 0, -_f2.y, _f2.x, 0
//    };
// 	if ( ! satForAxes(axes, 27, _triangle_v0, _triangle_v1, _triangle_v2, _extents ) ) {
// 		return false;
// 	}
//
// 	// test 3 face normals from the aabb
// 	double axes2[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
// 	if ( ! satForAxes(axes2, 9, _triangle_v0, _triangle_v1, _triangle_v2, _extents ) ) {
// 		return false;
// 	}
//
// 	// finally testing the face normal of the triangle
// 	// use already existing triangle edge vectors here
// 	_triangleNormal.crossVectors( _f0, _f1 );
// 	double axes3[] = { _triangleNormal.x, _triangleNormal.y, _triangleNormal.z };
//
// 	return satForAxes(axes3, 3, _triangle_v0, _triangle_v1, _triangle_v2, _extents );
// }
//
//// Box3& expandByObject(Object3d& object, bool precise = false ) {
//// 	// Computes the world-axis-aligned bounding box of an object (including its children),
//// 	// accounting for both the object's, and children's, world transforms
//// 	object.updateWorldMatrix( false, false );
//
//// 	const geometry = object.geometry;
//
//// 	if ( geometry !== undefined ) {
//
//// 		if ( precise && geometry.attributes != undefined && geometry.attributes.position !== undefined ) {
//
//// 			const position = geometry.attributes.position;
//// 			for ( let i = 0, l = position.count; i < l; i ++ ) {
//
//// 				_box3_vector.fromBufferAttribute( position, i ).applyMatrix4( object.matrixWorld );
//// 				this.expandByPoint( _box3_vector );
//
//// 			}
//
//// 		} else {
//
//// 			if ( geometry.boundingBox === null ) {
//
//// 				geometry.computeBoundingBox();
//
//// 			}
//
//// 			_box.copy( geometry.boundingBox );
//// 			_box.applyMatrix4( object.matrixWorld );
//
//// 			this.union( _box );
//
//// 		}
//
//// 	}
//
//// 	const children = object.children;
//
//// 	for ( let i = 0, l = children.length; i < l; i ++ ) {
//
//// 		this.expandByObject( children[ i ], precise );
//
//// 	}
//
//// 	return this;
//
//// }
//
////error: 'static' can only be specified inside the class definition
//bool Box3::satForAxes(double axes[],int length,Vector3& v0,Vector3& v1,Vector3& v2,Vector3& extents) {
//	for ( int i = 0, j = length - 3; i <= j; i += 3 ) {
//
//		_testAxis.fromArray( axes, i );
//		// project the aabb onto the separating axis
//		const double r = extents.x * abs( _testAxis.x ) + extents.y * abs( _testAxis.y ) + extents.z * abs( _testAxis.z );
//		// project all 3 vertices of the triangle onto the separating axis
//		const double p0 = v0.dot( _testAxis );
//		const double p1 = v1.dot( _testAxis );
//		const double p2 = v2.dot( _testAxis );
//        //对于cpp的min/max函数，fmin/fmax为double精度，fminf/fmaxf为float精度
//		// actual test, basically see if either of the most extreme of the triangle points intersects r
//		if ( fmax( - fmax( p0, fmax(p1, p2) ), fmin( p0, fmin(p1, p2) ) ) > r ) {
//			// points of the projected triangle are outside the projected half-length of the aabb
//			// the axis is separating and we can exit
//			return false;
//
//		}
//
//	}
//	return true;
//}
