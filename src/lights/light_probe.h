//
// Created by seanren on 02/05/2023.
//

#ifndef THREE_CPP_LIGHT_PROBE_H
#define THREE_CPP_LIGHT_PROBE_H

#include "light.h"
#include "color.h"
#include "common_types.h"
#include "spherical_harmonics3.h"
#include <string>


class LightProbe : public Light {
public:
    using sptr = std::shared_ptr<LightProbe>;

    LightProbe() : Light() {
        type = "LightProbe";
        isLightProbe = true;
    }

    LightProbe(const Color& color, float intensity = 1) : Light(color, intensity) {
        type = "LightProbe";
        isLightProbe = true;
    }

    LightProbe(const LightProbe& source) : Light(source) {
        type = "LightProbe";
        isLightProbe = true;
        sh.copy(source.sh);
    }
    LightProbe(const SphericalHarmonics3& sh, float intensity = 1.0f) : Light(Color(threecpp::Colors::white), intensity) {
        type = "LightProbe";
        isLightProbe = true;
        this->sh = sh;
    }

    virtual LightProbe& copy(const LightProbe& source) {
        Light::copy(source);
        sh.copy(source.sh);
        isLightProbe = true;
        return *this;
    }

    static sptr create() {
        return std::make_shared<LightProbe>();
    }

    static sptr create(const Color& color, float intensity = 1) {
        return std::make_shared<LightProbe>(color, intensity);
    }

    static ptr create(const SphericalHarmonics3& sh, float intensity = 1) {
        return std::make_shared<LightProbe>(sh, intensity);
    }

};

//class LightProbe : public Light {
//public:
//    LightProbe(ColorSptr color, double intensity):
//        Light(color,intensity){}
//
//
//    LightType type() const override{
//        return _type;
//    }
//
//    SphericalHarmonics3Sptr sh(){
//        return _sh;
//    }
//
//    bool isLightProbe() const override{
//        return _isLightProbe;
//    }
//
//    virtual bool isHemisphereLightProbe() const;
//
//
//private:
//    LightType _type = LightType::LightProbe;
//    bool _isLightProbe = true;
//    SphericalHarmonics3Sptr _sh;
//
//
//};
//
//using LightProbeSptr = std::shared_ptr<LightProbe>;


#endif //THREE_CPP_LIGHT_PROBE_H
