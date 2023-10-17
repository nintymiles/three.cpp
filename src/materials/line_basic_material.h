//
// Created by SeanR on 2022/10/15.
//

#ifndef THREE_CPP_SRC_MATERIALS_LINE_BASIC_MATERIAL_H
#define THREE_CPP_SRC_MATERIALS_LINE_BASIC_MATERIAL_H

#include "material.h"
#include "color.h"

#include <string>

class LineBasicMaterial : public Material {
public:
    using sptr = std::shared_ptr<LineBasicMaterial>;

    LineBasicMaterial()  : Material() {
        type = "LineBasicMaterial";
        name = "LineBasicMaterial";
        shaderId = "basic";
        color.setHex(0xffffff);
        linewidth = 1;
        linecap = "round";
        linejoin = "round";
    }

    LineBasicMaterial(const LineBasicMaterial& source) : Material(source) {

        color.copy(source.color);

        linewidth = source.linewidth;
        linecap = source.linecap;
        linejoin = source.linejoin;
    }

    LineBasicMaterial(const Color& color) : LineBasicMaterial() {
        this->color.copy(color);
    }

    static sptr create() {
        return std::make_shared<LineBasicMaterial>();
    }

    static sptr create(const Color& color) {
        return std::make_shared<LineBasicMaterial>(color);
    }

    virtual LineBasicMaterial* clone() {
        return new LineBasicMaterial(*this);
    }

    virtual LineBasicMaterial& copy(const LineBasicMaterial& source) {
        Material::copy(source);

        color.copy(source.color);

        linewidth = source.linewidth;
        linecap = source.linecap;
        linejoin = source.linejoin;
        return *this;
    }

    virtual LineBasicMaterial& copy(const Material& source) override {
        Material::copy(source);
        type = "LineBasicMaterial";
        name = "LineBasicMaterial";
        shaderId = "basic";
        return *this;
    }
};

//class LineBasicMaterial : public Material{
//public:
//    std::string type = "LineBasicMaterial";
//    Color color{0xffffff};
//    double lineWidth = 1;
//    std::string linecap = "round";
//    std::string linejoin = "round";
//    bool isLineBasicMaterial = true;
//
//    LineBasicMaterial() = default;
//    LineBasicMaterial(const LineBasicMaterial& l): type(l.type),color(color),
//            lineWidth(lineWidth),linecap(linecap),linejoin(linejoin),isLineBasicMaterial(isLineBasicMaterial){}
//
//    LineBasicMaterial& copy(const LineBasicMaterial& l){
//        *this = l;
//
//        return *this;
//    }
//
//
//
//};
//
////this.setValues( parameters );


#endif //THREE_CPP_SRC_MATERIALS_LINE_BASIC_MATERIAL_H
