#include "vector3.h"

#include "spherical.h"
#include "cylindrical.h"
#include "matrix3.h"
#include "matrix4.h"
#include "quaternion.h"
#include "camera.h"

Vector3::Vector3() :x(0), y(0), z(0) {}

Vector3::Vector3(float s) : x(s), y(s), z(s) {}

Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Vector3::Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

Vector3& Vector3::setFromSpherical(const Spherical& s){
    return setFromSphericalCoords(s.radius, s.phi, s.theta);
}

Vector3& Vector3::setFromSphericalCoords(float r, float phi, float theta){
    float sinPhiRadius = sin(phi) * r;

    x = sinPhiRadius * sin(theta);
    y = cos(phi) * r;
    z = sinPhiRadius * cos(theta);

    return *this;
}

Vector3& Vector3::setFromCylindrical(const Cylindrical& c){
    return setFromCylindricalCoords(c.radius, c.theta, c.y);
}

Vector3& Vector3::setFromCylindricalCoords(float radius, float theta, float y){
    this->x = radius * sin(theta);
    this->y = y;
    this->z = radius * cos(theta);

    return *this;
}

Vector3& Vector3::setFromMatrixPosition(const Matrix4& m){
    x = m.elements[12];
    y = m.elements[13];
    z = m.elements[14];

    return *this;
}

Vector3& Vector3::setFromMatrixScale(const Matrix4& m){
    float sx = setFromMatrixColumn(m, 0).length();
    float sy = setFromMatrixColumn(m, 1).length();
    float sz = setFromMatrixColumn(m, 2).length();

    x = sx;
    y = sy;
    z = sz;

    return *this;
}

Vector3& Vector3::setFromMatrixColumn(const Matrix4& matrix, unsigned index){
    return this->fromArray((float*)matrix.elements, 16,index * 4);
}

Vector3& Vector3::setFromMatrix3Column(const Matrix3& matrix, unsigned index){
    return this->fromArray((float *)matrix.elements, 9,index * 3);
}

Vector3& Vector3::applyQuaternion(const Quaternion& q){
    const float qx = q.getX(), qy = q.getY(), qz = q.getZ(), qw = q.getW();

    // calculate quat * vector
    float ix = qw * x + qy * z - qz * y,
            iy = qw * y + qz * x - qx * z,
            iz = qw * z + qx * y - qy * x,
            iw = -qx * x - qy * y - qz * z;

    // calculate result * inverse quat

    x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
    y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
    z = iz * qw + iw * -qz + ix * -qy - iy * -qx;

    return *this;
}

Vector3& Vector3::applyEuler(const Euler& euler) {
    Quaternion _quaternion;
    return applyQuaternion(_quaternion.setFromEuler(euler,true));
}

Vector3& Vector3::applyAxisAngle(const Vector3& axis, float angle){
    Quaternion _quaternion;
    return applyQuaternion(_quaternion.setFromAxisAngle(axis, angle));
}

Vector3& Vector3::applyMatrix3(const Matrix3& m){
    float _x = x, _y = y, _z = z;
    const float* e = (float *)m.elements;

    x = e[0] * _x + e[3] * _y + e[6] * _z;
    y = e[1] * _x + e[4] * _y + e[7] * _z;
    z = e[2] * _x + e[5] * _y + e[8] * _z;

    return *this;
}

Vector3& Vector3::applyMatrix4(const Matrix4& m){
    float _x = x, _y = y, _z = z;
    const float* e = (float *)m.elements;

    float _w = 1 / (e[3] * x + e[7] * y + e[11] * z + e[15]);

    x = (e[0] * _x + e[4] * _y + e[8] * _z + e[12]) * _w;
    y = (e[1] * _x + e[5] * _y + e[9] * _z + e[13]) * _w;
    z = (e[2] * _x + e[6] * _y + e[10] * _z + e[14]) * _w;

    return *this;
}

Vector3& Vector3::project(const Camera& camera){
    return applyMatrix4(camera.matrixWorldInverse).applyMatrix4(camera.projectionMatrix);
}

Vector3& Vector3::unproject(const Camera& camera){
    return applyMatrix4(camera.projectionMatrixInverse).applyMatrix4(camera.matrixWorld);

}

Vector3& Vector3::transformDirection(const Matrix4& m){
    // input: THREE.Matrix4 affine matrix
    // vector interpreted as a direction

    float _x = x, _y = y, _z = z;
    const float* e = (float *)m.elements;

    x = e[0] * _x + e[4] * _y + e[8] * _z;
    y = e[1] * _x + e[5] * _y + e[9] * _z;
    z = e[2] * _x + e[6] * _y + e[10] * _z;

    return normalize();
}

Vector3& Vector3::min(const Vector3& v){
    x = std::min(x, v.x);
    y = std::min(y, v.y);
    z = std::min(z, v.z);

    return *this;
}

Vector3& Vector3::max(const Vector3& v){
    x = std::max(x, v.x);
    y = std::max(y, v.y);
    z = std::max(z, v.z);

    return *this;
}

Vector3& Vector3::clamp(const Vector3& min, const Vector3& max){
    // assumes min < max, componentwise

    x = std::max(min.x, std::min(max.x, x));
    y = std::max(min.y, std::min(max.y, y));
    z = std::max(min.z, std::min(max.z, z));

    return *this;
}

Vector3& Vector3::clampScalar(float min, float max){
    x = std::max(min, std::min(max, x));
    y = std::max(min, std::min(max, y));
    z = std::max(min, std::min(max, z));

    return *this;
}

Vector3& Vector3::clampLength(float min, float max){
    float len = length();

    len = len == 0 ? 1 : len;

    return divideScalar(len).multiplyScalar(std::max(min, std::min(max, len)));
}

//#include "vector3.h"
//
//#include "quaternion.h"
//#include "euler.h"
//#include "matrix4.h"
//#include "matrix3.h"
//#include "camera.h"
//#include "buffer_attribute.h"
//namespace __vector3 {
////如果变量只是内部使用，那么不需要在头文件中定义，尤其是这种需要自身前置声明的交换存储变量
//    Quaternion _quaternion;
////std::shared_ptr<Vector3> _vector = std::make_shared<Vector3>();
//    Vector3 _vector{};
//}
//using namespace __vector3;
//
////segmentation fault  /Users/rensean/Documents/MyGitWorkspace/three.cpp/build/bin/threelibcpp
////上述错误出现在默认构造函数尝试以默认初始化的方法初始化自己
////Vector3::Vector3():_vector(std::make_shared<Vector3>()){}
//                                //,_quaternion(std::make_shared<Quaternion>()){}
//
//template<typename T>
//Vector3& Vector3::fromBufferAttribute(BufferAttribute<T>& attribute,int index){
//     this->x = attribute.getX( index );
//     this->y = attribute.getY( index );
//     this->z = attribute.getZ( index );
//
//    return *this;
//}
//
//Vector3& Vector3::fromBufferAttribute(BufferAttribute<float>& attribute,int index){
//    this->x = attribute.getX( index );
//    this->y = attribute.getY( index );
//    this->z = attribute.getZ( index );
//
//    return *this;
//}
//
//Vector3& Vector3::applyNormalMatrix(Matrix3& m) {
//    return applyMatrix3( m ).normalize();
//}
//
//Vector3& Vector3::applyMatrix3(Matrix3& m) {
//    const double x = this->x, y = this->y, z = this->z;
//
//    this->x = m.elements[ 0 ] * x + m.elements[ 3 ] * y + m.elements[ 6 ] * z;
//    this->y = m.elements[ 1 ] * x + m.elements[ 4 ] * y + m.elements[ 7 ] * z;
//    this->z = m.elements[ 2 ] * x + m.elements[ 5 ] * y + m.elements[ 8 ] * z;
//
//    return *this;
//}
//
// Vector3& Vector3::setFromMatrix3Column(Matrix3& m,int index) {
// 	return fromArray( m.elements, index * 3 );
// }
//
//Vector3& Vector3::applyAxisAngle(Vector3& axis, double angle) {
//    return applyQuaternion(_quaternion.setFromAxisAngle(axis,angle));
//}
//
//Vector3& Vector3::applyQuaternion(Quaternion& q) {
//    const auto x = this->x, y = this->y, z = this->z;
//    const double qx = q.x(), qy = q.y(), qz = q.z(), qw = q.w();
//
//    // calculate quadouble * vector
//    const auto ix = qw * x + qy * z - qz * y;
//    const auto iy = qw * y + qz * x - qx * z;
//    const auto iz = qw * z + qx * y - qy * x;
//    const auto iw = - qx * x - qy * y - qz * z;
//
//    // calculate resuldouble * inverse quat
//    this->x = ix * qw + iw * - qx + iy * - qz - iz * - qy;
//    this->y = iy * qw + iw * - qy + iz * - qx - ix * - qz;
//    this->z = iz * qw + iw * - qz + ix * - qy - iy * - qx;
//
//    return *this;
//}
//
//Vector3& Vector3::applyEuler(Euler& euler){
//    return applyQuaternion( _quaternion.setFromEuler( euler,true ) );
//}
//
//Vector3& Vector3::setFromEuler(Euler& e) {
//    this->x = e.x();
//    this->y = e.y();
//    this->z = e.z();
//
//    return *this;
//}
//
//Vector3& Vector3::setFromMatrixColumn(const Matrix4& m,int index){
//	return fromArray( m.elements, index * 4);
//}
//
//Vector3& Vector3::setFromMatrixPosition(const Matrix4& m){
//	this->x = m.elements[ 12 ];
//	this->y = m.elements[ 13 ];
//	this->z = m.elements[ 14 ];
//
//	return *this;
//}
//
//Vector3& Vector3::setFromMatrixScale(Matrix4& m){
//	const double sx = setFromMatrixColumn( m, 0).length();
//	const double sy = setFromMatrixColumn( m, 1).length();
//	const double sz = setFromMatrixColumn( m, 2).length();
//
//	return this->set(sx,sy,sz);
//}
//
//Vector3& Vector3::applyMatrix4(const Matrix4& m) {
//    const double x = this->x, y = this->y, z = this->z;
//
//    const double w = 1 / (m.elements[ 3 ] * x + m.elements[ 7 ] * y + m.elements[ 11 ] * z + m.elements[ 15 ]);
//
//    this->x = ( m.elements[ 0 ] * x + m.elements[ 4 ] * y + m.elements[ 8 ] * z + m.elements[ 12 ] ) * w;
//    this->y = ( m.elements[ 1 ] * x + m.elements[ 5 ] * y + m.elements[ 9 ] * z + m.elements[ 13 ] ) * w;
//    this->z = ( m.elements[ 2 ] * x + m.elements[ 6 ] * y + m.elements[ 10 ] * z + m.elements[ 14 ] ) * w;
//
//    return *this;
//}
//
//Vector3& Vector3::transformDirection(Matrix4& m) {
//    // input: Matrix4 affine matrix
//    // vector interpreted as a direction
//    const double x = this->x, y = this->y, z = this->z;
//
//    this->x = m.elements[ 0 ] * x + m.elements[ 4 ] * y + m.elements[ 8 ] * z;
//    this->y = m.elements[ 1 ] * x + m.elements[ 5 ] * y + m.elements[ 9 ] * z;
//    this->z = m.elements[ 2 ] * x + m.elements[ 6 ] * y + m.elements[ 10 ] * z;
//
//    return this->normalize();
//}
//
//
//Vector3& Vector3::projectOnPlane(Vector3& planeNormal) {
//    _vector.copy(*this).projectOnVector(planeNormal);
//
//    return sub(_vector);
//}
//
//Vector3& Vector3::reflect(Vector3& normal) {
//    // reflecdouble incidendouble vector off plane orthogonal to normal
//    // normal is assumed to have unidouble length
//
//    return sub(_vector.copy(normal).multiplyScalar(2*dot(normal)));
//}
//
//Vector3& Vector3::project(Camera& camera) {
////    applyMatrix4(*camera.matrixWorldInverse ).applyMatrix4(*camera.projectionMatrix);
//    applyMatrix4(camera.matrixWorldInverse ).applyMatrix4(camera.projectionMatrix);
//
//    return *this;
//}
//
//Vector3& Vector3::unproject(Camera& camera) {
////    return applyMatrix4( *camera.projectionMatrixInverse ).applyMatrix4( camera.matrixWorld );
//    return applyMatrix4( camera.projectionMatrixInverse ).applyMatrix4( camera.matrixWorld );
//}
//
//
//// inline Vector3& applyMatrix4(Vector3& v,Matrix4& m) {
////     const double x = v.x, y = v.y, z = v.z;
//
////     const double w = 1 / (m.elements[ 3 ] * x + m.elements[ 7 ] * y + m.elements[ 11 ] * z + m.elements[ 15 ]);
//
////     v.x = ( m.elements[ 0 ] * x + m.elements[ 4 ] * y + m.elements[ 8 ] * z + m.elements[ 12 ] ) * w;
////     v.y = ( m.elements[ 1 ] * x + m.elements[ 5 ] * y + m.elements[ 9 ] * z + m.elements[ 13 ] ) * w;
////     v.z = ( m.elements[ 2 ] * x + m.elements[ 6 ] * y + m.elements[ 10 ] * z + m.elements[ 14 ] ) * w;
//
////     return v;
//// }
//
//
//// Vector3& setFromMatrixColumn(Vector3& v,Matrix4& m,int index){
////     return v.fromArray(m.elements, index * 4);
//// }
//
//// Vector3& setFromMatrixPosition(Vector3& v,Matrix4& m){
//// 	v.x = m.elements[ 12 ];
//// 	v.y = m.elements[ 13 ];
//// 	v.z = m.elements[ 14 ];
//
//// 	return v;
//// }
//
//// Vector3& setFromMatrixScale(Vector3& v,Matrix4& m){
//// 	const double sx = setFromMatrixColumn(v, m, 0).length();
//// 	const double sy = setFromMatrixColumn(v, m, 1).length();
//// 	const double sz = setFromMatrixColumn(v, m, 2).length();
//
//// 	v.x = sx;
//// 	v.y = sy;
//// 	v.z = sz;
//
//// 	return v;
//// }
//
//// Vector3& transformDirection(Vector3& v,Matrix4& m) {
////     // input: Matrix4 affine matrix
////     // vector interpreted as a direction
//
////     const double x = v.x, y = v.y, z = v.z;
//
////     v.x = m.elements[ 0 ] * x + m.elements[ 4 ] * y + m.elements[ 8 ] * z;
////     v.y = m.elements[ 1 ] * x + m.elements[ 5 ] * y + m.elements[ 9 ] * z;
////     v.z = m.elements[ 2 ] * x + m.elements[ 6 ] * y + m.elements[ 10 ] * z;
//
////     return v.normalize();
//// }
//
//// //如果涉及相互引用的其它（类）代码，不应该使用inline，可能等同于类内定义。
//// inline Vector3	lerpVectors( const Vector3& v1, const Vector3& v2, double alpha ){
//// 	const double x = v1.x + ( v2.x - v1.x ) * alpha;
//// 	const double y = v1.y + ( v2.y - v1.y ) * alpha;
//// 	const double z = v1.z + ( v2.z - v1.z ) * alpha;
//
//// 	return Vector3(x,y,z);
//// }
//
//// Vector3& applyEuler(Vector3& v,Euler& euler){
////     return applyQuaternion( v, _quaternion.setFromEuler( euler,true ) );
//// }
//
//// Vector3& setFromEuler(Vector3& v,Euler& e) {
////     v.x = e.x();
////     v.y = e.y();
////     v.z = e.z();
//
////     return v;
//// }
//
//// Vector3& applyQuaternion(Vector3& v,Quaternion& q) {
////     const auto x = v.x, y = v.y, z = v.z;
////     const double qx = q.x(), qy = q.y(), qz = q.z(), qw = q.w();
//
////     // calculate quadouble * vector
////     const auto ix = qw * x + qy * z - qz * y;
////     const auto iy = qw * y + qz * x - qx * z;
////     const auto iz = qw * z + qx * y - qy * x;
////     const auto iw = - qx * x - qy * y - qz * z;
//
////     // calculate resuldouble * inverse quat
////     v.x = ix * qw + iw * - qx + iy * - qz - iz * - qy;
////     v.y = iy * qw + iw * - qy + iz * - qx - ix * - qz;
////     v.z = iz * qw + iw * - qz + ix * - qy - iy * - qx;
//
////     return v;
//// }
//
//// Vector3& applyAxisAngle(Vector3& v,Vector3& axis, double angle) {
////     return applyQuaternion( v, _quaternion.setFromAxisAngle(axis,angle));
//// }
//
//// Vector3& applyMatrix3(Vector3& v,Matrix3& m) {
////     const double x = v.x, y = v.y, z = v.z;
//
////     v.x = m.elements[ 0 ] * x + m.elements[ 3 ] * y + m.elements[ 6 ] * z;
////     v.y = m.elements[ 1 ] * x + m.elements[ 4 ] * y + m.elements[ 7 ] * z;
////     v.z = m.elements[ 2 ] * x + m.elements[ 5 ] * y + m.elements[ 8 ] * z;
//
////     return v;
//// }
//
//// Vector3& applyNormalMatrix(Vector3& v,Matrix3& m) {
////     return applyMatrix3( v,m ).normalize();
//// }