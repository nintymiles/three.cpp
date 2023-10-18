//
// Created by SeanR on 2022/12/8.
//

#ifndef THREE_CPP_SRC_SCENES_FOG_H
#define THREE_CPP_SRC_SCENES_FOG_H

#include "color.h"

class Fog {
public:
    using sptr = std::shared_ptr<Fog>;

    std::string name;
    std::string type;
    Color color;
    float _near = 1.0f;
    float _far = 1000.0f;

    Fog() : type("Fog") {}

    Fog(const Fog& source) : Fog() {
        name = source.name;
        color = source.color;
        _near = source._near;
        _far = source._far;
    }

    Fog(const Color& color, float _near = std::numeric_limits<float>::quiet_NaN(), float _far = std::numeric_limits<float>::quiet_NaN()){
        name = "";
        type = "Fog";
        this->color = color;
        this->_near = !std::isnan(_near)?_near : 1.0f;
        this->_far = !std::isnan(_far) ? _far : 1000.0f;
    }

    virtual Fog* clone() {
        return new Fog(*this);
    }

    virtual Fog& copy(const Fog& source) {
        name = source.name;
        color = source.color;
        _near = source._near;
        _far = source._far;

        return *this;
    }

    bool operator == (const Fog& source) const {
        return name == source.name && color == source.color && _near == source._near && _far && source._far;
    }

    static sptr create() {
        return std::make_shared<Fog>();
    }

    static sptr create(const Color& color, float _near = std::numeric_limits<float>::quiet_NaN(), float _far = std::numeric_limits<float>::quiet_NaN()) {
        return std::make_shared<Fog>(color, _near, _far);
    }
};

class FogExp2 : public Fog {
public:
    using sptr = std::shared_ptr<FogExp2>;
    float density = 0.00025f;

    FogExp2() : Fog() {
        type = "FogExp2";
    }

    FogExp2(const Color& color, float density = std::numeric_limits<float>::quiet_NaN()) : Fog(color), density(density) {
        type = "FogExp2";
    }

    FogExp2(const FogExp2& source):Fog(source) {
        type = "FogExp2";
        density = source.density;
    }

    virtual FogExp2* clone() override {
        return new FogExp2(*this);
    }
    FogExp2& copy(const FogExp2& source) {
        Fog::Fog::copy(source);
        density = source.density;

        return *this;
    }

    static sptr create() {
        return std::make_shared<FogExp2>();
    }
    static sptr create(Color color, float density = std::numeric_limits<float>::quiet_NaN()) {
        return std::make_shared<FogExp2>(color, density);
    }
};

//class Fog {
//public:
//    std::string name = "Fog";
//    Color color;
//    double near,far;
//    Fog(Color color,double near = 1,double far = 1000):color(color),near(near),far(far){}
//    Fog(const Fog& fog):color(fog.color),near(fog.near),far(fog.far){}
//
//};
//
////import { Color } from '../math/Color.js';
////
////class Fog {
////
////    constructor( color, near = 1, far = 1000 ) {
////
////        this.name = '';
////
////        this.color = new Color( color );
////
////        this.near = near;
////        this.far = far;
////
////    }
////
////    clone() {
////
////        return new Fog( this.color, this.near, this.far );
////
////    }
////
////    toJSON( /* meta */ ) {
////
////        return {
////            type: 'Fog',
////            color: this.color.getHex(),
////            near: this.near,
////            far: this.far
////        };
////
////    }
////
////}

#endif //THREE_CPP_SRC_SCENES_FOG_H
