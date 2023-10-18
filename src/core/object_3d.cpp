//
// Created by SeanRen on 2022/10/5.
//
#include "object_3d.h"
#include "quaternion.h"
#include "buffer_geometry.h"
#include "mesh_basic_material.h"

namespace object3d {
Vector3 _v1;
Quaternion _q1;
Matrix4 _m1;
Vector3 _target;

Vector3 _position;
Vector3 _scale;
Quaternion _quaternion;

Vector3 _xAxis(1, 0, 0);
Vector3 _yAxis(0, 1, 0);
Vector3 _zAxis(0, 0, 1);
}
using namespace object3d;

Object3D::Object3D() :id(++_objectIdCount) {
    rotation.onChange.connect(*this, &Object3D::onRotationChange);
    quaternion.onChange.connect(*this, &Object3D::onQuaternionChange);

    this->geometry = BufferGeometry::create();
    this->material = MeshBasicMaterial::create();
    material->color.setHex(0xffffff);
}

Object3D::Object3D(const Geometry::sptr& geometry, const Material::sptr& material) : id(++_objectIdCount),  geometry(geometry), material(material),materials({ material }) {
    rotation.onChange.connect(*this, &Object3D::onRotationChange);
    quaternion.onChange.connect(*this, &Object3D::onQuaternionChange);
}

Object3D::Object3D(const Geometry::sptr& geometry, const std::initializer_list<Material::sptr>& materials) : id(++_objectIdCount),geometry(geometry),materials(materials){
    rotation.onChange.connect(*this, &Object3D::onRotationChange);
    quaternion.onChange.connect(*this, &Object3D::onQuaternionChange);

    if (geometry != nullptr)
        this->geometry = geometry;

    if (materials.size() > 0) {
        this->materials.resize(materials.size());
        std::copy(materials.begin(), materials.end(), this->materials.begin());
    }
    this->material = this->materials[0];
}

Object3D::Object3D(const Geometry::sptr& geometry, const std::vector<Material::sptr>& materials) : id(++_objectIdCount), geometry(geometry), material(material), materials({ material }) {

    rotation.onChange.connect(*this, &Object3D::onRotationChange);
    quaternion.onChange.connect(*this, &Object3D::onQuaternionChange);

    if (geometry != nullptr)
        this->geometry = geometry;

    if (materials.size() > 0) {
        this->materials.resize(materials.size());
        std::copy(materials.begin(), materials.end(), this->materials.begin());
    }
    this->material = this->materials[0];
}

Object3D::Object3D(const Object3D& object) : Object3D() {
    name = object.name;
    up.copy(object.up);
    position.copy(object.position);
    rotation.copy(object.rotation);
    quaternion.copy(object.quaternion);
    scale.copy(object.scale);
    matrix.copy(object.matrix);
    modelViewMatrix.copy(object.modelViewMatrix);
    normalMatrix.copy(object.normalMatrix);
    matrixWorld.copy(object.matrixWorld);
    matrixAutoUpdate = object.matrixAutoUpdate;
    matrixWorldNeedsUpdate = object.matrixWorldNeedsUpdate;
    layers.copy(object.layers);
    visible = object.visible;
    castShadow = object.castShadow;
    receiveShadow = object.receiveShadow;
    frustumCulled = object.frustumCulled;
    renderOrder = object.renderOrder;
    if (children.size() != object.children.size()) {
        children.resize(object.children.size());
    }
    std::copy(object.children.begin(), object.children.end(), children.begin());

    if (geometry == nullptr && object.geometry != nullptr) {
        geometry = BufferGeometry::create();
        geometry->copy(*object.geometry);
    }
    if (materials.size() != object.materials.size()) {
        materials.resize(object.materials.size());
        std::copy(object.materials.begin(), object.materials.end(), materials.begin());
        material = object.materials[0];
    }


}

void Object3D::onRotationChange(const Euler& rotation){
    quaternion.setFromEuler(this->rotation, false);
}

void Object3D::onQuaternionChange(const Quaternion& quaternion){
    rotation.setFromQuaternion(this->quaternion, RotationOrder::Default, false);
}

void Object3D::applyMatrix4(Matrix4& matrix){
    if (matrixAutoUpdate) updateMatrix();

    matrix.premultiply(matrix);

    //matrix.decompose(&position, &quaternion, &scale);
    matrix.decompose(position, quaternion, scale);

}

Object3D& Object3D::applyQuaternion(Quaternion& quaternion){
    quaternion.premultiply(quaternion);

    return *this;
}

void Object3D::setRotationFromAxisAngle(Vector3& axis, float angle){
    quaternion.setFromAxisAngle(axis, angle);
}

void Object3D::setRotationFromEuler(Euler& euler){
    quaternion.setFromEuler(euler, true);
}

void Object3D::setRotationFromMatrix(Matrix4& m){
    // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

    quaternion.setFromRotationMatrix(m);
}

void Object3D::setRotationFromQuaternion(Quaternion& q){
    // assumes q is normalized

    quaternion.copy(q);
}

Object3D& Object3D::rotateOnAxis(const Vector3& axis, float angle){
    // rotate object on axis in object space
    // axis is assumed to be normalized
    _q1.setFromAxisAngle(axis, angle);

    quaternion.multiply(_q1);

    return *this;
}

Object3D& Object3D::rotateOnWorldAxis(const Vector3& axis, float angle){
    // rotate object on axis in world space
    // axis is assumed to be normalized
    // method assumes no rotated parent
    _q1.setFromAxisAngle(axis, angle);

    quaternion.premultiply(_q1);

    return *this;
}

Object3D& Object3D::rotateX(float angle){
    return this->rotateOnAxis(_xAxis, angle);
}

Object3D& Object3D::rotateY(float angle){
    return this->rotateOnAxis(_yAxis, angle);
}

Object3D& Object3D::rotateZ(float angle)
{
    return this->rotateOnAxis(_zAxis, angle);
}

Object3D& Object3D::translateOnAxis(const Vector3& axis, float distance){
    // translate object by distance along axis in object space
    // axis is assumed to be normalized

    _v1.copy(axis).applyQuaternion(quaternion);

    position.add(_v1.multiplyScalar(distance));

    return *this;
}

Object3D& Object3D::translateX(float distance){
    return translateOnAxis(_xAxis, distance);
}

Object3D& Object3D::translateY(float distance){
    return translateOnAxis(_yAxis, distance);
}

Object3D& Object3D::translateZ(float distance){
    return translateOnAxis(_zAxis, distance);
}

Vector3 Object3D::localToWorld(Vector3& vector){
    return vector.applyMatrix4(matrixWorld);
}

Vector3 Object3D::worldToLocal(Vector3& vector){
    return vector.applyMatrix4(_m1.getInverse(matrixWorld));
}

void Object3D::lookAt(const Vector3& vector){
    _target.copy(vector);
    //var parent = this.parent;

    updateWorldMatrix(true, false);

    _position.setFromMatrixPosition(matrixWorld);

    if (isCamera || isLight) {

        _m1.lookAt(_position, _target, up);

    }
    else {

        _m1.lookAt(_target, _position, up);

    }

    quaternion.setFromRotationMatrix(_m1);

    if (parent) {

        _m1.extractRotation(parent->matrixWorld);
        _q1.setFromRotationMatrix(_m1);
        quaternion.premultiply(_q1.invert());

    }

}

void Object3D::lookAt(float x, float y, float z){
    Vector3 vector(x, y, z);
    lookAt(vector);
    /*_target.set(x, y, z);

    updateWorldMatrix(true, false);

    _position.setFromMatrixPosition(matrixWorld);

    if (isCamera || isLight) {

        _m1.lookAt(_position, _target, up);

    }
    else {

        _m1.lookAt(_target, _position,up);

    }

    quaternion.setFromRotationMatrix(_m1);

    if (parent) {

        _m1.extractRotation(parent->matrixWorld);
        _q1.setFromRotationMatrix(_m1);
        quaternion.premultiply(_q1.inverse());
    }*/


}

Object3D& Object3D::add(const Object3D::ptr& object){
    if (object.get()==this) return *this;


    if (object->parent) {
        object->parent->remove(object);
    }
    object->parent = this;
    children.push_back(object);

    return *this;
}

Object3D& Object3D::remove(const Object3D::ptr& object){
    auto found = std::find(children.begin(), children.end(), object);

    if (found != children.end()) {

        (*found)->parent = nullptr;

        children.erase(found);

    }

    return *this;
}

Object3D& Object3D::removeAll()
{
    for (auto child : children) {
        child->parent = nullptr;
    }
    children.clear();

    return *this;
}

Object3D& Object3D::attach(const Object3D::ptr& object){
    // adds object as a child of this, while maintaining the object's world transform

    updateWorldMatrix(true, false);

    _m1.getInverse(matrixWorld);

    if (object->parent) {

        object->parent->updateWorldMatrix(true, false);

        _m1.multiply(object->parent->matrixWorld);

    }

    object->applyMatrix4(_m1);

    object->updateWorldMatrix(false, false);

    add(object);

    return *this;
}


Object3D* Object3D::clone(bool recursive){
    return new Object3D(*this);
}

Object3D& Object3D::copy(const Object3D& source,bool recursive){
    //return new Object3D(source);
    name = source.name;
    up.copy(source.up);
    position.copy(source.position);
    rotation.copy(source.rotation);
    quaternion.copy(source.quaternion);
    scale.copy(source.scale);
    matrix.copy(source.matrix);
    modelViewMatrix.copy(source.modelViewMatrix);
    normalMatrix.copy(source.normalMatrix);
    matrixWorld.copy(source.matrixWorld);
    matrixAutoUpdate = source.matrixAutoUpdate;
    matrixWorldNeedsUpdate = source.matrixWorldNeedsUpdate;
    layers.copy(source.layers);
    visible = source.visible;
    castShadow = source.castShadow;
    receiveShadow = source.receiveShadow;
    frustumCulled = source.frustumCulled;
    renderOrder = source.renderOrder;
    if (children.size() != source.children.size()) {
        children.resize(source.children.size());
    }
    std::copy(source.children.begin(), source.children.end(), children.begin());
    geometry->copy(*source.geometry);

    if (materials.size() != source.materials.size()) {
        materials.resize(source.materials.size());
        std::copy(source.materials.begin(), source.materials.end(), materials.begin());
        material = source.materials[0];
    }

    return *this;
}

Vector3& Object3D::getWorldPosition(Vector3& target){
    updateMatrixWorld(true);

    //float* e = matrixWorld.elements;
    double* e = matrixWorld.elements;

    return target.set(e[8], e[9], e[10]).normalize();
}

Quaternion& Object3D::getWorldQuaternion(Quaternion& target){
    updateMatrixWorld(true);

//    matrixWorld.decompose(&_position, &target, &_scale);
    matrixWorld.decompose(_position, target, _scale);

    return target;
}

Vector3& Object3D::getWorldScale(Vector3&& target){
    updateMatrixWorld(true);

    matrixWorld.decompose(_position, _quaternion, target);

    return target;
}

Vector3& Object3D::getWorldDirection(Vector3& target){
    updateMatrixWorld(true);

//    float* e = matrixWorld.elements;
    double* e = matrixWorld.elements;

    return target.set(e[8], e[9], e[10]).normalize();
}

void Object3D::traverse(std::function<void(Object3D&)> callback){
    callback(*this);

    for (auto child : children) {
        child->traverse(callback);
    }
}

void Object3D::traverseVisible(std::function<void(Object3D&)> callback){
    if (!visible) return;

    callback(*this);

    for (auto child : children) {
        child->traverseVisible(callback);
    }

}

void Object3D::traverseAncestors(std::function<void(Object3D&)> callback){
    if (parent) {
        callback(*this);
        parent->traverseAncestors(callback);
    }
}

void Object3D::updateMatrix(){
    matrix.compose(position, quaternion, scale);

    matrixWorldNeedsUpdate = true;
}

void Object3D::updateMatrixWorld(bool force){
    if (matrixAutoUpdate) updateMatrix();

    if (matrixWorldNeedsUpdate || force) {

        if (parent == nullptr) {

            matrixWorld.copy(matrix);

        }else {
            matrixWorld.multiplyMatrices(parent->matrixWorld, matrix);
        }

        matrixWorldNeedsUpdate = false;

        force = true;

    }

    // update children
    for (auto child : children) {
        child->updateMatrixWorld(force);
    }


}

void Object3D::updateWorldMatrix(bool updateParents, bool updateChildren){

    if (updateParents ==  true && parent) {

        parent->updateWorldMatrix(true, false);

    }

    if (matrixAutoUpdate) updateMatrix();

    if (parent == nullptr) {

        matrixWorld.copy(matrix);

    } else {

        matrixWorld.multiplyMatrices(parent->matrixWorld, matrix);

    }

    // update children
    if (updateChildren ==  true) {
        for (auto child : children) {
            child->updateWorldMatrix(false, true);
        }
    }
}


////Event _addedEvent = {"added"};
////Event _removedEvent = {"removed"};
//
////成员之间的数据传递应该尽量使用引用，引用可以理解为stack上指针。
////shared_ptr只是一种内存使用方式，heap上分配。但是不影响其所搭载的数据被stack上对象使用。
//std::shared_ptr<Quaternion> _q1 = std::make_shared<Quaternion>();
////Quaternion _q1();
//
//std::shared_ptr<Vector3>    _object3d_v1 = std::make_shared<Vector3>();
////Vector3 _object3d_v1();
//std::shared_ptr<Matrix4>    _object3d_m1 = std::make_shared<Matrix4>();
//// Matrix4 _object3d_m1();
//
//std::shared_ptr<Vector3>    _target = std::make_shared<Vector3>();
////Vector3 _target();
//
//std::shared_ptr<Vector3>    _position = std::make_shared<Vector3>();
////Vector3 _position();
//std::shared_ptr<Vector3>    _scale = std::make_shared<Vector3>();
////Vector3 _scale();
//std::shared_ptr<Quaternion> _object3d_quaternion = std::make_shared<Quaternion>();
////Quaternion _object3d_quaternion();
//
//
//std::shared_ptr<Vector3>    _xAxis = std::make_shared<Vector3>(1,0,0);
////Vector3 _xAxis(1f,0f,0f);
//std::shared_ptr<Vector3>    _yAxis = std::make_shared<Vector3>(0,1,0);
////Vector3 _yAxis(0f,1f,0f);
//std::shared_ptr<Vector3>    _zAxis = std::make_shared<Vector3>(0,0,1);
////Vector3 _zAxis(0f,0f,1f);
//
//std::shared_ptr<Vector3> Object3D::default_up= std::make_shared<Vector3>(0,1,0);
//
//Object3D& Object3D::rotateOnAxis( Vector3& axis,double angle ) {
//    // rotate object on axis in object space
//    // axis is assumed to be normalized
//    _q1->setFromAxisAngle( axis, angle );
//
//    quaternion->multiply( *_q1 );
//
//    return *this;
//}
//
//Object3D& Object3D::rotateOnWorldAxis( Vector3& axis, double angle ) {
//    // rotate object on axis in world space
//    // axis is assumed to be normalized
//    // method assumes no rotated parent
//    _q1->setFromAxisAngle( axis, angle );
//
//    quaternion->premultiply( *_q1 );
//
//    return *this;
//}
//
//Object3D& Object3D::rotateX( double angle ) {
//    rotateOnAxis( *_xAxis, angle );
//
//    return *this;
//}
//
//Object3D& Object3D::rotateY( double angle ) {
//    rotateOnAxis( *_yAxis, angle );
//
//    return *this;
//}
//
//Object3D& Object3D::rotateZ( double angle ) {
//    rotateOnAxis( *_zAxis, angle );
//
//    return *this;
//}
//
//Object3D& Object3D::translateOnAxis( Vector3& axis, double distance ) {
//    // translate object by distance along axis in object space
//    // axis is assumed to be normalized
//    _object3d_v1->copy( axis ).applyQuaternion( *quaternion );
//
//    position->add(_object3d_v1->multiplyScalar( distance ));
//
//    return *this;
//}
//
//Object3D& Object3D::translateX( double distance ) {
//    translateOnAxis( *_xAxis, distance );
//
//    return *this;
//}
//
//Object3D& Object3D::translateY( double distance ) {
//    translateOnAxis( *_yAxis, distance );
//
//    return *this;
//}
//
//Object3D& Object3D::translateZ( double distance ) {
//    translateOnAxis( *_zAxis, distance );
//
//    return *this;
//}
//
//Object3D& Object3D::worldToLocal( Vector3& vector ) {
//    vector.applyMatrix4( _object3d_m1->copy( *matrixWorld ).invert() );
//
//    return *this;
//}
//
//Object3D& Object3D::lookAt( double x,double y,double z ) {
//    // This method does not support objects having non-uniformly-scaled parent(s)
//
//    _target->set( x, y, z );
//
//    std::shared_ptr<Object3D> parent = this->parent;
//
//    updateWorldMatrix( true, false );
//
//    _position->setFromMatrixPosition( *matrixWorld );
//
//    //if object is an instance of camera or light
//    if ( this->getClassType()=="Camera" || this->getClassType()=="Light" ) {
//        _object3d_m1->lookAt( *_position, *_target, *up );
//    } else {
//        _object3d_m1->lookAt( *_target, *_position, *up );
//    }
//
//    quaternion->setFromRotationMatrix( *_object3d_m1 );
//
//    if ( parent ) {
//
//        _object3d_m1->extractRotation( *(parent->matrixWorld) );
//        _q1->setFromRotationMatrix( *_object3d_m1 );
//        quaternion->premultiply( _q1->invert() );
//
//    }
//
//    return *this;
//}
//
//Quaternion Object3D::getWorldQuaternion() {
//    updateWorldMatrix( true, false );
//
//    //执行默认初始化时，不需要使用括号,例如：Quaternion quat();
//    Quaternion quat;
//    matrixWorld->decompose( *_position, quat, *_scale );
//
//    return quat;
//}
//
//Vector3 Object3D::getWorldScale() {
//    updateWorldMatrix( true, false );
//
//    Vector3 target;
//    matrixWorld->decompose( *_position, *_object3d_quaternion, target );
//
//    return target;
//}