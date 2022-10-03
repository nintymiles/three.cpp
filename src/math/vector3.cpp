#include "vector3.h"
#include "quaternion.h"
#include "euler.h"
#include "matrix4.h"
#include "matrix3.h"
#include "cameras/camera.h"

// #include "buffer_attribute.h"

//如果变量只是内部使用，那么不需要在头文件中定义，尤其是这种需要自身前置声明的交换存储变量
static Quaternion _quaternion;
//std::shared_ptr<Vector3> _vector = std::make_shared<Vector3>();
static Vector3 _vector{};

//segmentation fault  /Users/rensean/Documents/MyGitWorkspace/three.cpp/build/bin/threelibcpp
//上述错误出现在默认构造函数尝试以默认初始化的方法初始化自己
//Vector3::Vector3():_vector(std::make_shared<Vector3>()){}
                                //,_quaternion(std::make_shared<Quaternion>()){}


Vector3& Vector3::fromBufferAttribute(BufferAttribute<double>& attribute,int index){
    // this->x = attribute.getX( index );
    // this->y = attribute.getY( index );
    // this->z = attribute.getZ( index );

    return *this;
}

Vector3& Vector3::applyNormalMatrix(Matrix3& m) {
    return applyMatrix3( m ).normalize();
}

Vector3& Vector3::applyMatrix3(Matrix3& m) {
    const double x = this->x, y = this->y, z = this->z;

    this->x = m.elements[ 0 ] * x + m.elements[ 3 ] * y + m.elements[ 6 ] * z;
    this->y = m.elements[ 1 ] * x + m.elements[ 4 ] * y + m.elements[ 7 ] * z;
    this->z = m.elements[ 2 ] * x + m.elements[ 5 ] * y + m.elements[ 8 ] * z;

    return *this;
}

 Vector3& Vector3::setFromMatrix3Column(Matrix3& m,int index) {
 	return fromArray( m.elements, index * 3 );
 }

Vector3& Vector3::applyAxisAngle(Vector3& axis, double angle) {
    return applyQuaternion(_quaternion.setFromAxisAngle(axis,angle));
}

Vector3& Vector3::applyQuaternion(Quaternion& q) {
    const auto x = this->x, y = this->y, z = this->z;
    const double qx = q.x(), qy = q.y(), qz = q.z(), qw = q.w();

    // calculate quadouble * vector
    const auto ix = qw * x + qy * z - qz * y;
    const auto iy = qw * y + qz * x - qx * z;
    const auto iz = qw * z + qx * y - qy * x;
    const auto iw = - qx * x - qy * y - qz * z;

    // calculate resuldouble * inverse quat
    this->x = ix * qw + iw * - qx + iy * - qz - iz * - qy;
    this->y = iy * qw + iw * - qy + iz * - qx - ix * - qz;
    this->z = iz * qw + iw * - qz + ix * - qy - iy * - qx;

    return *this;
}

Vector3& Vector3::applyEuler(Euler& euler){
    return applyQuaternion( _quaternion.setFromEuler( euler,true ) );
}

Vector3& Vector3::setFromEuler(Euler& e) {
    this->x = e.x();
    this->y = e.y();
    this->z = e.z();

    return *this;
}

Vector3& Vector3::setFromMatrixColumn(Matrix4& m,int index){
	return fromArray(m.elements, index * 4);
}

Vector3& Vector3::setFromMatrixPosition(Matrix4& m){
	this->x = m.elements[ 12 ];
	this->y = m.elements[ 13 ];
	this->z = m.elements[ 14 ];

	return *this;
}

Vector3& Vector3::setFromMatrixScale(Matrix4& m){
	const double sx = setFromMatrixColumn( m, 0).length();
	const double sy = setFromMatrixColumn( m, 1).length();
	const double sz = setFromMatrixColumn( m, 2).length();

	return this->set(sx,sy,sz);
}

Vector3& Vector3::applyMatrix4(Matrix4& m) {
    const double x = this->x, y = this->y, z = this->z;

    const double w = 1 / (m.elements[ 3 ] * x + m.elements[ 7 ] * y + m.elements[ 11 ] * z + m.elements[ 15 ]);

    this->x = ( m.elements[ 0 ] * x + m.elements[ 4 ] * y + m.elements[ 8 ] * z + m.elements[ 12 ] ) * w;
    this->y = ( m.elements[ 1 ] * x + m.elements[ 5 ] * y + m.elements[ 9 ] * z + m.elements[ 13 ] ) * w;
    this->z = ( m.elements[ 2 ] * x + m.elements[ 6 ] * y + m.elements[ 10 ] * z + m.elements[ 14 ] ) * w;

    return *this;
}

Vector3& Vector3::transformDirection(Matrix4& m) {
    // input: Matrix4 affine matrix
    // vector interpreted as a direction
    const double x = this->x, y = this->y, z = this->z;

    this->x = m.elements[ 0 ] * x + m.elements[ 4 ] * y + m.elements[ 8 ] * z;
    this->y = m.elements[ 1 ] * x + m.elements[ 5 ] * y + m.elements[ 9 ] * z;
    this->z = m.elements[ 2 ] * x + m.elements[ 6 ] * y + m.elements[ 10 ] * z;

    return this->normalize();
}


Vector3& Vector3::projectOnPlane(Vector3& planeNormal) {
    _vector.copy(*this).projectOnVector(planeNormal);

    return sub(_vector);
}

Vector3& Vector3::reflect(Vector3& normal) {
    // reflecdouble incidendouble vector off plane orthogonal to normal
    // normal is assumed to have unidouble length

    return sub(_vector.copy(normal).multiplyScalar(2*dot(normal)));
}

Vector3& Vector3::project(Camera& camera) {
    applyMatrix4(camera.matrixWorldInverse ).applyMatrix4(camera.projectionMatrix);

    return *this;
}

Vector3& Vector3::unproject(Camera& camera) {
    return applyMatrix4( camera.projectionMatrixInverse ).applyMatrix4( *camera.matrixWorld );
}


// inline Vector3& applyMatrix4(Vector3& v,Matrix4& m) {
//     const double x = v.x, y = v.y, z = v.z;

//     const double w = 1 / (m.elements[ 3 ] * x + m.elements[ 7 ] * y + m.elements[ 11 ] * z + m.elements[ 15 ]);

//     v.x = ( m.elements[ 0 ] * x + m.elements[ 4 ] * y + m.elements[ 8 ] * z + m.elements[ 12 ] ) * w;
//     v.y = ( m.elements[ 1 ] * x + m.elements[ 5 ] * y + m.elements[ 9 ] * z + m.elements[ 13 ] ) * w;
//     v.z = ( m.elements[ 2 ] * x + m.elements[ 6 ] * y + m.elements[ 10 ] * z + m.elements[ 14 ] ) * w;

//     return v;
// }


// Vector3& setFromMatrixColumn(Vector3& v,Matrix4& m,int index){
//     return v.fromArray(m.elements, index * 4);
// }

// Vector3& setFromMatrixPosition(Vector3& v,Matrix4& m){
// 	v.x = m.elements[ 12 ];
// 	v.y = m.elements[ 13 ];
// 	v.z = m.elements[ 14 ];

// 	return v;
// }

// Vector3& setFromMatrixScale(Vector3& v,Matrix4& m){
// 	const double sx = setFromMatrixColumn(v, m, 0).length();
// 	const double sy = setFromMatrixColumn(v, m, 1).length();
// 	const double sz = setFromMatrixColumn(v, m, 2).length();

// 	v.x = sx;
// 	v.y = sy;
// 	v.z = sz;

// 	return v;
// }

// Vector3& transformDirection(Vector3& v,Matrix4& m) {
//     // input: Matrix4 affine matrix
//     // vector interpreted as a direction

//     const double x = v.x, y = v.y, z = v.z;

//     v.x = m.elements[ 0 ] * x + m.elements[ 4 ] * y + m.elements[ 8 ] * z;
//     v.y = m.elements[ 1 ] * x + m.elements[ 5 ] * y + m.elements[ 9 ] * z;
//     v.z = m.elements[ 2 ] * x + m.elements[ 6 ] * y + m.elements[ 10 ] * z;

//     return v.normalize();
// }

// //如果涉及相互引用的其它（类）代码，不应该使用inline，可能等同于类内定义。
// inline Vector3	lerpVectors( const Vector3& v1, const Vector3& v2, double alpha ){
// 	const double x = v1.x + ( v2.x - v1.x ) * alpha;
// 	const double y = v1.y + ( v2.y - v1.y ) * alpha;
// 	const double z = v1.z + ( v2.z - v1.z ) * alpha;

// 	return Vector3(x,y,z);
// }

// Vector3& applyEuler(Vector3& v,Euler& euler){
//     return applyQuaternion( v, _quaternion.setFromEuler( euler,true ) );
// }

// Vector3& setFromEuler(Vector3& v,Euler& e) {
//     v.x = e.x();
//     v.y = e.y();
//     v.z = e.z();

//     return v;
// }

// Vector3& applyQuaternion(Vector3& v,Quaternion& q) {
//     const auto x = v.x, y = v.y, z = v.z;
//     const double qx = q.x(), qy = q.y(), qz = q.z(), qw = q.w();

//     // calculate quadouble * vector
//     const auto ix = qw * x + qy * z - qz * y;
//     const auto iy = qw * y + qz * x - qx * z;
//     const auto iz = qw * z + qx * y - qy * x;
//     const auto iw = - qx * x - qy * y - qz * z;

//     // calculate resuldouble * inverse quat
//     v.x = ix * qw + iw * - qx + iy * - qz - iz * - qy;
//     v.y = iy * qw + iw * - qy + iz * - qx - ix * - qz;
//     v.z = iz * qw + iw * - qz + ix * - qy - iy * - qx;

//     return v;
// }

// Vector3& applyAxisAngle(Vector3& v,Vector3& axis, double angle) {
//     return applyQuaternion( v, _quaternion.setFromAxisAngle(axis,angle));
// }

// Vector3& applyMatrix3(Vector3& v,Matrix3& m) {
//     const double x = v.x, y = v.y, z = v.z;

//     v.x = m.elements[ 0 ] * x + m.elements[ 3 ] * y + m.elements[ 6 ] * z;
//     v.y = m.elements[ 1 ] * x + m.elements[ 4 ] * y + m.elements[ 7 ] * z;
//     v.z = m.elements[ 2 ] * x + m.elements[ 5 ] * y + m.elements[ 8 ] * z;

//     return v;
// }

// Vector3& applyNormalMatrix(Vector3& v,Matrix3& m) {
//     return applyMatrix3( v,m ).normalize();
// }