//
// Created by seanren on 02/05/2023.
//

#ifndef THREE_CPP_AMBIENT_LIGHT_H
#define THREE_CPP_AMBIENT_LIGHT_H

#include "light.h"
#include "color.h"
#include <string>
#include <memory>

class AmbientLight : public Light {
public:
    using sptr = std::shared_ptr<AmbientLight>;

    AmbientLight();

    AmbientLight(const Color& color, float intensity = 1);

    AmbientLight(const AmbientLight& source);

    static sptr create() {
        return std::make_shared<AmbientLight>();
    }

    static sptr create(const Color& color, float intensity = 1) {
        return std::make_shared<AmbientLight>(color, intensity);
    }

    virtual AmbientLight* clone() override {
        return new AmbientLight(*this);
    }
    virtual AmbientLight& copy(const AmbientLight& source);

};

//class AmbientLight : public Light {
//public:
//    AmbientLight(Color &color, double intensity):
//        Light(color,intensity){}
//
//
//    LightType type() const override{
//        return _type;
//    }
//
//    bool isAmbientLight() const override{
//        return _isAmbientLight;
//    }
//
//
//private:
//    LightType _type = LightType::AmbientLight;
//    bool _isAmbientLight = true;
//
//};


#endif //THREE_CPP_AMBIENT_LIGHT_H
