//
// Created by SeanR on 2023/10/20.
//
#include "light_shadow.h"

#include "light.h"

namespace lightshadow {
    Matrix4 _projScreenMatrix;
    Vector3 _lightPositionWorld;
    Vector3 _lookTarget;
}
using namespace lightshadow;

LightShadow::LightShadow(const LightShadow& source) : LightShadow(){
    frameExtents = source.frameExtents;
    frustum.copy(source.frustum);
    viewportCount = source.viewportCount;

    if (viewports.size() != source.viewports.size())
        viewports.resize(source.viewports.size());

    std::copy(source.viewports.begin(), source.viewports.end(), viewports.begin());

    this->camera.reset();

    camera = std::make_shared<Camera>(*source.camera);

    shadowBias = source.shadowBias;

    shadowRadius = source.shadowRadius;

    shadowNormalBias = source.shadowNormalBias;

    shadowMapSize.copy(source.shadowMapSize);

    /*if (source.shadowMap != nullptr)
        shadowMap = std::make_shared<GLRenderTarget>(*source.map);
    if(source.mapPass!=nullptr)
        mapPass = std::make_shared<GLRenderTarget>(*source.mapPass);*/

    shadowMatrix.copy(source.shadowMatrix);

    shadowCameraNear = source.shadowCameraNear;

    shadowCameraFar = source.shadowCameraFar;
}

LightShadow& LightShadow::copy(const LightShadow& source){
    frameExtents = source.frameExtents;
    frustum.copy(source.frustum);
    viewportCount = source.viewportCount;

    if (viewports.size() != source.viewports.size())
        viewports.resize(source.viewports.size());

    std::copy(source.viewports.begin(), source.viewports.end(), viewports.begin());

    this->camera.reset();

    camera = std::make_shared<Camera>(*source.camera);

    camera = std::make_shared<Camera>(*source.camera);

    shadowBias = source.shadowBias;

    shadowRadius = source.shadowRadius;

    shadowNormalBias = source.shadowNormalBias;

    shadowMapSize.copy(source.shadowMapSize);

    /*if (source.shadowMap != nullptr)
        shadowMap = std::make_shared<GLRenderTarget>(*source.map);
    if(source.mapPass!=nullptr)
        mapPass = std::make_shared<GLRenderTarget>(*source.mapPass);*/

    shadowMatrix.copy(source.shadowMatrix);

    shadowCameraNear = source.shadowCameraNear;

    shadowCameraFar = source.shadowCameraFar;

    return *this;
}

LightShadow* LightShadow::clone(){
    return new LightShadow(*this);
}

void LightShadow::updateMatrices(const Light& light, int viewportIndex){
    /*var shadowCamera = this.camera,
        shadowMatrix = this.matrix,
        projScreenMatrix = this._projScreenMatrix,
        lookTarget = this._lookTarget,
        lightPositionWorld = this._lightPositionWorld;*/

    _lightPositionWorld.setFromMatrixPosition(light.matrixWorld);
    camera->position.copy(_lightPositionWorld);

    _lookTarget.setFromMatrixPosition(light.target->matrixWorld);
    camera->lookAt(_lookTarget);
    camera->updateMatrixWorld();

    _projScreenMatrix.multiplyMatrices(camera->projectionMatrix, camera->matrixWorldInverse);
    frustum.setFromProjectionMatrix(_projScreenMatrix);

    shadowMatrix.set(
            0.5, 0.0, 0.0, 0.5,
            0.0, 0.5, 0.0, 0.5,
            0.0, 0.0, 0.5, 0.5,
            0.0, 0.0, 0.0, 1.0
    );

    shadowMatrix.multiply(camera->projectionMatrix);
    shadowMatrix.multiply(camera->matrixWorldInverse);
}
