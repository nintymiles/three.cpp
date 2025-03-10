//
// Created by SeanR on 2023/10/16.
//

#include "camera.h"

Camera::Camera() : Object3D(){
    type = "Camera";
    isCamera = true;
}

Camera::Camera(const Camera& camera) : Object3D(camera) {
    type = "Camera";
    fov = camera.fov;
    aspect = camera.aspect;
    _far = camera._far;
    _near = camera._near;
    zoom = camera.zoom;
    matrixWorldInverse = camera.matrixWorldInverse;
    projectionMatrix = camera.projectionMatrix;
    projectionMatrixInverse = camera.projectionMatrixInverse;

    isCamera = true;
}

Vector3& Camera::getWorldDirection(Vector3& target){
    this->updateWorldMatrix(true,false);

    float* e = matrixWorld.elements;

    return target.set(-e[8], -e[9], -e[10]).normalize();

}

void Camera::updateMatrixWorld(bool force){
    Object3D::updateMatrixWorld(force);

    matrixWorldInverse.copy(this->matrixWorld).invert();
}

void Camera::updateWorldMatrix(bool updateParents, bool updateChildren){
    Object3D::updateWorldMatrix(updateParents, updateChildren);

    matrixWorldInverse.copy(this->matrixWorld).invert();
}

Camera& Camera::copy(Camera& source, bool recursive) {
    Object3D::copy(source, recursive);
    isCamera = true;

    fov = source.fov;
    aspect = source.aspect;
    _far = source._far;
    _near = source._near;
    zoom = source.zoom;

    matrixWorldInverse = source.matrixWorldInverse;
    projectionMatrix = source.projectionMatrix;
    projectionMatrixInverse = source.projectionMatrixInverse;

    return *this;
}

Camera* Camera::clone(){
    Camera* camera = new Camera();
    camera->copy(*this);
    return camera;
}

void Camera::updateProjectionMatrix(){
    matrixWorldInverse.getInverse(matrixWorld);
}
