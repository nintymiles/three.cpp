#include "euler.h"

#include "matrix4.h"
#include "quaternion.h"
#include "vector3.h"

Euler::Euler():_matrix(std::make_shared<Matrix4>(new Matrix4())),_quaternion(std::make_shared<Quaternion>(new Quaternion())){}

Euler& Euler::reorder(euler_order newOrder) {
    // WARNING: this discards revolution information -bhouston
    _quaternion->setFromEuler(*this,true);

    return setFromQuaternion(*_quaternion, newOrder);
}

Euler& Euler::setFromVector3(const Vector3& v,euler_order order) {
	return set( v.x, v.y, v.z, order );
}

Euler& Euler::setFromQuaternion(Quaternion& q,euler_order order,bool update) {
    _matrix->makeRotationFromQuaternion(q);

    return setFromRotationMatrix( *_matrix, order, update );
}


Euler& Euler::setFromRotationMatrix(Matrix4& m, euler_order order, bool update) {
    // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

    const double m11 = m.elements[ 0 ], m12 = m.elements[ 4 ], m13 = m.elements[ 8 ];
    const double m21 = m.elements[ 1 ], m22 = m.elements[ 5 ], m23 = m.elements[ 9 ];
    const double m31 = m.elements[ 2 ], m32 = m.elements[ 6 ], m33 = m.elements[ 10 ];

    switch ( order ) {

        case XYZ:
            _y = asin( clamp<double>( m13, - 1, 1 ) );

            if ( abs( m13 ) < 0.9999999 ) {
                _x = atan2( - m23, m33 );
                _z = atan2( - m12, m11 );
            } else {
                _x = atan2( m32, m22 );
                _z = 0;
            }

            break;
        case YXZ:
            _x = asin( - clamp<double>( m23, - 1, 1 ) );

            if ( abs( m23 ) < 0.9999999 ) {
                _y = atan2( m13, m33 );
                _z = atan2( m21, m22 );

            } else {
                _y = atan2( - m31, m11 );
                _z = 0;
            }

            break;
        case ZXY:
            _x = asin( clamp<double>( m32, - 1, 1 ) );

            if ( abs( m32 ) < 0.9999999 ) {
                _y = atan2( - m31, m33 );
                _z = atan2( - m12, m22 );
            } else {
                _y = 0;
                _z = atan2( m21, m11 );
            }

            break;
        case ZYX:

            _y = asin( - clamp<double>( m31, - 1, 1 ) );

            if ( abs( m31 ) < 0.9999999 ) {
                _x = atan2( m32, m33 );
                _z = atan2( m21, m11 );
            } else {
                _x = 0;
                _z = atan2( - m12, m22 );
            }

            break;
        case YZX:
            _z = asin( clamp<double>( m21, - 1, 1 ) );

            if ( abs( m21 ) < 0.9999999 ) {
                _x = atan2( - m23, m22 );
                _y = atan2( - m31, m11 );
            } else {
                _x = 0;
                _y = atan2( m13, m33 );

            }

            break;
        case XZY:

            _z = asin( - clamp<double>( m12, - 1, 1 ) );

            if ( abs( m12 ) < 0.9999999 ) {
                _x = atan2( m32, m22 );
                _y = atan2( m13, m11 );

            } else {
                _x = atan2( - m23, m33 );
                _y = 0;
            }

            break;
        default:
            //console.warn( 'THREE.Euler: .setFromRotationMatrix() encountered an unknown order: ' + order );
            ;
    }

    _order = order;

    if (update == true) _onChangeCallback();

    return *this;

}