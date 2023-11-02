#ifndef QUATERNION_H
#define QUATERNION_H

#include <cassert>
#include "osdecl.h"
#include "simplesignal.h"
#include "math_utils.h"

class Euler;

class Vector3;

class Matrix4;

class Quaternion
{
    union {
        struct {
            float _x, _y, _z, _w;
        };
        float elements[4];
    };

public:

    threecpp::Signal<void(const Quaternion&)> onChange;

    Quaternion();

    Quaternion(float x, float y, float z) :_x(x), _y(y), _z(z), _w(1) {}

    Quaternion(float x, float y, float z, float w) :_x(x), _y(y), _z(z), _w(w) {}

    Quaternion(float scalar) :_x(scalar), _y(scalar), _z(scalar), _w(scalar) {}

    Quaternion(const Quaternion& q) : _x(q._x), _y(q._y), _z(q._z), _w(q._w) {}

    Quaternion& operator = (const Quaternion& q) {
        return set(q, true);
    }

    Quaternion& set(const Quaternion& q, bool emitSignal) {
        _x = q._x;
        _y = q._y;
        _z = q._z;
        _w = q._w;

        if (emitSignal) onChange.emitSignal(*this);

        return *this;
    }
    Quaternion& clone() {
        return *this;
    }
    Quaternion& set(float x, float y, float z) {
        _x = x;
        _y = y;
        _z = z;
        _w = 1;

        return *this;
    }

    Quaternion& set(float x, float y, float z,float w) {
        _x = x;
        _y = y;
        _z = z;
        _w = w;

        return *this;
    }
    Quaternion& copy(const Quaternion& q) {
        _x = q._x;
        _y = q._y;
        _z = q._z;
        _w = q._w;

        return *this;
    }
    float getX() const { return _x; }
    float getY() const { return _y; }
    float getZ() const { return _z; }
    float getW() const { return _w; }

    void setX(float value) {
        _x = value;
        onChange.emitSignal(*this);
    }
    void setY(float value) {
        _y = value;
        onChange.emitSignal(*this);
    }
    void setZ(float value) {
        _z = value;
        onChange.emitSignal(*this);
    }
    void setW(float value) {
        _w = value;
        onChange.emitSignal(*this);
    }

    Quaternion& setFromEuler(const Euler& euler, bool update = false);

    Quaternion& setFromAxisAngle(const Vector3& axis, float angle);

    Quaternion& setFromRotationMatrix(const Matrix4& m);

    Quaternion& setFromUnitVectors(const Vector3& vFrom, const Vector3& vTo);

    float angleTo(const Quaternion& q);

    Quaternion& rotateTowards(const Quaternion& q, float step);

    Quaternion& inverse() {
        return conjugate();
    }

    Quaternion& conjugate() {
        _x *= -1;
        _y *= -1;
        _z *= -1;

        onChange.emitSignal(*this);

        return *this;
    }

    float dot(const Quaternion& v) const {
        return _x * v._x + _y * v._y + _z * v._z + _w * v._w;
    }

    float lengthSq() const {
        return _x * _x + _y * _y + _z * _z + _w * _w;
    }

    float length() const {
        return sqrt(_x * _x + _y * _y + _z * _z + _w * _w);
    }

    Quaternion& normalize() {
        float l = length();
        if (l == 0) {
            _x = 0;
            _y = 0;
            _z = 0;
            _w = 1;
        }
        else {
            l = 1 / l;
            _x = _x * l;
            _y = _y * l;
            _z = _z * l;
            _w = _w * l;

        }
        return *this;
    }

    Quaternion& multiply(const Quaternion& q) {
        return multiplyQuaternions(*this, q);
    }

    Quaternion& premultiply(const Quaternion& q) {
        return multiplyQuaternions(q, *this);
    }
    Quaternion& multiplyQuaternions(const Quaternion& a, const Quaternion& b) {
        float qax = a._x, qay = a._y, qaz = a._z, qaw = a._w,
                qbx = b._x, qby = b._y, qbz = b._z, qbw = b._w;

        _x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
        _y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
        _z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
        _w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

        onChange.emitSignal(*this);

        return *this;
    }
    bool equals(const Quaternion& quaternion);

    Quaternion& fromArray(const float* array, unsigned offset = 0) {
        _x = array[offset];
        _y = array[offset + 1];
        _z = array[offset + 2];
        _w = array[offset + 3];

        return *this;
    }

    float* toArray(float* array, unsigned offset = 0) {
        float a[4];
        if (array == NULL) array = a;

        array[offset] = _x;
        array[offset + 1] = _y;
        array[offset + 2] = _z;
        array[offset + 3] = _w;

        return array;
    }

    Quaternion& slerp(const Quaternion& qb, float t) {
        if (t == 0) return *this;
        if (t == 1) return copy(qb);

        float x = _x, y = _y, z = _z, w = _w;

        // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/

        float cosHalfTheta = w * qb._w + x * qb._x + y * qb._y + z * qb._z;

        if (cosHalfTheta < 0) {

            _w = -qb._w;
            _x = -qb._x;
            _y = -qb._y;
            _z = -qb._z;

            cosHalfTheta = -cosHalfTheta;

        }
        else {

            copy(qb);

        }

        if (cosHalfTheta >= 1.0f) {

            _w = w;
            _x = x;
            _y = y;
            _z = z;

            return *this;

        }

        float sqrSinHalfTheta = 1.0f - cosHalfTheta * cosHalfTheta;

        if (sqrSinHalfTheta <= std::numeric_limits<float>::epsilon()) {

            float s = 1 - t;
            _w = s * w + t * _w;
            _x = s * x + t * _x;
            _y = s * y + t * _y;
            _z = s * z + t * _z;

            normalize();
            onChange.emitSignal(*this);

            return *this;

        }

        float sinHalfTheta = sqrt(sqrSinHalfTheta);
        float halfTheta = atan2(sinHalfTheta, cosHalfTheta);
        float ratioA = sin((1 - t) * halfTheta) / sinHalfTheta,
                ratioB = sin(t * halfTheta) / sinHalfTheta;

        _w = (w * ratioA + _w * ratioB);
        _x = (x * ratioA + _x * ratioB);
        _y = (y * ratioA + _y * ratioB);
        _z = (z * ratioA + _z * ratioB);

        onChange.emitSignal(*this);

        return *this;
    }

    Quaternion& slerpFlat(float* dst, unsigned dstOffset, const float* src0, unsigned srcOffset0, const float* src1, unsigned srcOffset1, float t) {

        float x0 = src0[srcOffset0 + 0], y0 = src0[srcOffset0 + 1], z0 = src0[srcOffset0 + 2], w0 = src0[srcOffset0 + 3],
                x1 = src1[srcOffset1 + 0], y1 = src1[srcOffset1 + 1], z1 = src1[srcOffset1 + 2], w1 = src1[srcOffset1 + 3];

        if (w0 != w1 || x0 != x1 || y0 != y1 || z0 != z1) {

            float s = 1 - t,
                    _cos = x0 * x1 + y0 * y1 + z0 * z1 + w0 * w1,
                    sqrSin = 1 - _cos * _cos;

            int dir = (_cos >= 0 ? 1 : -1);


            // Skip the Slerp for tiny steps to avoid numeric problems:
            if (sqrSin > std::numeric_limits<float>::epsilon()) {

                float _sin = sqrt(sqrSin),
                        len = atan2(_sin, _cos * dir);

                s = sin(s * len) / _sin;
                t = sin(t * len) / _sin;

            }

            float tDir = t * dir;

            x0 = x0 * s + x1 * tDir;
            y0 = y0 * s + y1 * tDir;
            z0 = z0 * s + z1 * tDir;
            w0 = w0 * s + w1 * tDir;

            // Normalize in case we just did a lerp:
            if (s == 1 - t) {

                float f = 1 / sqrt(x0 * x0 + y0 * y0 + z0 * z0 + w0 * w0);

                x0 *= f;
                y0 *= f;
                z0 *= f;
                w0 *= f;

            }

        }

        dst[dstOffset] = x0;
        dst[dstOffset + 1] = y0;
        dst[dstOffset + 2] = z0;
        dst[dstOffset + 3] = w0;

        return *this;
    }

    Quaternion& operator *=(const Quaternion& q) {
        return multiplyQuaternions(*this, q);
    }
};

inline Quaternion operator *(const Quaternion& q1, const Quaternion& q2) {
    Quaternion result(q1);

    result *= q2;

    return result;
}

//#include "vector3.h"
//#include "matrix4.h"
//#include "math_utils.h"
//#include "number.h"
//#include "simplesignal.h"
//
//#include <cmath>
//#include <random>
//#include <limits>
//#include <functional>
//
////类模版在对象交叉引用时的前置声明
//// template <typename T> class Vector3;
//// 使用时如模版的实例化声明： Vecotr3<double> v;
//// typedef Vector3<double> Vector3d;
//// 若类模版中有别名定义，则前置声明时此别名也需要在前置声明后重新定义
//class Euler;
//
//class Quaternion;
//typedef void(*OnChangeCallbackType)(const Quaternion&);
//
//class Quaternion{
//
//public:
//    Quaternion(double x=0,double y=0,double z=0,double w=1):_x(x),_y(y),_z(z),_w(w){};
//    //~Quaternion();
//
//
//
//    auto x()       -> double&  { return _x; }
//    auto x() const -> const double& { return _x;}
//    Quaternion& setX(double x){
//        _x = x;
//        if(onChangeCallback)
//            onChangeCallback(*this);
//        return *this;
//    }
//
//    double& y(){return _y;}
//    const double& y() const{return _y;}
//    Quaternion& setY(double y){
//        _y = y;
//        if(onChangeCallback)
//            onChangeCallback(*this);
//        return *this;
//    }
//
//    double& z(){return _z;}
//    const double& z() const{return _z;}
//    Quaternion& setZ(double z){
//        _z = z;
//        if(onChangeCallback)
//            onChangeCallback(*this);
//        return *this;
//    }
//
//    double& w(){return _w;}
//    const double& w() const{return _w;}
//    Quaternion& setW(double w){
//        _w = w;
//        if(onChangeCallback)
//            onChangeCallback(*this);
//        return *this;
//    }
//
//    Quaternion& set(double x,double y,double z,double w){
//        _x = x;
//        _y = y;
//        _z = z;
//        _w = w;
//        //cpp回调函数判空
//        if(onChangeCallback)
//            onChangeCallback(*this);
//        return *this;
//    }
//
//    Quaternion clone(){
//		return Quaternion(_x, _y, _z, _w);
//	}
//
//    Quaternion& copy(const Quaternion& quaternion) {
//		_x = quaternion.x();
//		_y = quaternion.y();
//		_z = quaternion.z();
//		_w = quaternion.w();
//
//		if(onChangeCallback)
//            onChangeCallback(*this);
//
//		return *this;
//	}
//
//	Quaternion& setFromEuler(const Euler& euler, bool update);
//
//	Quaternion& setFromAxisAngle(const Vector3& axis, double angle);
//
//	Quaternion& setFromRotationMatrix(Matrix4& m){
//		// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
//		// assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)
//
//		const double
//			m11 = m.elements[ 0 ], m12 = m.elements[ 4 ], m13 = m.elements[ 8 ],
//			m21 = m.elements[ 1 ], m22 = m.elements[ 5 ], m23 = m.elements[ 9 ],
//			m31 = m.elements[ 2 ], m32 = m.elements[ 6 ], m33 = m.elements[ 10 ],
//
//			trace = m11 + m22 + m33;
//
//		if (trace > 0) {
//			const double s = 0.5 / sqrt( trace + 1.0 );
//
//			_w = 0.25 / s;
//			_x = ( m32 - m23 ) * s;
//			_y = ( m13 - m31 ) * s;
//			_z = ( m21 - m12 ) * s;
//
//		} else if (m11 > m22 && m11 > m33) {
//			const double s = 2.0 * sqrt(1.0 + m11 - m22 - m33);
//
//			_w = ( m32 - m23 ) / s;
//			_x = 0.25 * s;
//			_y = ( m12 + m21 ) / s;
//			_z = ( m13 + m31 ) / s;
//		} else if ( m22 > m33 ) {
//			const double s = 2.0 * sqrt(1.0 + m22 - m11 - m33);
//
//			_w = ( m13 - m31 ) / s;
//			_x = ( m12 + m21 ) / s;
//			_y = 0.25 * s;
//			_z = ( m23 + m32 ) / s;
//		} else {
//			const double s = 2.0 * sqrt(1.0 + m33 - m11 - m22);
//
//			_w = ( m21 - m12 ) / s;
//			_x = ( m13 + m31 ) / s;
//			_y = ( m23 + m32 ) / s;
//			_z = 0.25 * s;
//		}
//
//		if(onChangeCallback)
//            onChangeCallback(*this);
//
//		return *this;
//	}
//
//	Quaternion& setFromUnitVectors(Vector3& vFrom,Vector3& vTo);
//
//	double angleTo(Quaternion& q) {
//		return 2 * acos(abs(MathUtils::clamp<double>(dot(q), - 1, 1)));
//	}
//
//    //step理解为步距？一个step为角度的倒数
//	Quaternion& rotateTowards(Quaternion q, double step) {
//		const double angle = angleTo(q);
//
//		if (angle == 0) return *this;
//
//		const double t = fmin(1, step/angle);
//
//		slerp(q, t);
//
//		return *this;
//	}
//
//    //why change ownself into identity？
//    Quaternion&	identity() {
//		return set( 0, 0, 0, 1 );
//	}
//
//    Quaternion& invert(){
//		// quaternion is assumed to have unit length
//		return conjugate();
//	}
//
//	Quaternion& conjugate() {
//		_x *= - 1;
//		_y *= - 1;
//		_z *= - 1;
//
//		if(onChangeCallback)
//            onChangeCallback(*this);
//
//		return *this;
//	}
//
//    //dot product
//	double dot(Quaternion& v) {
//		return _x * v.x() + _y * v.y() + _z * v.z() + _w * v.w();
//	}
//
//    //Square length
//	double lengthSq() {
//		return _x * _x + _y * _y + _z * _z + _w * _w;
//	}
//
//    //length
//	double length() {
//		return sqrt( _x * _x + _y * _y + _z * _z + _w * _w );
//	}
//
//    Quaternion& normalize(){
//		double l = this->length();
//
//		if (l == 0) {
//
//			_x = 0;
//			_y = 0;
//			_z = 0;
//			_w = 1;
//
//		} else {
//
//			l = 1 / l;
//
//			_x = _x * l;
//			_y = _y * l;
//			_z = _z * l;
//			_w = _w * l;
//
//		}
//
//		if(onChangeCallback)
//            onChangeCallback(*this);
//
//		return *this;
//	}
//
//    Quaternion& multiply(Quaternion& q) {
//		return multiplyQuaternions( *this, q );
//	}
//
//	Quaternion& premultiply(Quaternion& q) {
//		return multiplyQuaternions( q, *this );
//	}
//
//    //CPP中直接重载operator*即可
//	Quaternion& multiplyQuaternions(Quaternion& a,Quaternion& b) {
//
//		// from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/code/index.htm
//		const double qax = a._x, qay = a._y, qaz = a._z, qaw = a._w;
//		const double qbx = b._x, qby = b._y, qbz = b._z, qbw = b._w;
//
//		_x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
//		_y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
//		_z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
//		_w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;
//
//		if(onChangeCallback)
//            onChangeCallback(*this);
//
//		return *this;
//	}
//
//
//
//    Quaternion& slerp(Quaternion& qb, double t) {
//		if ( t == 0 ) return *this;
//		if ( t == 1 ) return this->copy(qb);
//
//		const double x = _x, y = _y, z = _z, w = _w;
//
//		// http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/
//		double cosHalfTheta = w * qb._w + x * qb._x + y * qb._y + z * qb._z;
//
//		if ( cosHalfTheta < 0 ) {
//
//			_w = - qb._w;
//			_x = - qb._x;
//			_y = - qb._y;
//			_z = - qb._z;
//
//			cosHalfTheta = - cosHalfTheta;
//
//		} else {
//
//			copy(qb);
//
//		}
//
//		if ( cosHalfTheta >= 1.0 ) {
//
//			_w = w;
//			_x = x;
//			_y = y;
//			_z = z;
//
//			return *this;
//
//		}
//
//		const double sqrSinHalfTheta = 1.0 - cosHalfTheta * cosHalfTheta;
//
//		if ( sqrSinHalfTheta <= Number::EPSILON ) {
//
//			const double s = 1 - t;
//			_w = s * w + t * _w;
//			_x = s * x + t * _x;
//			_y = s * y + t * _y;
//			_z = s * z + t * _z;
//
//			normalize();
//			if(onChangeCallback)
//                onChangeCallback(*this);
//
//			return *this;
//		}
//
//		const double sinHalfTheta = sqrt( sqrSinHalfTheta );
//		const double halfTheta = atan2( sinHalfTheta, cosHalfTheta );
//		const double ratioA = sin( ( 1 - t ) * halfTheta ) / sinHalfTheta,
//			ratioB = sin( t * halfTheta ) / sinHalfTheta;
//
//		_w = ( w * ratioA + _w * ratioB );
//		_x = ( x * ratioA + _x * ratioB );
//		_y = ( y * ratioA + _y * ratioB );
//		_z = ( z * ratioA + _z * ratioB );
//
//		if(onChangeCallback)
//            onChangeCallback(*this);
//
//		return *this;
//	}
//
//	Quaternion& slerpQuaternions(Quaternion& qa,Quaternion& qb,double t) {
//		return copy( qa ).slerp( qb, t );
//	}
//
//	Quaternion& random() {
//		// Derived from http://planning.cs.uiuc.edu/node198.html
//		// Note, this source uses w, x, y, z ordering,
//		// so we swap the order below.
//
//        // CPP random generator mechanism
//        std::default_random_engine generator;
//        std::uniform_real_distribution<double> distribution(0,1);
//		const double u1 = distribution(generator);
//		const double sqrt1u1 = sqrt( 1 - u1 );
//		const double sqrtu1 = sqrt( u1 );
//
//		const double u2 = 2 * M_PI * distribution(generator);
//
//		const double u3 = 2 * M_PI * distribution(generator);
//
//		return set(
//			sqrt1u1 * cos( u2 ),
//			sqrtu1 * sin( u3 ),
//			sqrtu1 * cos( u3 ),
//			sqrt1u1 * sin( u2 )
//		);
//	}
//
//	bool equals(Quaternion& quaternion) {
//		return ( quaternion._x == _x ) && ( quaternion._y == _y ) && ( quaternion._z == _z ) && ( quaternion._w == _w );
//	}
//
//    //create a quaternion from an array
//    Quaternion&	fromArray(double array[], int offset = 0) {
//		_x = array[ offset ];
//		_y = array[ offset + 1 ];
//		_z = array[ offset + 2 ];
//		_w = array[ offset + 3 ];
//
//		if(onChangeCallback)
//            onChangeCallback(*this);
//
//		return *this;
//	}
//
//    //from a quaternion to an array
//	void toArray(double array[], int offset = 0) {
//
//		array[ offset ] = _x;
//		array[ offset + 1 ] = _y;
//		array[ offset + 2 ] = _z;
//		array[ offset + 3 ] = _w;
//
//		//return array;
//	}
//
//// 	fromBufferAttribute( attribute, index ) {
//
//// 		this._x = attribute.getX( index );
//// 		this._y = attribute.getY( index );
//// 		this._z = attribute.getZ( index );
//// 		this._w = attribute.getW( index );
//
//// 		return this;
//
//// 	}
//
//	//function pointer's definition void(*callback)(void) which already includes declaration's everything
////	Quaternion& onChange(void(*callback)(void)){
////		onChangeCallback = callback;
////		return *this;
////	}
//    threecpp::Signal<void(const Quaternion&)> onChange;
//
//private:
//    double _x,_y,_z,_w;
//	OnChangeCallbackType onChangeCallback = nullptr;
////	std::function<void(const Quaternion&)> onChangeCallback;
//};
//
//
//// 	static slerp( qa, qb, qm, t ) {
//// 		console.warn( 'THREE.Quaternion: Static .slerp() has been deprecated. Use qm.slerpQuaternions( qa, qb, t ) instead.' );
//// 		return qm.slerpQuaternions( qa, qb, t );
//// 	}
//static void slerpFlat(double dst[],int dstOffset,double src0[],int srcOffset0,double src1[],int srcOffset1,double t){
//	// fuzz-free, array-based Quaternion SLERP operation
//	double
//		x0 = src0[ srcOffset0 + 0 ],
//		y0 = src0[ srcOffset0 + 1 ],
//		z0 = src0[ srcOffset0 + 2 ],
//		w0 = src0[ srcOffset0 + 3 ];
//
//	const double
//		x1 = src1[ srcOffset1 + 0 ],
//		y1 = src1[ srcOffset1 + 1 ],
//		z1 = src1[ srcOffset1 + 2 ],
//		w1 = src1[ srcOffset1 + 3 ];
//
//	if (t == 0){
//		dst[ dstOffset + 0 ] = x0;
//		dst[ dstOffset + 1 ] = y0;
//		dst[ dstOffset + 2 ] = z0;
//		dst[ dstOffset + 3 ] = w0;
//		return;
//	}
//
//	if (t == 1) {
//		dst[ dstOffset + 0 ] = x1;
//		dst[ dstOffset + 1 ] = y1;
//		dst[ dstOffset + 2 ] = z1;
//		dst[ dstOffset + 3 ] = w1;
//		return;
//	}
//
//	if ( w0 != w1 || x0 != x1 || y0 != y1 || z0 != z1 ) {
//		double s = 1 - t;
//		const double cos = x0 * x1 + y0 * y1 + z0 * z1 + w0 * w1,
//					dir = ( cos >= 0 ? 1 : - 1 ),
//					sqrSin = 1 - cos * cos;
//
//		// Skip the Slerp for tiny steps to avoid numeric problems:
//		if (sqrSin > Number::EPSILON) {
//			const double sin = sqrt( sqrSin ),
//						len = atan2( sin, cos * dir );
//
//			s = std::sin( s * len ) / sin;
//			t = std::sin( t * len ) / sin;
//		}
//
//		const double tDir = t * dir;
//
//		x0 = x0 * s + x1 * tDir;
//		y0 = y0 * s + y1 * tDir;
//		z0 = z0 * s + z1 * tDir;
//		w0 = w0 * s + w1 * tDir;
//
//		// Normalize in case we just did a lerp:
//		if ( s == 1 - t ) {
//			const double f = 1 / sqrt( x0 * x0 + y0 * y0 + z0 * z0 + w0 * w0 );
//
//			x0 *= f;
//			y0 *= f;
//			z0 *= f;
//			w0 *= f;
//		}
//
//	}
//
//	dst[ dstOffset ] = x0;
//	dst[ dstOffset + 1 ] = y0;
//	dst[ dstOffset + 2 ] = z0;
//	dst[ dstOffset + 3 ] = w0;
//}
//
//static void multiplyQuaternionsFlat(double dst[],int dstOffset,double src0[],int srcOffset0,double src1[],int srcOffset1) {
//
//	const double x0 = src0[ srcOffset0 ];
//	const double y0 = src0[ srcOffset0 + 1 ];
//	const double z0 = src0[ srcOffset0 + 2 ];
//	const double w0 = src0[ srcOffset0 + 3 ];
//
//	const double x1 = src1[ srcOffset1 ];
//	const double y1 = src1[ srcOffset1 + 1 ];
//	const double z1 = src1[ srcOffset1 + 2 ];
//	const double w1 = src1[ srcOffset1 + 3 ];
//
//	dst[ dstOffset ] = x0 * w1 + w0 * x1 + y0 * z1 - z0 * y1;
//	dst[ dstOffset + 1 ] = y0 * w1 + w0 * y1 + z0 * x1 - x0 * z1;
//	dst[ dstOffset + 2 ] = z0 * w1 + w0 * z1 + x0 * y1 - y0 * x1;
//	dst[ dstOffset + 3 ] = w0 * w1 - x0 * x1 - y0 * y1 - z0 * z1;
//}

#endif //QUATERNION_H