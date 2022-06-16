#ifndef EULER_H
#define EULER_H

#include <functional>
#include <memory>

#include "math_utils.h"

enum euler_order{
    XYZ = 0,
    XZY,
    YXZ,
    YZX,
    ZXY,
    ZYX
};

class Matrix4;
class Vector3;
class Quaternion;

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

        Euler& setFromRotationMatrix(Matrix4& m, euler_order order, bool update = true );

        Euler& setFromQuaternion(Quaternion& q,euler_order order,bool update = true);

        Euler& setFromVector3(const Vector3& v,euler_order order);

        Euler& reorder(euler_order newOrder);

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
        //void _changeCallBack(void);  //wrong function decalartion
        //void (*_changeCallBack)(void); //right function (pointer) declaration
        std::function<void (void)> _onChangeCallback;

        std::shared_ptr<Matrix4> _matrix; // = std::make_shared<Matrix4>(new Matrix4());
        std::shared_ptr<Quaternion> _quaternion; //= std::make_shared<Quaternion>(new Quaternion());
};


#endif //EULER_H