//
// Created by seanren on 11/20/2022.
//

#ifndef THREE_CPP_LIGHT_H
#define THREE_CPP_LIGHT_H

#include "object_3d.h"
#include "color.h"
#include "common_types.h"
#include "light_shadow.h"
#include "spherical_harmonics3.h"

#include <string>
#include <memory>

enum class LightType : unsigned {
    Light=0,
    AmbientLight=1,
    DirectionalLight=2,
    SpotLight=3,
    PointLight=4,
    HemisphereLight=5,
    RectAreaLight=6
};


class Light : public Object3D {
public:
    using sptr = std::shared_ptr<Light>;

    using LightShadowPtr= std::shared_ptr<LightShadow>;

    Color color{(unsigned)threecpp::Colors::white};

    Color groundColor{(unsigned)threecpp::Colors::blue};

    Color skyColor;

    float intensity=1.0f;

    float distance=0.0f;

    float angle=0.0f;

    float exponent=0.0f;

    float decay=0.0f;

    float penumbra=0.0f;

    LightShadow::sptr shadow = nullptr;

    Object3D::ptr target;

    SphericalHarmonics3 sh;

    float width=1;

    float height=1;

    LightType lightType;

    Vector3 direction;

    float coneCos = 0;


    Vector3 halfWidth;

    Vector3 halfHeight;


    Light();


    Light(const Color& color, float indensity = 1);

    Light(const Light& source);

    static sptr create() {
        return std::make_shared<Light>();
    }

    static sptr create(const Color& color, float indensity = 1) {
        return std::make_shared<Light>(color, indensity);
    }

    virtual Light& copy(const Light& source);

    virtual Light* clone() {
        return new Light(*this);
    }
    virtual float power() {
        return this->intensity;
    }
    virtual void setPower(float value) {
        this->intensity = value;
    }
};


//class Light:public Object3D{
//    public:
//        using LightType = threecpp::LightType;
//
//        Light(ColorSptr color,double intensity = 1.0):_color(color),_intensity(intensity){}
//
//        Light(const Light &light):_color(light.color()),_intensity(light.intensity()){}
//
//        ColorSptr color() const{return _color;}
//
//        bool isLight() const{return _isLight;}
//
//        void intensity(double intensity){_intensity=intensity;}
//
//        virtual LightType type() const {return _type;}
//
//        virtual double intensity() const {return _intensity;}
//
//        virtual double distance() const {}
//
//        virtual LightShadowSptr shadow();
//
//        virtual bool isAmbientLight() const {};
//
//        virtual bool isDirectionalLight() const {};
//
//        virtual bool isLightProbe() const {};
//
//        virtual bool isHemisphereLight() const {};
//
//    protected:
//        bool _isLight = true;
//        LightType _type = LightType::Light;
//        double _intensity = 1.0;
//        ColorSptr _color;
//
//
//};

//
//class Light extends Object3D {
//
//constructor( color, intensity = 1 ) {
//
//super();
//
//this.isLight = true;
//
//this.type = 'Light';
//
//this.color = new Color( color );
//this.intensity = intensity;
//
//}

//copy( source, recursive ) {
//
//super.copy( source, recursive );
//
//this.color.copy( source.color );
//this.intensity = source.intensity;
//
//return this;
//
//}
//
//toJSON( meta ) {
//
//        const data = super.toJSON( meta );
//
//        data.object.color = this.color.getHex();
//        data.object.intensity = this.intensity;
//
//        if ( this.groundColor !== undefined ) data.object.groundColor = this.groundColor.getHex();
//
//        if ( this.distance !== undefined ) data.object.distance = this.distance;
//        if ( this.angle !== undefined ) data.object.angle = this.angle;
//        if ( this.decay !== undefined ) data.object.decay = this.decay;
//        if ( this.penumbra !== undefined ) data.object.penumbra = this.penumbra;
//
//        if ( this.shadow !== undefined ) data.object.shadow = this.shadow.toJSON();
//
//        return data;
//
//}
//
//}



#endif //THREE_CPP_LIGHT_H
