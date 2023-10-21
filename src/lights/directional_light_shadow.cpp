//
// Created by SeanR on 2023/10/17.
//
#include "directional_light_shadow.h"

#include "orthographic_camera.h"

DirectionalLightShadow::DirectionalLightShadow() : LightShadow(OrthographicCamera::create(-5, 5, 5, -5, 0.5f, 500.0f)) {
    lightShadowType = LightShadowType::DirectionalLightShadow;
}

