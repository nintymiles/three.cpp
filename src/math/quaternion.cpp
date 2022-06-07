#include "quaternion.h"
#include "vector3.h"

Quaternion& Quaternion::setFromAxisAngle(const Vector3& axis, double angle) {
		// http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
		// assumes axis is normalized
		auto halfAngle = angle / 2;
        auto s = sin(halfAngle);

		_x = axis.x * s;
		_y = axis.y * s;
		_z = axis.z * s;
		_w = cos(halfAngle);

		onChangeCallback();

		return *this;
}

//if vForm is a const reference,then we can't call ordinary member function of Vector3.
Quaternion& Quaternion::setFromUnitVectors(Vector3& vFrom,Vector3& vTo){
		// assumes direction vectors vFrom and vTo are normalized

		double r = vFrom.dot(vTo) + 1;

		if ( r < std::numeric_limits<double>::epsilon() ) {
			// vFrom and vTo point in opposite directions
			r = 0;

			if ( abs(vFrom.x) > abs(vFrom.z) ) {
				_x = - vFrom.y;
				_y = vFrom.x;
				_z = 0;
				_w = r;

			} else {
				_x = 0;
				_y = - vFrom.z;
				_z = vFrom.y;
				_w = r;
			}

		} else {
			// crossVectors( vFrom, vTo ); // inlined to avoid cyclic dependency on Vector3
			_x = vFrom.y * vTo.z - vFrom.z * vTo.y;
			_y = vFrom.z * vTo.x - vFrom.x * vTo.z;
			_z = vFrom.x * vTo.y - vFrom.y * vTo.x;
			_w = r;
		}

		return this->normalize();

}

Euler& Quaternion::setFromVector3(const Vector3& v,euler_order order) {
            return set( v.x, v.y, v.z, order );
        }