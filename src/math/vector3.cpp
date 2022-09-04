#include "vector3.h"
#include "quaternion.h"
#include "matrix3.h"
#include "buffer_attribute.h"

Vector3::Vector3():_vector(std::make_shared<Vector3>()),
                                _quaternion(std::make_shared<Quaternion>()){}


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


Vector3& Vector3::applyEuler(Euler& euler) {
    return this->applyQuaternion( _quaternion->setFromEuler( euler,true ) );
}

Vector3& Vector3::applyAxisAngle(Vector3& axis, double angle) {
    applyQuaternion( _quaternion->setFromAxisAngle(axis,angle));

    return *this;
}

Vector3& Vector3::setFromEuler(Euler& e) {
    this->x = e.x();
    this->y = e.y();
    this->z = e.z();

    return *this;
}

Vector3& Vector3::fromBufferAttribute(BufferAttribute<double>& attribute,int index){
    this->x = attribute.getX( index );
    this->y = attribute.getY( index );
    this->z = attribute.getZ( index );

    return *this;
}


Vector3& Vector3::applyMatrix3(Matrix3& m) {

    const double x = this->x, y = this->y, z = this->z;

    this->x = m.elements[ 0 ] * x + m.elements[ 3 ] * y + m.elements[ 6 ] * z;
    this->y = m.elements[ 1 ] * x + m.elements[ 4 ] * y + m.elements[ 7 ] * z;
    this->z = m.elements[ 2 ] * x + m.elements[ 5 ] * y + m.elements[ 8 ] * z;

    return *this;
}

Vector3& Vector3::applyNormalMatrix(Matrix3& m) {
    return applyMatrix3( m ).normalize();
}