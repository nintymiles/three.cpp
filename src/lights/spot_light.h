//
// Created by SeanR on 2023/9/16.
//

#ifndef THREE_CPP_SPOT_LIGHT_H
#define THREE_CPP_SPOT_LIGHT_H
#include "light.h"
#include "object_3d.h"

#include <memory>

class SpotLight : public Light {
public:
    using sptr = std::shared_ptr<SpotLight>;

    SpotLight();

    SpotLight(const Color& color, float intensity = 1, float distance = std::numeric_limits<float>::quiet_NaN(),	float angle = std::numeric_limits<float>::quiet_NaN(),	float penumbra = std::numeric_limits<float>::quiet_NaN(),float decay = std::numeric_limits<float>::quiet_NaN());

    static sptr create(const Color& color, float intensity = 1, float distance = std::numeric_limits<float>::quiet_NaN(),	float angle = std::numeric_limits<float>::quiet_NaN(),	float penumbra = std::numeric_limits<float>::quiet_NaN(),float decay = std::numeric_limits<float>::quiet_NaN()){
        return std::make_shared<SpotLight>(color,intensity,distance,angle,penumbra,decay);
    }

    virtual SpotLight* clone() override {
        return new SpotLight(*this);
    }

    virtual SpotLight& copy(const SpotLight& source);

    virtual float power() override {
        return (float)(this->intensity *  M_PI);
    }
    virtual void setPower(float value) override {
        this->intensity = (float)(value /  M_PI);
    }
};

//class SpotLight:public Light{
//public:
//    SpotLight(ColorSptr color,double intensity,double distance = 0, double angle = Number::PI / 3, double penumbra = 0, double decay = 1 )
//        : Light(color,intensity),distance(distance),angle(angle),penumbra(penumbra),decay(decay),target(std::make_shared<Object3D>()){
//        position->copy(*Object3D::default_up);
//        updateMatrix();
//
//    }
//
//    LightType type() const{return LightType::SpotLight;}
//
//    const double power() const{
//        // compute the light's luminous power (in lumens) from its intensity (in nits)
//        return this->_intensity * Number::PI;
//    }
//
//    SpotLight& power( double power ) {
//        // set the light's intensity (in nits) from the desired luminous power (in lumens)
//        this->_intensity = power / Number::PI;
//    }
//
//private:
//   bool isSpotLight = true;
//   //std::string type = "SpotLight"
//   double distance = 0;
//   double angle = Number::PI / 3;
//   double penumbra = 0;
//   double decay = 1;
//   Object3DSptr target;
//
//};

#endif //THREE_CPP_SPOT_LIGHT_H
