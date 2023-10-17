//
// Created by seanren on 8/6/2023.
//

#ifndef THREE_CPP_HEMISPHERE_LIGHT_H
#define THREE_CPP_HEMISPHERE_LIGHT_H

#include "light.h"
#include "color.h"

#include <memory>

class HemisphereLight : public Light {
public:
    using ptr = std::shared_ptr<HemisphereLight>;

    HemisphereLight();

    HemisphereLight(const Color& skyColor, const Color& groundColor, float intensity = 1);


    static ptr create(const Color& skyColor, const Color& groundColor, float intensity = 1) {
        return std::make_shared<HemisphereLight>(skyColor, groundColor, intensity);
    }

    virtual HemisphereLight* clone() override {
        return new HemisphereLight(*this);
    }

    virtual HemisphereLight& copy(const HemisphereLight& source);

};

//class HemisphereLight:public Light{
//public:
//    HemisphereLight(ColorSptr skyColor,ColorSptr groundColor,double intensity): Light(skyColor,intensity){
//        position->copy(*Object3D::default_up);
//        updateMatrix();
//        _groundColor->copy(*groundColor);
//    }
//
//    bool isHemisphereLight(){return true;}
//
//    LightType type(){return threecpp::LightType::HemisphereLight;}
//
//    HemisphereLight& copy(const HemisphereLight& source,bool recursive){
//        _groundColor->copy(*source.groundColor());
//    }
//
//    const ColorSptr groundColor() const {return _groundColor;}
//
//
//private:
//    ColorSptr _groundColor;
//
//};


#endif //THREE_CPP_HEMISPHERE_LIGHT_H
