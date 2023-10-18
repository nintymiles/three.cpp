//
// Created by SeanR on 2023/10/18.
//
#include "point_light_shadow.h"

namespace pointlightshaow {
    std::vector<Vector3> _cubeDirections = {
            Vector3(1,0,0),
            Vector3(-1,0,0),
            Vector3(0,0,1),
            Vector3(0,0,-1),
            Vector3(0,1,0),
            Vector3(0,-1,0)
    };

    std::vector<Vector3> _cubeUps = {
            Vector3(0,1,0),
            Vector3(0,1,0),
            Vector3(0,1,0),
            Vector3(0,1,0),
            Vector3(0,0,1),
            Vector3(0,0,-1)
    };

    Matrix4 projScreenMatrix;

    Vector3 lightPositionWorld;

    Vector3 lookTarget;
}
using namespace pointlightshaow;

PointLightShadow::PointLightShadow() : LightShadow(PerspectiveCamera::create(90.0f,1.0f,0.5f,500.0f)){
    lightShadowType = LightShadowType::PointLightShadow;
    frameExtents.copy(Vector2(4, 2));
    viewportCount = 6;
    this->viewports.resize(6);
    std::vector<Vector4f> _viewports = {
            Vector4f(2,1,1,1),
            Vector4f(0,1,1,1),
            Vector4f(3,1,1,1),
            Vector4f(1,1,1,1),
            Vector4f(3,0,1,1),
            Vector4f(1,0,1,1)
    };

    for (unsigned i = 0; i < viewports.size(); i++) {
        this->viewports[i].copy(viewports[i]);
    }
}

void PointLightShadow::updateMatrices(const Light& light, int viewportIndex){
    if (viewportIndex == -1) viewportIndex = 0;


    /*var camera = this.Camera;
    var shadowMatrix = this.Matrix;
    var lightPositionWorld = this._lightPositionWorld;
    var lookTarget = this._lookTarget;
    var projScreenMatrix = this._projScreenMatrix;*/

    lightPositionWorld.setFromMatrixPosition(light.matrixWorld);
    camera->position.copy(lightPositionWorld);

    lookTarget.copy(camera->position);
    lookTarget.add(_cubeDirections[viewportIndex]);
    camera->up.copy(_cubeUps[viewportIndex]);
    camera->lookAt(lookTarget);
    camera->updateMatrixWorld();

    shadowMatrix.makeTranslation(-lightPositionWorld.x, -lightPositionWorld.y, -lightPositionWorld.z);

    projScreenMatrix.multiplyMatrices(camera->projectionMatrix, camera->matrixWorldInverse);
    frustum.setFromProjectionMatrix(projScreenMatrix);
}

