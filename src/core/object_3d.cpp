//
// Created by SeanRen on 2022/10/5.
//
#include "object_3d.h"
#include "quaternion.h"

//Event _addedEvent = {"added"};
//Event _removedEvent = {"removed"};

//成员之间的数据传递应该尽量使用引用，引用可以理解为stack上指针。
//shared_ptr只是一种内存使用方式，heap上分配。但是不影响其所搭载的数据被stack上对象使用。
std::shared_ptr<Quaternion> _q1 = std::make_shared<Quaternion>();
//Quaternion _q1();

std::shared_ptr<Vector3>    _object3d_v1 = std::make_shared<Vector3>();
//Vector3 _object3d_v1();
std::shared_ptr<Matrix4>    _object3d_m1 = std::make_shared<Matrix4>();
// Matrix4 _object3d_m1();

std::shared_ptr<Vector3>    _target = std::make_shared<Vector3>();
//Vector3 _target();

std::shared_ptr<Vector3>    _position = std::make_shared<Vector3>();
//Vector3 _position();
std::shared_ptr<Vector3>    _scale = std::make_shared<Vector3>();
//Vector3 _scale();
std::shared_ptr<Quaternion> _object3d_quaternion = std::make_shared<Quaternion>();
//Quaternion _object3d_quaternion();


std::shared_ptr<Vector3>    _xAxis = std::make_shared<Vector3>(1,0,0);
//Vector3 _xAxis(1f,0f,0f);
std::shared_ptr<Vector3>    _yAxis = std::make_shared<Vector3>(0,1,0);
//Vector3 _yAxis(0f,1f,0f);
std::shared_ptr<Vector3>    _zAxis = std::make_shared<Vector3>(0,0,1);
//Vector3 _zAxis(0f,0f,1f);

std::shared_ptr<Vector3> Object3D::default_up= std::make_shared<Vector3>(0,1,0);

Object3D& Object3D::rotateOnAxis( Vector3& axis,double angle ) {
    // rotate object on axis in object space
    // axis is assumed to be normalized
    _q1->setFromAxisAngle( axis, angle );

    quaternion->multiply( *_q1 );

    return *this;
}

Object3D& Object3D::rotateOnWorldAxis( Vector3& axis, double angle ) {
    // rotate object on axis in world space
    // axis is assumed to be normalized
    // method assumes no rotated parent
    _q1->setFromAxisAngle( axis, angle );

    quaternion->premultiply( *_q1 );

    return *this;
}

Object3D& Object3D::rotateX( double angle ) {
    rotateOnAxis( *_xAxis, angle );

    return *this;
}

Object3D& Object3D::rotateY( double angle ) {
    rotateOnAxis( *_yAxis, angle );

    return *this;
}

Object3D& Object3D::rotateZ( double angle ) {
    rotateOnAxis( *_zAxis, angle );

    return *this;
}

Object3D& Object3D::translateOnAxis( Vector3& axis, double distance ) {
    // translate object by distance along axis in object space
    // axis is assumed to be normalized
    _object3d_v1->copy( axis ).applyQuaternion( *quaternion );

    position->add(_object3d_v1->multiplyScalar( distance ));

    return *this;
}

Object3D& Object3D::translateX( double distance ) {
    translateOnAxis( *_xAxis, distance );

    return *this;
}

Object3D& Object3D::translateY( double distance ) {
    translateOnAxis( *_yAxis, distance );

    return *this;
}

Object3D& Object3D::translateZ( double distance ) {
    translateOnAxis( *_zAxis, distance );

    return *this;
}

Object3D& Object3D::worldToLocal( Vector3& vector ) {
    vector.applyMatrix4( _object3d_m1->copy( *matrixWorld ).invert() );

    return *this;
}

Object3D& Object3D::lookAt( double x,double y,double z ) {
    // This method does not support objects having non-uniformly-scaled parent(s)

    _target->set( x, y, z );

    std::shared_ptr<Object3D> parent = this->parent;

    updateWorldMatrix( true, false );

    _position->setFromMatrixPosition( *matrixWorld );

    //if object is an instance of camera or light
    if ( this->getClassType()=="Camera" || this->getClassType()=="Light" ) {
        _object3d_m1->lookAt( *_position, *_target, *up );
    } else {
        _object3d_m1->lookAt( *_target, *_position, *up );
    }

    quaternion->setFromRotationMatrix( *_object3d_m1 );

    if ( parent ) {

        _object3d_m1->extractRotation( *(parent->matrixWorld) );
        _q1->setFromRotationMatrix( *_object3d_m1 );
        quaternion->premultiply( _q1->invert() );

    }

    return *this;
}

Quaternion Object3D::getWorldQuaternion() {
    updateWorldMatrix( true, false );

    //执行默认初始化时，不需要使用括号,例如：Quaternion quat();
    Quaternion quat;
    matrixWorld->decompose( *_position, quat, *_scale );

    return quat;
}

Vector3 Object3D::getWorldScale() {
    updateWorldMatrix( true, false );

    Vector3 target;
    matrixWorld->decompose( *_position, *_object3d_quaternion, target );

    return target;
}