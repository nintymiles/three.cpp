#ifndef EULER_H
#define EULER_H

#include "simplesignal.h"

class Matrix4;

class Quaternion;

class Vector3;

class Matrix3;


class Euler {
public:
    enum class RotationOrder { XYZ, YZX, ZXY, XZY, YXZ, ZYX,Default };
    float _x, _y, _z;
    static const RotationOrder defaultOrder = RotationOrder::XYZ;
private:


    RotationOrder _order;

public:



    threecpp::Signal<void(const Euler&)> onChange;

    Euler() :_x(0), _y(0), _z(0), _order(RotationOrder::XYZ) {}

    Euler(float x, float y, float z, RotationOrder order) : _x(x), _y(y), _z(z), _order(order) {}

    Euler(const Euler& e) : _x(e._x), _y(e._y), _z(e._z), _order(e._order) {}

    Euler(float x, float y, float z);

    Euler& set(float x, float y, float z, RotationOrder order = RotationOrder::XYZ);

    Euler& clone();

    Euler& copy(const Euler& euler);

    Euler& setFromRotationMatrix(const Matrix4& m, RotationOrder order = RotationOrder::XYZ, bool update = false);

    Euler& setFromQuaternion(const Quaternion& q, RotationOrder order = RotationOrder::XYZ, bool update = false);

    Euler& setFromVector3(const Vector3& v, RotationOrder order = RotationOrder::XYZ);

    Euler& reorder(RotationOrder order);

    bool equals(const Euler& euler);

    Euler& fromArray(const float* array);

    float* toArray(float* array, unsigned offset = 0);

    Vector3& toVector3(Vector3* optionalResult = NULL);

    float getX() const;

    float getY() const;

    float getZ() const;

    RotationOrder getOrder() const;

    void setX(float x);

    void setY(float y);

    void setZ(float z);

    void setOrder(RotationOrder order);


    bool operator == (const Euler& euler) const;

    bool operator != (const Euler& euler) const { return !(*this == euler); }

    Euler& operator = (const Euler& euler);
};

//#include <functional>
//#include <memory>
//
//#include "math_utils.h"
//#include "simplesignal.h"
//
//enum RotationOrder{
//    XYZ = 0,
//    XZY,
//    YXZ,
//    YZX,
//    ZXY,
//    ZYX,
//    Default
//};
////enum class RotationOrder { XYZ, YZX, ZXY, XZY, YXZ, ZYX,Default };
//
//class Matrix4;
//class Vector3;
//class Quaternion;
//
//class Euler{
//    public:
//        static const RotationOrder defaultOrder = RotationOrder::XYZ;
//
//        //无默认构造器，会导致undefined symbol错误，通常为默认构造函数声明而未定义导致
//        //Undefined symbols for architecture arm64: "Euler::Euler()", referenced from
//        Euler()=default;
//        Euler(double x,double y,double z,RotationOrder order = Euler::defaultOrder){}
//
//        double x() const{return _x;}
//        void x(double value){
//            _x = value;
//            //_onChangeCallback();
//            onChange.emitSignal(*this);
//        }
//
//        double y() const{return _y;}
//        void y(double value){
//            _y = value;
//            //_onChangeCallback();
//            onChange.emitSignal(*this);
//        }
//
//        double z() const{return _z;}
//        void z(double value){
//            _z = value;
//            //_onChangeCallback();
//            onChange.emitSignal(*this);
//        }
//
//
//        RotationOrder order() const{return _order;}
//        void order(RotationOrder value){
//            _order = value;
//            //_onChangeCallback();
//            onChange.emitSignal(*this);
//        }
//
//        Euler& set(double x,double y,double z) {
//            //return set(x,y,z);
//            _x = x;
//            _y = y;
//            _z = z;
//            //_onChangeCallback();
//            onChange.emitSignal(*this);
//
//            return *this;
//        }
//
//        Euler& set(double x,double y,double z, RotationOrder order) {
//            _x = x;
//            _y = y;
//            _z = z;
//            _order = order;
//
//            //_onChangeCallback();
//            onChange.emitSignal(*this);
//
//            return *this;
//        }
//
//        Euler clone() {
//            return Euler(_x, _y, _z, _order);
//        }
//
//        Euler& copy(const Euler& euler) {
//            _x = euler.x();
//            _y = euler.y();
//            _z = euler.z();
//            _order = euler.order();
//
//            //_onChangeCallback();
//            onChange.emitSignal(*this);
//
//            return *this;
//        }
//
//        Euler& setFromRotationMatrix(Matrix4& m, RotationOrder order, bool update = true );
//
//        Euler& setFromQuaternion(Quaternion& q,RotationOrder order,bool update = true);
//
//        Euler& setFromVector3(Vector3& v,RotationOrder order);
//
//        Euler& reorder(RotationOrder newOrder);
//
//        bool equals(Euler&  euler) {
//            return (euler.x() == _x ) && (euler.y() == _y) && (euler.z() == _z) && (euler.order() == _order);
//        }
//
//        Euler& fromArray(double array[4]) {
//            _x = array[ 0 ];
//            _y = array[ 1 ];
//            _z = array[ 2 ];
//            _order = (RotationOrder)array[ 3 ];
//
//            //_onChangeCallback();
//            onChange.emitSignal(*this);
//
//            return *this;
//        }
//
//        double* toArray(double array[],int offset = 0) {
//            array[ offset ] = _x;
//            array[ offset + 1 ] = _y;
//            array[ offset + 2 ] = _z;
//            array[ offset + 3 ] = _order;
//
//            return array;
//        }
//
////        void onChange(std::function<void (void)> onChangeCallBack){
////            _onChangeCallback = onChangeCallBack;
////        }
//        threecpp::Signal<void(const Euler&)> onChange;
//
//    private:
//        double _x,_y,_z;
//        RotationOrder _order;
//        //void _changeCallBack(void);  //wrong function decalartion
//        //void (*_changeCallBack)(void); //right function (pointer) declaration
//        std::function<void (void)> _onChangeCallback;
//
//};

#endif //EULER_H