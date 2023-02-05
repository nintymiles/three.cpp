//
// Created by seanren on 11/20/2022.
//

#ifndef THREE_CPP_POINT_LIGHT_SHADOW_H
#define THREE_CPP_POINT_LIGHT_SHADOW_H

#include "light_shadow.h"
#include "point_light.h"
#include "cameras/perspective_camera.h"
#include "cameras/orthographic_camera.h"

#include "matrix4.h"
#include "vector4.h"
#include "vector3.h"
#include "vector2.h"

class DirectionalLightShadow:public LightShadow{
public:
    DirectionalLightShadow(): LightShadow(std::make_shared<OrthographicCamera>(-5, 5, 5, -5, 0.5, 500)){}

    bool isDirectionalLightShadow() override {return _isDirectionalLightShadow;}

private:
    bool _isDirectionalLightShadow = true;

};


#endif //THREE_CPP_POINT_LIGHT_SHADOW_H
