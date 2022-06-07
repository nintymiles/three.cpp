#include <functional>
#include <memory>

#include "matrix4.h"
#include "quaternion.h"
#include "math_utils.h"

enum euler_order{
    XYZ = 0,
    XZY,
    YXZ,
    YZX,
    ZXY,
    ZYX
};

class Euler{
    public:
        static const euler_order defaultOrder = euler_order::XYZ;

        Euler();
        Euler(double x,double y,double z,euler_order order = Euler::defaultOrder){}

        double x() const{return _x;}
        void x(double value){
            _x = value;
            _onChangeCallback();
        }

        double y() const{return _y;}
        void y(double value){
            _y = value;
            _onChangeCallback();
        }

        double z() const{return _z;}
        void z(double value){
            _z = value;
            _onChangeCallback();
        }


        euler_order order() const{return _order;}
        void order(euler_order value){
            _order = value;
            _onChangeCallback();
        }

        Euler& set(double x,double y,double z) {
            //return set(x,y,z);
            _x = x;
            _y = y;
            _z = z;
            _onChangeCallback();

            return *this;
        }

        Euler& set(double x,double y,double z, euler_order order) {
            _x = x;
            _y = y;
            _z = z;
            _order = order;

            _onChangeCallback();

            return *this;
        }

        Euler clone() {
            return Euler(_x, _y, _z, _order);
        }

        Euler& copy(Euler euler) {
            _x = euler.x();
            _y = euler.y();
            _z = euler.z();
            _order = euler.order();

            _onChangeCallback();

            return *this;
        }

        Euler& setFromRotationMatrix(Matrix4& m, euler_order order, bool update = true ) {
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

        Euler& setFromQuaternion(Quaternion& q,euler_order order,bool update = true) {
            _matrix->makeRotationFromQuaternion(q);

            return setFromRotationMatrix( *_matrix, order, update );
        }

        Euler& setFromVector3(const Vector3& v,euler_order order);

        // Euler& reorder(euler_order newOrder) {
        // 	// WARNING: this discards revolution information -bhouston
        // 	_quaternion->setFromEuler(*this);

        // 	return setFromQuaternion(*_quaternion, newOrder);
        // }

        bool equals(Euler&  euler) {
            return (euler.x() == _x ) && (euler.y() == _y) && (euler.z() == _z) && (euler.order() == _order);
        }

        Euler& fromArray(double array[4]) {
            _x = array[ 0 ];
            _y = array[ 1 ];
            _z = array[ 2 ];
            _order = (euler_order)array[ 3 ];

            _onChangeCallback();

            return *this;
        }

        double* toArray(double array[],int offset = 0) {
            array[ offset ] = _x;
            array[ offset + 1 ] = _y;
            array[ offset + 2 ] = _z;
            array[ offset + 3 ] = _order;

            return array;
        }

        void onChange(std::function<void (void)> onChangeCallBack){
            _onChangeCallback = onChangeCallBack;
        }

    private:
        double _x,_y,_z;
        euler_order _order;    
        //void _changeCallBack(void);
        std::function<void (void)> _onChangeCallback;

        std::shared_ptr<Matrix4> _matrix; // = std::make_shared<Matrix4>(new Matrix4());
        std::shared_ptr<Quaternion> _quaternion; //= std::make_shared<Quaternion>(new Quaternion());
};