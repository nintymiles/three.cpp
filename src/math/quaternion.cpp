#include "quaternion.h"

#include "Euler.h"
#include "Vector3.h"
#include "Matrix4.h"

Quaternion::Quaternion() : _x(0), _y(0), _z(0), _w(1) {}

Quaternion& Quaternion::setFromEuler(const Euler& euler, bool update) {

    float x = euler.getX(), y = euler.getY(), z = euler.getZ();
    Euler::RotationOrder order = euler.getOrder();

    // http://www.mathworks.com/matlabcentral/fileexchange/
    // 	20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/
    //	content/SpinCalc.m

    float c1 = cos(x / 2);
    float c2 = cos(y / 2);
    float c3 = cos(z / 2);

    float s1 = sin(x / 2);
    float s2 = sin(y / 2);
    float s3 = sin(z / 2);

    if (order == Euler::RotationOrder::XYZ) {

        _x = s1 * c2 * c3 + c1 * s2 * s3;
        _y = c1 * s2 * c3 - s1 * c2 * s3;
        _z = c1 * c2 * s3 + s1 * s2 * c3;
        _w = c1 * c2 * c3 - s1 * s2 * s3;

    }
    else if (order == Euler::RotationOrder::YXZ) {

        _x = s1 * c2 * c3 + c1 * s2 * s3;
        _y = c1 * s2 * c3 - s1 * c2 * s3;
        _z = c1 * c2 * s3 - s1 * s2 * c3;
        _w = c1 * c2 * c3 + s1 * s2 * s3;

    }
    else if (order == Euler::RotationOrder::ZXY) {

        _x = s1 * c2 * c3 - c1 * s2 * s3;
        _y = c1 * s2 * c3 + s1 * c2 * s3;
        _z = c1 * c2 * s3 + s1 * s2 * c3;
        _w = c1 * c2 * c3 - s1 * s2 * s3;

    }
    else if (order == Euler::RotationOrder::ZYX) {

        _x = s1 * c2 * c3 - c1 * s2 * s3;
        _y = c1 * s2 * c3 + s1 * c2 * s3;
        _z = c1 * c2 * s3 - s1 * s2 * c3;
        _w = c1 * c2 * c3 + s1 * s2 * s3;

    }
    else if (order == Euler::RotationOrder::YZX) {

        _x = s1 * c2 * c3 + c1 * s2 * s3;
        _y = c1 * s2 * c3 + s1 * c2 * s3;
        _z = c1 * c2 * s3 - s1 * s2 * c3;
        _w = c1 * c2 * c3 - s1 * s2 * s3;

    }
    else if (order == Euler::RotationOrder::XZY) {

        _x = s1 * c2 * c3 - c1 * s2 * s3;
        _y = c1 * s2 * c3 - s1 * c2 * s3;
        _z = c1 * c2 * s3 + s1 * s2 * c3;
        _w = c1 * c2 * c3 + s1 * s2 * s3;

    }

    if (update) onChange.emitSignal(*this);

    return *this;
}

Quaternion& Quaternion::setFromAxisAngle(const Vector3& axis, float angle) {

    float halfAngle = angle / 2, s = sin(halfAngle);

    _x = axis.x * s;
    _y = axis.y * s;
    _z = axis.z * s;
    _w = cos(halfAngle);

    onChange.emitSignal(*this);

    return *this;
}

Quaternion& Quaternion::setFromRotationMatrix(const Matrix4& m) {
    const float* te = m.elements;
    float m11 = te[0], m12 = te[4], m13 = te[8],
            m21 = te[1], m22 = te[5], m23 = te[9],
            m31 = te[2], m32 = te[6], m33 = te[10],
            trace = m11 + m22 + m33, s;

    if (trace > 0) {

        s = 0.5f / sqrt(trace + 1.0f);

        _w = 0.25f / s;
        _x = (m32 - m23) * s;
        _y = (m13 - m31) * s;
        _z = (m21 - m12) * s;

    }
    else if (m11 > m22 && m11 > m33) {

        s = 2.0f * sqrt(1.0f + m11 - m22 - m33);

        _w = (m32 - m23) / s;
        _x = 0.25f * s;
        _y = (m12 + m21) / s;
        _z = (m13 + m31) / s;

    }
    else if (m22 > m33) {

        s = 2.0f * sqrt(1.0f + m22 - m11 - m33);

        _w = (m13 - m31) / s;
        _x = (m12 + m21) / s;
        _y = 0.25f * s;
        _z = (m23 + m32) / s;

    }
    else {

        s = 2.0f * sqrt(1.0f + m33 - m11 - m22);

        _w = (m21 - m12) / s;
        _x = (m13 + m31) / s;
        _y = (m23 + m32) / s;
        _z = 0.25f * s;

    }

    onChange.emitSignal(*this);

    return *this;
}

Quaternion& Quaternion::setFromUnitVectors(const Vector3& vFrom, const Vector3& vTo) {

    float EPS = 0.000001f;

    float r = vFrom.dot(vTo) + 1;

    if (r < EPS) {

        r = 0;

        if (abs(vFrom.x) > abs(vFrom.z)) {

            _x = -vFrom.y;
            _y = vFrom.x;
            _z = 0;
            _w = r;

        }
        else {

            _x = 0;
            _y = -vFrom.z;
            _z = vFrom.y;
            _w = r;

        }

    }
    else {

        // crossVectors( vFrom, vTo ); // inlined to avoid cyclic dependency on Vector3

        _x = vFrom.y * vTo.z - vFrom.z * vTo.y;
        _y = vFrom.z * vTo.x - vFrom.x * vTo.z;
        _z = vFrom.x * vTo.y - vFrom.y * vTo.x;
        _w = r;

    }

    return this->normalize();
}

float Quaternion::angleTo(const Quaternion& q) {
    return 2 * acos(abs(math::clamp(dot(q), -1.0f, 1.0f)));
}

Quaternion& Quaternion::rotateTowards(const Quaternion& q, float step) {
    float angle = angleTo(q);
    if (angle == 0) return *this;

    float t = std::min(1.0f, step / angle);

    slerp(q, t);

    return *this;
}

bool Quaternion::equals(const Quaternion& quaternion) {
    return (quaternion._x == _x) && (quaternion._y == _y) && (quaternion._z == _z) && (quaternion._w == _w);

}

//#include "vector3.h"
//#include "euler.h"
//
//Quaternion& Quaternion::setFromAxisAngle(const Vector3& axis, double angle) {
//		// http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
//		// assumes axis is normalized
//		auto halfAngle = angle / 2;
//        auto s = sin(halfAngle);
//
//		_x = axis.x * s;
//		_y = axis.y * s;
//		_z = axis.z * s;
//		_w = cos(halfAngle);
//
//		if(onChangeCallback)
//            onChangeCallback(*this);
//
//		return *this;
//}
//
////if vForm is a const reference,then we can't call ordinary member function of Vector3.
//Quaternion& Quaternion::setFromUnitVectors(Vector3& vFrom,Vector3& vTo){
//		// assumes direction vectors vFrom and vTo are normalized
//
//		double r = vFrom.dot(vTo) + 1;
//
//		if ( r < std::numeric_limits<double>::epsilon() ) {
//			// vFrom and vTo point in opposite directions
//			r = 0;
//
//			if ( abs(vFrom.x) > abs(vFrom.z) ) {
//				_x = - vFrom.y;
//				_y = vFrom.x;
//				_z = 0;
//				_w = r;
//
//			} else {
//				_x = 0;
//				_y = - vFrom.z;
//				_z = vFrom.y;
//				_w = r;
//			}
//
//		} else {
//			// crossVectors( vFrom, vTo ); // inlined to avoid cyclic dependency on Vector3
//			_x = vFrom.y * vTo.z - vFrom.z * vTo.y;
//			_y = vFrom.z * vTo.x - vFrom.x * vTo.z;
//			_z = vFrom.x * vTo.y - vFrom.y * vTo.x;
//			_w = r;
//		}
//
//		return this->normalize();
//
//}
//
//Quaternion& Quaternion::setFromEuler(const Euler& euler, bool update) {
//	const double x = euler.x(), y = euler.y(), z = euler.z();
//	const RotationOrder order = euler.order();
//	// http://www.mathworks.com/matlabcentral/fileexchange/
//	// 	20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/
//	//	content/SpinCalc.m
//
//	const double c1 = cos( x / 2 );
//	const double c2 = cos( y / 2 );
//	const double c3 = cos( z / 2 );
//
//	const double s1 = sin( x / 2 );
//	const double s2 = sin( y / 2 );
//	const double s3 = sin( z / 2 );
//
//	switch ( order ) {
//
//		case XYZ:
//			_x = s1 * c2 * c3 + c1 * s2 * s3;
//			_y = c1 * s2 * c3 - s1 * c2 * s3;
//			_z = c1 * c2 * s3 + s1 * s2 * c3;
//			_w = c1 * c2 * c3 - s1 * s2 * s3;
//			break;
//
//		case YXZ:
//			_x = s1 * c2 * c3 + c1 * s2 * s3;
//			_y = c1 * s2 * c3 - s1 * c2 * s3;
//			_z = c1 * c2 * s3 - s1 * s2 * c3;
//			_w = c1 * c2 * c3 + s1 * s2 * s3;
//			break;
//
//		case ZXY:
//			_x = s1 * c2 * c3 - c1 * s2 * s3;
//			_y = c1 * s2 * c3 + s1 * c2 * s3;
//			_z = c1 * c2 * s3 + s1 * s2 * c3;
//			_w = c1 * c2 * c3 - s1 * s2 * s3;
//			break;
//
//		case ZYX:
//			_x = s1 * c2 * c3 - c1 * s2 * s3;
//			_y = c1 * s2 * c3 + s1 * c2 * s3;
//			_z = c1 * c2 * s3 - s1 * s2 * c3;
//			_w = c1 * c2 * c3 + s1 * s2 * s3;
//			break;
//
//		case YZX:
//			_x = s1 * c2 * c3 + c1 * s2 * s3;
//			_y = c1 * s2 * c3 + s1 * c2 * s3;
//			_z = c1 * c2 * s3 - s1 * s2 * c3;
//			_w = c1 * c2 * c3 - s1 * s2 * s3;
//			break;
//
//		case XZY:
//			_x = s1 * c2 * c3 - c1 * s2 * s3;
//			_y = c1 * s2 * c3 - s1 * c2 * s3;
//			_z = c1 * c2 * s3 + s1 * s2 * c3;
//			_w = c1 * c2 * c3 + s1 * s2 * s3;
//			break;
//
//		default:
//			//console.warn( 'THREE.Quaternion: .setFromEuler() encountered an unknown order: ' + order );
//			;
//
//	}
//
//	if ( update != false )
//        if(onChangeCallback)
//            onChangeCallback(*this);
//
//	return *this;
//}