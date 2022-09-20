#include "vector3.h"
// #include "quaternion.h"
// #include "euler.h"
#include "matrix4.h"
// #include "matrix3.h"
// #include "buffer_attribute.h"

Vector3::Vector3():_vector(std::make_shared<Vector3>()){}
                                //,_quaternion(std::make_shared<Quaternion>()){}


// Vector3& Vector3::applyQuaternion(Quaternion& q) {

//     const auto x = this->x, y = this->y, z = this->z;
//     const double qx = q.x(), qy = q.y(), qz = q.z(), qw = q.w();

//     // calculate quadouble * vector
//     const auto ix = qw * x + qy * z - qz * y;
//     const auto iy = qw * y + qz * x - qx * z;
//     const auto iz = qw * z + qx * y - qy * x;
//     const auto iw = - qx * x - qy * y - qz * z;

//     // calculate resuldouble * inverse quat
//     this->x = ix * qw + iw * - qx + iy * - qz - iz * - qy;
//     this->y = iy * qw + iw * - qy + iz * - qx - ix * - qz;
//     this->z = iz * qw + iw * - qz + ix * - qy - iy * - qx;

//     return *this;
// }


// Vector3& Vector3::applyEuler(Euler& euler) {
//     return this->applyQuaternion( _quaternion->setFromEuler( euler,true ) );
// }

// Vector3& Vector3::applyAxisAngle(Vector3& axis, double angle) {
//     applyQuaternion( _quaternion->setFromAxisAngle(axis,angle));

//     return *this;
// }

// Vector3& Vector3::setFromEuler(Euler& e) {
//     this->x = e.x();
//     this->y = e.y();
//     this->z = e.z();

//     return *this;
// }

// Vector3& Vector3::fromBufferAttribute(BufferAttribute<double>& attribute,int index){
//     // this->x = attribute.getX( index );
//     // this->y = attribute.getY( index );
//     // this->z = attribute.getZ( index );

//     return *this;
// }


// Vector3& Vector3::applyMatrix3(Matrix3& m) {

//     const double x = this->x, y = this->y, z = this->z;

//     this->x = m.elements[ 0 ] * x + m.elements[ 3 ] * y + m.elements[ 6 ] * z;
//     this->y = m.elements[ 1 ] * x + m.elements[ 4 ] * y + m.elements[ 7 ] * z;
//     this->z = m.elements[ 2 ] * x + m.elements[ 5 ] * y + m.elements[ 8 ] * z;

//     return *this;
// }

// Vector3& Vector3::applyNormalMatrix(Matrix3& m) {
//     return applyMatrix3( m ).normalize();
// }



// Vector3& Vector3::setFromMatrixColumn(Matrix4& m,int index){
// 	return fromArray(m.elements, index * 4);
// }
    


inline Vector3& applyMatrix4(Vector3& v,Matrix4& m) {
    const double x = v.x, y = v.y, z = v.z;

    const double w = 1 / (m.elements[ 3 ] * x + m.elements[ 7 ] * y + m.elements[ 11 ] * z + m.elements[ 15 ]);

    v.x = ( m.elements[ 0 ] * x + m.elements[ 4 ] * y + m.elements[ 8 ] * z + m.elements[ 12 ] ) * w;
    v.y = ( m.elements[ 1 ] * x + m.elements[ 5 ] * y + m.elements[ 9 ] * z + m.elements[ 13 ] ) * w;
    v.z = ( m.elements[ 2 ] * x + m.elements[ 6 ] * y + m.elements[ 10 ] * z + m.elements[ 14 ] ) * w;

    return v;
}

inline Vector3& setFromMatrixColumn(Vector3& v,Matrix4& m,int index){
    return v.fromArray(m.elements, index * 4);
}


inline Vector3& setFromMatrixPosition(Vector3& v,Matrix4& m){
	v.x = m.elements[ 12 ];
	v.y = m.elements[ 13 ];
	v.z = m.elements[ 14 ];

	return v;
}

inline Vector3& setFromMatrixScale(Vector3& v,Matrix4& m){
	const double sx = setFromMatrixColumn(v, m, 0).length();
	const double sy = setFromMatrixColumn(v, m, 1).length();
	const double sz = setFromMatrixColumn(v, m, 2).length();

	v.x = sx;
	v.y = sy;
	v.z = sz;

	return v;
}

inline Vector3& transformDirection(Vector3& v,Matrix4& m) {
    // input: Matrix4 affine matrix
    // vector interpreted as a direction

    const double x = v.x, y = v.y, z = v.z;

    v.x = m.elements[ 0 ] * x + m.elements[ 4 ] * y + m.elements[ 8 ] * z;
    v.y = m.elements[ 1 ] * x + m.elements[ 5 ] * y + m.elements[ 9 ] * z;
    v.z = m.elements[ 2 ] * x + m.elements[ 6 ] * y + m.elements[ 10 ] * z;

    return v.normalize();
}

inline Vector3	lerpVectors( const Vector3& v1, const Vector3& v2, double alpha ){
	const double x = v1.x + ( v2.x - v1.x ) * alpha;
	const double y = v1.y + ( v2.y - v1.y ) * alpha;
	const double z = v1.z + ( v2.z - v1.z ) * alpha;

	return Vector3(x,y,z);
}

