#include "matrix4.h"
#include "vector3.h"
#include "quaternion.h"

Matrix4::Matrix4():_v1(std::make_shared<Vector3>(new Vector3())),
    _m1(std::make_shared<Matrix4>(new Matrix4())),
    _zero(std::make_shared<Vector3>(new Vector3(0,0,0))),
    _one(std::make_shared<Vector3>(new Vector3(0,0,1))),
    _x(std::make_shared<Vector3>(new Vector3())),
    _y(std::make_shared<Vector3>(new Vector3())),
    _z(std::make_shared<Vector3>(new Vector3())){};

Matrix4& Matrix4::extractBasis(Vector3& xAxis,Vector3& yAxis,Vector3& zAxis){
    xAxis.setFromMatrixColumn( *this, 0 );
	yAxis.setFromMatrixColumn( *this, 1 );
	zAxis.setFromMatrixColumn( *this, 2 );

	return *this;
}

Matrix4& Matrix4::makeBasis(Vector3& xAxis,Vector3& yAxis,Vector3& zAxis){
    set(
        xAxis.x, yAxis.x, zAxis.x, 0,
        xAxis.y, yAxis.y, zAxis.y, 0,
        xAxis.z, yAxis.z, zAxis.z, 0,
        0, 0, 0, 1
    );
    return *this;
}

Matrix4& Matrix4::extractRotation(Matrix4& m) {
    // this method does not support reflection matrices
    const double scaleX = 1 / _v1->setFromMatrixColumn(m, 0).length();
    const double scaleY = 1 / _v1->setFromMatrixColumn(m, 1).length();
    const double scaleZ = 1 / _v1->setFromMatrixColumn(m, 2).length();

    elements[ 0 ] = m.elements[ 0 ] * scaleX;
    elements[ 1 ] = m.elements[ 1 ] * scaleX;
    elements[ 2 ] = m.elements[ 2 ] * scaleX;
    elements[ 3 ] = 0;

    elements[ 4 ] = m.elements[ 4 ] * scaleY;
    elements[ 5 ] = m.elements[ 5 ] * scaleY;
    elements[ 6 ] = m.elements[ 6 ] * scaleY;
    elements[ 7 ] = 0;

    elements[ 8 ] = m.elements[ 8 ] * scaleZ;
    elements[ 9 ] = m.elements[ 9 ] * scaleZ;
    elements[ 10 ] = m.elements[ 10 ] * scaleZ;
    elements[ 11 ] = 0;

    elements[ 12 ] = 0;
    elements[ 13 ] = 0;
    elements[ 14 ] = 0;
    elements[ 15 ] = 1;

    return *this;
}

Matrix4& Matrix4::compose(const Vector3& position,const Quaternion& quaternion,const Vector3& scale){

    const double x = quaternion.x(), y = quaternion.y(), z = quaternion.z(), w = quaternion.w();
    const double x2 = x + x,	y2 = y + y, z2 = z + z;
    const double xx = x * x2, xy = x * y2, xz = x * z2;
    const double yy = y * y2, yz = y * z2, zz = z * z2;
    const double wx = w * x2, wy = w * y2, wz = w * z2;

    const double sx = scale.x, sy = scale.y, sz = scale.z;

    elements[ 0 ] = ( 1 - ( yy + zz ) ) * sx;
    elements[ 1 ] = ( xy + wz ) * sx;
    elements[ 2 ] = ( xz - wy ) * sx;
    elements[ 3 ] = 0;

    elements[ 4 ] = ( xy - wz ) * sy;
    elements[ 5 ] = ( 1 - ( xx + zz ) ) * sy;
    elements[ 6 ] = ( yz + wx ) * sy;
    elements[ 7 ] = 0;

    elements[ 8 ] = ( xz + wy ) * sz;
    elements[ 9 ] = ( yz - wx ) * sz;
    elements[ 10 ] = ( 1 - ( xx + yy ) ) * sz;
    elements[ 11 ] = 0;

    elements[ 12 ] = position.x;
    elements[ 13 ] = position.y;
    elements[ 14 ] = position.z;
    elements[ 15 ] = 1;

    return *this;
}

Matrix4& Matrix4::decompose(Vector3& position,Quaternion& quaternion,Vector3& scale){
    double sx = (_v1->set(elements[0], elements[1], elements[2])).length();
    const double sy = (_v1->set(elements[4], elements[5], elements[6])).length();
    const double sz =  _v1->set(elements[8], elements[9], elements[10]).length();

    // if determine is negative, we need to invert one scale
    const double det = determinant();
    if (det < 0) sx = - sx;

    position.x = elements[12];
    position.y = elements[13];
    position.z = elements[14];

    // scale the rotation part
    _m1->copy(*this);

    const double invSX = 1 / sx;
    const double invSY = 1 / sy;
    const double invSZ = 1 / sz;

    _m1->elements[ 0 ] *= invSX;
    _m1->elements[ 1 ] *= invSX;
    _m1->elements[ 2 ] *= invSX;

    _m1->elements[ 4 ] *= invSY;
    _m1->elements[ 5 ] *= invSY;
    _m1->elements[ 6 ] *= invSY;

    _m1->elements[ 8 ] *= invSZ;
    _m1->elements[ 9 ] *= invSZ;
    _m1->elements[ 10 ] *= invSZ;

    quaternion.setFromRotationMatrix(*_m1);

    scale.x = sx;
    scale.y = sy;
    scale.z = sz;

    return *this;
}

Matrix4& Matrix4::makeRotationFromQuaternion(Quaternion& q){
    return compose(*_zero, q, *_one);
}

Matrix4& Matrix4::lookAt(Vector3& eye,Vector3& target,Vector3& up){
    _z->subVectors( eye, target );

    if (_z->lengthSq() == 0) {
        // eye and target are in the same position
        _z->z = 1;
    }

    _z->normalize();
    _x->crossVectors(up, *_z);

    if (_x->lengthSq() == 0) {
        // up and z are parallel
        if (abs(up.z) == 1) {
            _z->x += 0.0001;

        } else {
            _z->z += 0.0001;
        }

        _z->normalize();
        _x->crossVectors(up, *_z);

    }

    _x->normalize();
    _y->crossVectors(*_z, *_x);

    elements[ 0 ] = _x->x; elements[ 4 ] = _y->x; elements[ 8 ] = _z->x;
    elements[ 1 ] = _x->y; elements[ 5 ] = _y->y; elements[ 9 ] = _z->y;
    elements[ 2 ] = _x->z; elements[ 6 ] = _y->z; elements[ 10 ] = _z->z;

    return *this;
}

Matrix4& Matrix4::setPosition(Vector3& v) {
    elements[ 12 ] = v.x;
    elements[ 13 ] = v.y;
    elements[ 14 ] = v.z;

    return *this;
}

Matrix4& Matrix4::scale(Vector3& v) {
    const double x = v.x, y = v.y, z = v.z;

    elements[ 0 ] *= x; elements[ 4 ] *= y; elements[ 8 ] *= z;
    elements[ 1 ] *= x; elements[ 5 ] *= y; elements[ 9 ] *= z;
    elements[ 2 ] *= x; elements[ 6 ] *= y; elements[ 10 ] *= z;
    elements[ 3 ] *= x; elements[ 7 ] *= y; elements[ 11 ] *= z;

    return *this;
}

Matrix4& Matrix4::makeRotationAxis(Vector3& axis, double angle) {

		// Based on http://www.gamedev.net/reference/articles/article1199.asp
		const double c = cos(angle);
		const double s = sin(angle);
		const double t = 1 - c;
		const double x = axis.x, y = axis.y, z = axis.z;
		const double tx = t * x, ty = t * y;

		set(
			tx * x + c, tx * y - s * z, tx * z + s * y, 0,
			tx * y + s * z, ty * y + c, ty * z - s * x, 0,
			tx * z - s * y, ty * z + s * x, t * z * z + c, 0,
			0, 0, 0, 1
		);

		return *this;
	}
