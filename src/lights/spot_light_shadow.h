//
// Created by SeanR on 2023/9/16.
//

#ifndef THREE_CPP_SPOT_LIGHT_SHADOW_H
#define THREE_CPP_SPOT_LIGHT_SHADOW_H

#include "light.h"

class SpotLightShadow : public LightShadow {

public:
    using sptr = std::shared_ptr<SpotLightShadow>;
    float focus;

    SpotLightShadow();

    SpotLightShadow(Camera::sptr& camera):LightShadow(camera){}

    static sptr create() {
        return std::make_shared<SpotLightShadow>();
    }

    static sptr create(Camera::sptr& camera) {
        return std::make_shared<SpotLightShadow>(camera);
    }

    virtual void updateMatrices(const Light& light, int viewportIndex = -1) override;

    virtual SpotLightShadow* clone() override {
        return new SpotLightShadow(*this);
    }

    virtual SpotLightShadow& copy(const SpotLightShadow& source) {
        LightShadow::copy(source);
        lightShadowType = LightShadowType::SpotLightShadow;
        return *this;
    }

};

//class SpotLightShadow:public LightShadow{
//public:
//    SpotLightShadow(CameraSptr camera): LightShadow(std::make_shared<PerspectiveCamera>(50, 1, 0.5, 500)){}
//
////    SpotLightShadow& updateMatrices( Light& light ) {
////        const fov = MathUtils.RAD2DEG * 2 * light.angle * this.focus;
////        const aspect = this.mapSize.width / this.mapSize.height;
////        const far = light.distance || camera.far;
////
////        if ( fov !== camera.fov || aspect !== camera.aspect || far !== camera.far ) {
////            camera.fov = fov;
////            camera.aspect = aspect;
////            camera.far = far;
////            camera.updateProjectionMatrix();
////        }
////
////        super.updateMatrices( light );
////    }
//
//private:
//    bool isSpotLightShadow = true;
//    int focus = 1;
//};

#endif //THREE_CPP_SPOT_LIGHT_SHADOW_H
