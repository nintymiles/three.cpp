#include "euler.h"

#include "quaternion.h"
#include "matrix4.h"
#include "vector3.h"
#include <iostream>

Euler::Euler(float x, float y, float z) : _x(x), _y(y), _z(z), _order(Euler::RotationOrder::XYZ) {}

Euler& Euler::set(float x, float y, float z, RotationOrder order) {
    _x = x;
    _y = y;
    _z = z;
    _order = order;

    onChange.emitSignal(*this);

    return *this;
}

Euler& Euler::clone() {
    return *this;
}

Euler& Euler::copy(const Euler& euler) {
    _x = euler._x;
    _y = euler._y;
    _z = euler._z;
    _order = euler._order;

    onChange.emitSignal(*this);

    return *this;
}

Euler& Euler::setFromRotationMatrix(const Matrix4& m, RotationOrder order, bool update) {

    // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

    const float* te = m.elements;
    float m11 = te[0], m12 = te[4], m13 = te[8],
            m21 = te[1], m22 = te[5], m23 = te[9],
            m31 = te[2], m32 = te[6], m33 = te[10];



    if (order == RotationOrder::XYZ) {

        _y = asin(math::clamp(m13, -1.0f, 1.0f));

        if (abs(m13) < 0.9999999f) {

            _x = atan2(-m23, m33);
            _z = atan2(-m12, m11);

        }
        else {

            _x = atan2(m32, m22);
            _z = 0;

        }

    }
    else if (order == RotationOrder::YXZ) {

        _x = asin(-math::clamp(m23, -1.0f, 1.0f));

        if (abs(m23) < 0.9999999f) {

            _y = atan2(m13, m33);
            _z = atan2(m21, m22);

        }
        else {

            _y = atan2(-m31, m11);
            _z = 0;

        }

    }
    else if (order == RotationOrder::ZXY) {

        _x = asin(math::clamp(m32, -1.0f, 1.0f));

        if (abs(m32) < 0.9999999) {

            _y = atan2(-m31, m33);
            _z = atan2(-m12, m22);

        }
        else {

            _y = 0;
            _z = atan2(m21, m11);

        }

    }
    else if (order == RotationOrder::ZYX) {

        _y = asin(-math::clamp(m31, -1.0f, 1.0f));

        if (abs(m31) < 0.9999999f) {

            _x = atan2(m32, m33);
            _z = atan2(m21, m11);

        }
        else {

            _x = 0;
            _z = atan2(-m12, m22);

        }

    }
    else if (order == RotationOrder::YZX) {

        _z = asin(math::clamp(m21, -1.0f, 1.0f));

        if (abs(m21) < 0.9999999f) {

            _x = atan2(-m23, m22);
            _y = atan2(-m31, m11);

        }
        else {

            _x = 0;
            _y = atan2(m13, m33);

        }

    }
    else if (order == RotationOrder::XZY) {

        _z = asin(-math::clamp(m12, -1.0f, 1.0f));

        if (abs(m12) < 0.9999999f) {

            _x = atan2(m32, m22);
            _y = atan2(m13, m11);

        }
        else {

            _x = atan2(-m23, m33);
            _y = 0;

        }

    }

    _order = order;

    if (update) onChange.emitSignal(*this);

    return *this;
}

Euler& Euler::setFromQuaternion(const Quaternion& q, RotationOrder order, bool update) {
    Matrix4 _matrix;
    _matrix.makeRotationFromQuaternion(q);
    return setFromRotationMatrix(_matrix, order, update);
}

Euler& Euler::setFromVector3(const Vector3& v, RotationOrder order) {
    return set(v.x, v.y, v.z, order);
}

Euler& Euler::reorder(RotationOrder order) {
    Quaternion _quaternion;
    _quaternion.setFromEuler(*this);
    return setFromQuaternion(_quaternion, order);
}

bool Euler::equals(const Euler& euler) {
    return _x == euler._x && _y == euler._y && _z == euler._z && _order == euler._order;
}

Euler& Euler::fromArray(const float* array) {
    _x = array[0];
    _y = array[1];
    _z = array[2];

    //_order = Euler::defaultOrder;

    return *this;
}

float* Euler::toArray(float* array, unsigned offset) {
    float a[4];

    if (array == NULL) array = a;

    array[offset] = _x;
    array[offset + 1] = _y;
    array[offset + 2] = _z;
    array[offset + 3] = static_cast<float>(_order);

    return array;
}

Vector3& Euler::toVector3(Vector3* optionalResult) {
    assert(optionalResult != nullptr);
    return optionalResult->set(_x, _y, _z);

}

//float Euler::getX() const {
//    return _x;
//}
//
//float Euler::getY() const {
//    return _y;
//}
//
//float Euler::getZ() const {
//    return _z;
//}

Euler::RotationOrder Euler::getOrder() const {
    return _order;
}

void Euler::setX(float x) {
    _x = x;
    onChange.emitSignal(*this);

}

void Euler::setY(float y) {
    _y = y;
    onChange.emitSignal(*this);
}

void Euler::setZ(float z) {
    _z = z;
    onChange.emitSignal(*this);
}


void Euler::setOrder(Euler::RotationOrder order) {
    _order = order;
    onChange.emitSignal(*this);
}
bool Euler::operator == (const Euler& euler) const {
    return _x == euler._x && _y == euler._y && _z == euler._z && _order == euler._order;
}

Euler& Euler::operator= (const Euler& euler) {
    _x = euler._x;
    _y = euler._y;
    _z = euler._z;
    _order = euler._order;

    onChange.emitSignal(*this);

    return *this;
}

//#include "matrix4.h"
//#include "quaternion.h"
//#include "vector3.h"
//
////Euler::Euler():_matrix(std::make_shared<Matrix4>()),_quaternion(std::make_shared<Quaternion>()){}
//
//// std::shared_ptr<Matrix4> _matrix; // = std::make_shared<Matrix4>(new Matrix4());
//// std::shared_ptr<Quaternion> _quaternion; //= std::make_shared<Quaternion>(new Quaternion());
//namespace __euler {
//    Matrix4 _matrix;
//    Quaternion _quaternion;
//}
//using namespace __euler;
//
//Euler& Euler::reorder(RotationOrder newOrder) {
//    // WARNING: this discards revolution information -bhouston
//    _quaternion.setFromEuler(*this,true);
//
//    return setFromQuaternion(_quaternion, newOrder);
//}
//
//Euler& Euler::setFromVector3(Vector3& v,RotationOrder order) {
//	return set( v.x, v.y, v.z, order );
//}
//
//Euler& Euler::setFromQuaternion(Quaternion& q,RotationOrder order,bool update) {
//    _matrix.makeRotationFromQuaternion(q);
//
//    return setFromRotationMatrix( _matrix, order, update );
//}
//
//Euler& Euler::setFromRotationMatrix(Matrix4& m, RotationOrder order, bool update) {
//    // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)
//
//    const double m11 = m.elements[ 0 ], m12 = m.elements[ 4 ], m13 = m.elements[ 8 ];
//    const double m21 = m.elements[ 1 ], m22 = m.elements[ 5 ], m23 = m.elements[ 9 ];
//    const double m31 = m.elements[ 2 ], m32 = m.elements[ 6 ], m33 = m.elements[ 10 ];
//
//    switch ( order ) {
//
//        case XYZ:
//            _y = asin( MathUtils::clamp<double>( m13, - 1, 1 ) );
//
//            if ( abs( m13 ) < 0.9999999 ) {
//                _x = atan2( - m23, m33 );
//                _z = atan2( - m12, m11 );
//            } else {
//                _x = atan2( m32, m22 );
//                _z = 0;
//            }
//
//            break;
//        case YXZ:
//            _x = asin( - MathUtils::clamp<double>( m23, - 1, 1 ) );
//
//            if ( abs( m23 ) < 0.9999999 ) {
//                _y = atan2( m13, m33 );
//                _z = atan2( m21, m22 );
//
//            } else {
//                _y = atan2( - m31, m11 );
//                _z = 0;
//            }
//
//            break;
//        case ZXY:
//            _x = asin( MathUtils::clamp<double>( m32, - 1, 1 ) );
//
//            if ( abs( m32 ) < 0.9999999 ) {
//                _y = atan2( - m31, m33 );
//                _z = atan2( - m12, m22 );
//            } else {
//                _y = 0;
//                _z = atan2( m21, m11 );
//            }
//
//            break;
//        case ZYX:
//
//            _y = asin( - MathUtils::clamp<double>( m31, - 1, 1 ) );
//
//            if ( abs( m31 ) < 0.9999999 ) {
//                _x = atan2( m32, m33 );
//                _z = atan2( m21, m11 );
//            } else {
//                _x = 0;
//                _z = atan2( - m12, m22 );
//            }
//
//            break;
//        case YZX:
//            _z = asin( MathUtils::clamp<double>( m21, - 1, 1 ) );
//
//            if ( abs( m21 ) < 0.9999999 ) {
//                _x = atan2( - m23, m22 );
//                _y = atan2( - m31, m11 );
//            } else {
//                _x = 0;
//                _y = atan2( m13, m33 );
//
//            }
//
//            break;
//        case XZY:
//
//            _z = asin( - MathUtils::clamp<double>( m12, - 1, 1 ) );
//
//            if ( abs( m12 ) < 0.9999999 ) {
//                _x = atan2( m32, m22 );
//                _y = atan2( m13, m11 );
//
//            } else {
//                _x = atan2( - m23, m33 );
//                _y = 0;
//            }
//
//            break;
//        default:
//            //console.warn( 'THREE.Euler: .setFromRotationMatrix() encountered an unknown order: ' + order );
//            ;
//    }
//
//    _order = order;
//
//    if (update == true) _onChangeCallback();
//
//    return *this;
//
//}