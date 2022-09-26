#include "quaternion.h"
#include "vector3.h"
#include "euler.h"

Quaternion& Quaternion::setFromAxisAngle(Vector3& axis, double angle) {
		// http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
		// assumes axis is normalized
		auto halfAngle = angle / 2;
        auto s = sin(halfAngle);

		_x = axis.x * s;
		_y = axis.y * s;
		_z = axis.z * s;
		_w = cos(halfAngle);

		if(onChangeCallback)
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

Quaternion& Quaternion::setFromEuler(Euler& euler, bool update) {
	const double x = euler.x(), y = euler.y(), z = euler.z();
	const euler_order order = euler.order();
	// http://www.mathworks.com/matlabcentral/fileexchange/
	// 	20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/
	//	content/SpinCalc.m

	const double c1 = cos( x / 2 );
	const double c2 = cos( y / 2 );
	const double c3 = cos( z / 2 );

	const double s1 = sin( x / 2 );
	const double s2 = sin( y / 2 );
	const double s3 = sin( z / 2 );

	switch ( order ) {

		case XYZ:
			_x = s1 * c2 * c3 + c1 * s2 * s3;
			_y = c1 * s2 * c3 - s1 * c2 * s3;
			_z = c1 * c2 * s3 + s1 * s2 * c3;
			_w = c1 * c2 * c3 - s1 * s2 * s3;
			break;

		case YXZ:
			_x = s1 * c2 * c3 + c1 * s2 * s3;
			_y = c1 * s2 * c3 - s1 * c2 * s3;
			_z = c1 * c2 * s3 - s1 * s2 * c3;
			_w = c1 * c2 * c3 + s1 * s2 * s3;
			break;

		case ZXY:
			_x = s1 * c2 * c3 - c1 * s2 * s3;
			_y = c1 * s2 * c3 + s1 * c2 * s3;
			_z = c1 * c2 * s3 + s1 * s2 * c3;
			_w = c1 * c2 * c3 - s1 * s2 * s3;
			break;

		case ZYX:
			_x = s1 * c2 * c3 - c1 * s2 * s3;
			_y = c1 * s2 * c3 + s1 * c2 * s3;
			_z = c1 * c2 * s3 - s1 * s2 * c3;
			_w = c1 * c2 * c3 + s1 * s2 * s3;
			break;

		case YZX:
			_x = s1 * c2 * c3 + c1 * s2 * s3;
			_y = c1 * s2 * c3 + s1 * c2 * s3;
			_z = c1 * c2 * s3 - s1 * s2 * c3;
			_w = c1 * c2 * c3 - s1 * s2 * s3;
			break;

		case XZY:
			_x = s1 * c2 * c3 - c1 * s2 * s3;
			_y = c1 * s2 * c3 - s1 * c2 * s3;
			_z = c1 * c2 * s3 + s1 * s2 * c3;
			_w = c1 * c2 * c3 + s1 * s2 * s3;
			break;

		default:
			//console.warn( 'THREE.Quaternion: .setFromEuler() encountered an unknown order: ' + order );
			;

	}

	if ( update != false )
        if(onChangeCallback)
            onChangeCallback();

	return *this;
}