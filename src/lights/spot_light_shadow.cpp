//
// Created by SeanR on 2023/10/17.
//
#include "spot_light_shadow.h"

#include "perspective_camera.h"
#include "number.h"

SpotLightShadow::SpotLightShadow() :LightShadow(PerspectiveCamera::create(50.0f, 1.0f, 0.5f, 500.0f)) {
    lightShadowType = LightShadowType::SpotLightShadow;
    focus = 1.0;
}

void SpotLightShadow::updateMatrices(const Light& light, int viewportIndex){
    float fov = (float)(180 / math_number::PI * 2 * light.angle);
    float aspect = shadowMapSize.x / shadowMapSize.y;
    float far = !std::isnan(light.distance)&& light.distance != 0 ? light.distance : camera->_far;

    if (fov != camera->fov || aspect != camera->aspect || far != camera->_far)
    {
        camera->fov = fov;
        camera->aspect = aspect;
        camera->_far = far;
        camera->updateProjectionMatrix();
    }
    LightShadow::updateMatrices(light);
}
