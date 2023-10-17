//
// Created by seanren on 02/05/2023.
//

#ifndef THREE_CPP_DIRECTIONAL_LIGHT_H
#define THREE_CPP_DIRECTIONAL_LIGHT_H

#include "light.h"
#include "directional_light_shadow.h"
#include "color.h"
#include <string>
#include <memory>

class DirectionalLight : public Light {
public:
    using sptr = std::shared_ptr<DirectionalLight>;

    DirectionalLight();

    DirectionalLight(const Color& color, float intensity = 1);

    DirectionalLight(const DirectionalLight& source);

    static sptr create(const Color color, float intensity = 1) {
        return std::make_shared<DirectionalLight>(color, intensity);
    }

    virtual DirectionalLight* clone() override {
        return new DirectionalLight(*this);
    }

    virtual DirectionalLight& copy(const DirectionalLight& source);


};

//class DirectionalLight : public Light {
//public:
//    //std::string type = "PointLight";
//    bool isPointLight = true;
//
//
//    DirectionalLight( Color &color, double intensity, double distance = 0, double decay = 1 ):
//        Light(color,intensity){}
//
//    LightType type() const override{
//        return _type;
//    }
//
//    double distance() const override{
//        return _distance;
//    }
//
//    PointLightShadow shadow() override{
//        return _shadow;
//    }
//
//
//
//private:
//    LightType _type = LightType::DirectionalLight;
//    double _distance = 0.0;
//    int _decay = 1; // for physically correct lights, should be 2.
//    PointLightShadow _shadow;
//
////    dispose() {
////
////        this.shadow.dispose();
////
////    }
////
////    copy( source ) {
////
////        super.copy( source );
////
////        this.distance = source.distance;
////        this.decay = source.decay;
////
////        this.shadow = source.shadow.clone();
////
////        return this;
////
////    }
//
//};


#endif //THREE_CPP_DIRECTIONAL_LIGHT_H
