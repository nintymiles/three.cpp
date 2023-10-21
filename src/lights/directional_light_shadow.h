//
// Created by seanren on 11/20/2022.
//

//如果guard header和其它重复，会导致相关内容无法加载
//#ifndef THREE_CPP_POINT_LIGHT_SHADOW_H
//#define THREE_CPP_POINT_LIGHT_SHADOW_H
#ifndef THREE_CPP_DIRECTIONAL_LIGHT_SHADOW_H
#define THREE_CPP_DIRECTIONAL_LIGHT_SHADOW_H

#include "light_shadow.h"

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

    virtual void updateMatrices(const Light& light, int viewportIndex=-1) override{}
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


#endif //THREE_CPP_DIRECTIONAL_LIGHT_SHADOW_H
