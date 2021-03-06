#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector3.h"
#include "matrix4.h"
#include "euler.h"
#include "math_utils.h"
#include "number.h"

#include <cmath>
#include <random>
#include <limits>
#include <functional>

//类模版在对象交叉引用时的前置声明
// template <typename T> class Vector3;
// 使用时如模版的实例化声明： Vecotr3<double> v;
// typedef Vector3<double> Vector3d;
// 若类模版中有别名定义，则前置声明时此别名也需要在前置声明后重新定义
class Euler;

class Quaternion{

public:
    Quaternion(double x=0,double y=0,double z=0,double w=1):_x(x),_y(y),_z(z),_w(w){};
    //~Quaternion();



    auto x()       -> double&  { return _x; }
    auto x() const -> const double& { return _x;}
    Quaternion& setX(double x){
        _x = x;
        onChangeCallback();
        return *this;
    }

    double& y(){return _y;}
    const double& y() const{return _y;}
    Quaternion& setY(double y){
        _y = y;
        onChangeCallback();
        return *this;
    }

    double& z(){return _z;}
    const double& z() const{return _z;}
    Quaternion& setZ(double z){
        _z = z;
        onChangeCallback();
        return *this;
    }

    double& w(){return _w;}
    const double& w() const{return _w;}
    Quaternion& setW(double w){
        _w = w;
        onChangeCallback();
        return *this;
    }

    Quaternion& set(double x,double y,double z,double w){
        _x = x;
        _y = y;
        _z = z;
        _w = w;
        onChangeCallback();
        return *this;
    }

    Quaternion clone(){
		return Quaternion(_x, _y, _z, _w);
	}

    Quaternion& copy(Quaternion& quaternion) {
		_x = quaternion.x();
		_y = quaternion.y();
		_z = quaternion.z();
		_w = quaternion.w();

		onChangeCallback();

		return *this;
	}

	Quaternion& setFromEuler(Euler& euler, bool update);
	
	Quaternion& setFromAxisAngle(Vector3& axis, double angle);

	Quaternion& setFromRotationMatrix(Matrix4& m){
		// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
		// assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

		const double
			m11 = m.elements[ 0 ], m12 = m.elements[ 4 ], m13 = m.elements[ 8 ],
			m21 = m.elements[ 1 ], m22 = m.elements[ 5 ], m23 = m.elements[ 9 ],
			m31 = m.elements[ 2 ], m32 = m.elements[ 6 ], m33 = m.elements[ 10 ],

			trace = m11 + m22 + m33;

		if (trace > 0) {
			const double s = 0.5 / sqrt( trace + 1.0 );

			_w = 0.25 / s;
			_x = ( m32 - m23 ) * s;
			_y = ( m13 - m31 ) * s;
			_z = ( m21 - m12 ) * s;

		} else if (m11 > m22 && m11 > m33) {
			const double s = 2.0 * sqrt(1.0 + m11 - m22 - m33);

			_w = ( m32 - m23 ) / s;
			_x = 0.25 * s;
			_y = ( m12 + m21 ) / s;
			_z = ( m13 + m31 ) / s;
		} else if ( m22 > m33 ) {
			const double s = 2.0 * sqrt(1.0 + m22 - m11 - m33);

			_w = ( m13 - m31 ) / s;
			_x = ( m12 + m21 ) / s;
			_y = 0.25 * s;
			_z = ( m23 + m32 ) / s;
		} else {
			const double s = 2.0 * sqrt(1.0 + m33 - m11 - m22);

			_w = ( m21 - m12 ) / s;
			_x = ( m13 + m31 ) / s;
			_y = ( m23 + m32 ) / s;
			_z = 0.25 * s;
		}

		onChangeCallback();

		return *this;
	}

	Quaternion& setFromUnitVectors(Vector3& vFrom,Vector3& vTo);

	double angleTo(Quaternion& q) {
		return 2 * acos(abs(clamp<double>(dot(q), - 1, 1)));
	}

    //step理解为步距？一个step为角度的倒数
	Quaternion& rotateTowards(Quaternion q, double step) {
		const double angle = angleTo(q);

		if (angle == 0) return *this;

		const double t = fmin(1, step/angle);

		slerp(q, t);

		return *this;
	}

    //why change ownself into identity？
    Quaternion&	identity() {
		return set( 0, 0, 0, 1 );
	}

    Quaternion& invert(){
		// quaternion is assumed to have unit length
		return conjugate();
	}

	Quaternion& conjugate() {
		_x *= - 1;
		_y *= - 1;
		_z *= - 1;

		onChangeCallback();

		return *this;
	}

    //dot product
	double dot(Quaternion& v) {
		return _x * v._x + _y * v._y + _z * v._z + _w * v._w;
	}

    //Square length
	double lengthSq() {
		return _x * _x + _y * _y + _z * _z + _w * _w;
	}

    //length
	double length() {
		return sqrt( _x * _x + _y * _y + _z * _z + _w * _w );
	}

    Quaternion& normalize(){
		double l = this->length();

		if (l == 0) {

			_x = 0;
			_y = 0;
			_z = 0;
			_w = 1;

		} else {

			l = 1 / l;

			_x = _x * l;
			_y = _y * l;
			_z = _z * l;
			_w = _w * l;

		}

		onChangeCallback();

		return *this;
	}

    Quaternion& multiply(Quaternion& q) {
		return multiplyQuaternions( *this, q );
	}

	Quaternion& premultiply(Quaternion& q) {
		return multiplyQuaternions( q, *this );
	}

    //CPP中直接重载operator*即可
	Quaternion& multiplyQuaternions(Quaternion& a,Quaternion& b) {

		// from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/code/index.htm
		const double qax = a._x, qay = a._y, qaz = a._z, qaw = a._w;
		const double qbx = b._x, qby = b._y, qbz = b._z, qbw = b._w;

		_x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
		_y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
		_z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
		_w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

		onChangeCallback();

		return *this;
	}



    Quaternion& slerp(Quaternion& qb, double t) {
		if ( t == 0 ) return *this;
		if ( t == 1 ) return this->copy(qb);

		const double x = _x, y = _y, z = _z, w = _w;

		// http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/
		double cosHalfTheta = w * qb._w + x * qb._x + y * qb._y + z * qb._z;

		if ( cosHalfTheta < 0 ) {

			_w = - qb._w;
			_x = - qb._x;
			_y = - qb._y;
			_z = - qb._z;

			cosHalfTheta = - cosHalfTheta;

		} else {

			copy(qb);

		}

		if ( cosHalfTheta >= 1.0 ) {

			_w = w;
			_x = x;
			_y = y;
			_z = z;

			return *this;

		}

		const double sqrSinHalfTheta = 1.0 - cosHalfTheta * cosHalfTheta;

		if ( sqrSinHalfTheta <= Number::EPSILON ) {

			const double s = 1 - t;
			_w = s * w + t * _w;
			_x = s * x + t * _x;
			_y = s * y + t * _y;
			_z = s * z + t * _z;

			normalize();
			onChangeCallback();

			return *this;
		}

		const double sinHalfTheta = sqrt( sqrSinHalfTheta );
		const double halfTheta = atan2( sinHalfTheta, cosHalfTheta );
		const double ratioA = sin( ( 1 - t ) * halfTheta ) / sinHalfTheta,
			ratioB = sin( t * halfTheta ) / sinHalfTheta;

		_w = ( w * ratioA + _w * ratioB );
		_x = ( x * ratioA + _x * ratioB );
		_y = ( y * ratioA + _y * ratioB );
		_z = ( z * ratioA + _z * ratioB );

		onChangeCallback();

		return *this;
	}

	Quaternion& slerpQuaternions(Quaternion& qa,Quaternion& qb,double t) {
		return copy( qa ).slerp( qb, t );
	}

	Quaternion& random() {
		// Derived from http://planning.cs.uiuc.edu/node198.html
		// Note, this source uses w, x, y, z ordering,
		// so we swap the order below.

        // CPP random generator mechanism
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(0,1);
		const double u1 = distribution(generator); 
		const double sqrt1u1 = sqrt( 1 - u1 );
		const double sqrtu1 = sqrt( u1 );

		const double u2 = 2 * M_PI * distribution(generator);

		const double u3 = 2 * M_PI * distribution(generator);

		return set(
			sqrt1u1 * cos( u2 ),
			sqrtu1 * sin( u3 ),
			sqrtu1 * cos( u3 ),
			sqrt1u1 * sin( u2 )
		);
	}

	bool equals(Quaternion& quaternion) {
		return ( quaternion._x == _x ) && ( quaternion._y == _y ) && ( quaternion._z == _z ) && ( quaternion._w == _w );
	}

    //create a quaternion from an array
    Quaternion&	fromArray(double array[], int offset = 0) {
		_x = array[ offset ];
		_y = array[ offset + 1 ];
		_z = array[ offset + 2 ];
		_w = array[ offset + 3 ];

		onChangeCallback();

		return *this;
	}

    //from a quaternion to an array
	void toArray(double array[], int offset = 0) {

		array[ offset ] = _x;
		array[ offset + 1 ] = _y;
		array[ offset + 2 ] = _z;
		array[ offset + 3 ] = _w;

		//return array;
	}

// 	fromBufferAttribute( attribute, index ) {

// 		this._x = attribute.getX( index );
// 		this._y = attribute.getY( index );
// 		this._z = attribute.getZ( index );
// 		this._w = attribute.getW( index );

// 		return this;

// 	}

	//function pointer's definition void(*callback)(void) which already includes declaration's everything 
	Quaternion& onChange(void(*callback)(void)){
		onChangeCallback = callback;
		return *this;
	}

private:
    double _x,_y,_z,_w;
	void(*onChangeCallback)(void);
	//std::function<void(void)> onChangeCallback;
};


// 	static slerp( qa, qb, qm, t ) {
// 		console.warn( 'THREE.Quaternion: Static .slerp() has been deprecated. Use qm.slerpQuaternions( qa, qb, t ) instead.' );
// 		return qm.slerpQuaternions( qa, qb, t );
// 	}
static void slerpFlat(double dst[],int dstOffset,double src0[],int srcOffset0,double src1[],int srcOffset1,double t){
	// fuzz-free, array-based Quaternion SLERP operation
	double
		x0 = src0[ srcOffset0 + 0 ],
		y0 = src0[ srcOffset0 + 1 ],
		z0 = src0[ srcOffset0 + 2 ],
		w0 = src0[ srcOffset0 + 3 ];

	const double
		x1 = src1[ srcOffset1 + 0 ],
		y1 = src1[ srcOffset1 + 1 ],
		z1 = src1[ srcOffset1 + 2 ],
		w1 = src1[ srcOffset1 + 3 ];

	if (t == 0){
		dst[ dstOffset + 0 ] = x0;
		dst[ dstOffset + 1 ] = y0;
		dst[ dstOffset + 2 ] = z0;
		dst[ dstOffset + 3 ] = w0;
		return;
	}

	if (t == 1) {
		dst[ dstOffset + 0 ] = x1;
		dst[ dstOffset + 1 ] = y1;
		dst[ dstOffset + 2 ] = z1;
		dst[ dstOffset + 3 ] = w1;
		return;
	}

	if ( w0 != w1 || x0 != x1 || y0 != y1 || z0 != z1 ) {
		double s = 1 - t;
		const double cos = x0 * x1 + y0 * y1 + z0 * z1 + w0 * w1,
					dir = ( cos >= 0 ? 1 : - 1 ),
					sqrSin = 1 - cos * cos;

		// Skip the Slerp for tiny steps to avoid numeric problems:
		if (sqrSin > Number::EPSILON) {
			const double sin = sqrt( sqrSin ),
						len = atan2( sin, cos * dir );

			s = std::sin( s * len ) / sin;
			t = std::sin( t * len ) / sin;
		}

		const double tDir = t * dir;

		x0 = x0 * s + x1 * tDir;
		y0 = y0 * s + y1 * tDir;
		z0 = z0 * s + z1 * tDir;
		w0 = w0 * s + w1 * tDir;

		// Normalize in case we just did a lerp:
		if ( s == 1 - t ) {
			const double f = 1 / sqrt( x0 * x0 + y0 * y0 + z0 * z0 + w0 * w0 );

			x0 *= f;
			y0 *= f;
			z0 *= f;
			w0 *= f;
		}

	}

	dst[ dstOffset ] = x0;
	dst[ dstOffset + 1 ] = y0;
	dst[ dstOffset + 2 ] = z0;
	dst[ dstOffset + 3 ] = w0;
}

static void multiplyQuaternionsFlat(double dst[],int dstOffset,double src0[],int srcOffset0,double src1[],int srcOffset1) {

	const double x0 = src0[ srcOffset0 ];
	const double y0 = src0[ srcOffset0 + 1 ];
	const double z0 = src0[ srcOffset0 + 2 ];
	const double w0 = src0[ srcOffset0 + 3 ];

	const double x1 = src1[ srcOffset1 ];
	const double y1 = src1[ srcOffset1 + 1 ];
	const double z1 = src1[ srcOffset1 + 2 ];
	const double w1 = src1[ srcOffset1 + 3 ];

	dst[ dstOffset ] = x0 * w1 + w0 * x1 + y0 * z1 - z0 * y1;
	dst[ dstOffset + 1 ] = y0 * w1 + w0 * y1 + z0 * x1 - x0 * z1;
	dst[ dstOffset + 2 ] = z0 * w1 + w0 * z1 + x0 * y1 - y0 * x1;
	dst[ dstOffset + 3 ] = w0 * w1 - x0 * x1 - y0 * y1 - z0 * z1;
}

#endif //QUATERNION_H