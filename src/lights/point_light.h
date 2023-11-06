//
// Created by seanren on 11/20/2022.
//

#ifndef THREE_CPP_POINT_LIGHT_H
#define THREE_CPP_POINT_LIGHT_H

#include "light.h"
#include "point_light_shadow.h"
#include "color.h"
#include "number.h"

#include <string>
#include <memory>


class PointLight : public Light {
public:
    using sptr = std::shared_ptr<PointLight>;


    PointLight();

    PointLight(const Color& color, float intensity = 1, float distance = std::numeric_limits<float>::quiet_NaN(), float decay = std::numeric_limits<float>::quiet_NaN());


    static sptr create(const Color& color, float intensity = 1, float distance = std::numeric_limits<float>::quiet_NaN(), float decay = std::numeric_limits<float>::quiet_NaN()) {
        return std::make_shared<PointLight>(color,intensity,distance,decay);
    }

    virtual PointLight* clone() override {
        return new PointLight(*this);
    }
    virtual PointLight& copy(const PointLight& source);

    virtual float power() override {
        return (float)(this->intensity * 4 * math_number::PI);
    }
    virtual void setPower(float value) override {
        this->intensity = (float)(value / (4 * math_number::PI));
    }
};

//class PointLight : public Light {
//public:
//    //std::string type = "PointLight";
//    bool isPointLight = true;
//
//
//    PointLight( Color &color, double intensity, double distance = 0, double decay = 1 ):
//        Light(color,intensity),_decay(decay),_distance(distance){}
//
//    double power(){
//        // compute the light's luminous power (in lumens) from its intensity (in candela)
//        // for an isotropic light source, luminous power (lm) = 4 π luminous intensity (cd)
//        return _intensity * 4 * Number::PI;
//    }
//
//    PointLight& power(double power){
//        // set the light's intensity (in candela) from the desired luminous power (in lumens)
//        _intensity = power / ( 4 * Number::PI );
//        return *this;
//    }
//
//    LightType type() const override{
//        return _type;
//    }
//
//    double distance() const override{
//        return _distance;
//    }
//
//    std::shared_ptr<LightShadow> shadow() override {
//        return _shadow;
//    }
//
//
//
//private:
//    LightType _type = LightType::PointLight;
//    double _distance = 0.0;
//    int _decay = 1; // for physically correct lights, should be 2.
//    std::shared_ptr<LightShadow> _shadow = std::make_shared<PointLightShadow>();
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


#endif //THREE_CPP_POINT_LIGHT_H
