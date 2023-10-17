//
// Created by seanren on 11/20/2022.
//

#ifndef THREE_CPP_POINT_LIGHT_SHADOW_H
#define THREE_CPP_POINT_LIGHT_SHADOW_H

#include "light_shadow.h"
//#include "point_light.h"
#include "perspective_camera.h"
#include "orthographic_camera.h"

#include "matrix4.h"
#include "vector4.h"
#include "vector3.h"
#include "vector2.h"

class DirectionalLightShadow : public LightShadow {
public:
    using sptr = std::shared_ptr<DirectionalLightShadow>;

    DirectionalLightShadow();

    DirectionalLightShadow(const DirectionalLightShadow& source) : LightShadow(source) {
        lightShadowType = LightShadowType::DirectionalLightShadow;
    }

    static sptr create() {
        return std::make_shared<DirectionalLightShadow>();
    }

    virtual DirectionalLightShadow* clone() override {
        return new DirectionalLightShadow(*this);
    }

    virtual DirectionalLightShadow& copy(const DirectionalLightShadow& source) {
        LightShadow::copy(source);
        lightShadowType = LightShadowType::DirectionalLightShadow;
        return *this;
    }
};



//class DirectionalLightShadow:public LightShadow{
//public:
//    DirectionalLightShadow(): LightShadow(std::make_shared<OrthographicCamera>(-5, 5, 5, -5, 0.5, 500)){}
//
//    bool isDirectionalLightShadow() override {return _isDirectionalLightShadow;}
//
//private:
//    bool _isDirectionalLightShadow = true;
//
//};


#endif //THREE_CPP_POINT_LIGHT_SHADOW_H
