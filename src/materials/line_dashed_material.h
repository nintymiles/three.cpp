//
// Created by SeanR on 2022/11/6.
//

#ifndef THREE_CPP_SRC_MATERIALS_LINE_DASH_MATERIAL_H
#define THREE_CPP_SRC_MATERIALS_LINE_DASH_MATERIAL_H

#include "line_basic_material.h"

#include "color.h"

#include <string>

class LineDashedMaterial : public LineBasicMaterial {
    float scale = 1.0f;
    float dashSize = 2.0f;
    float gapSize = 1.0f;
public:
    LineDashedMaterial() : LineBasicMaterial(),scale(1), dashSize(3), gapSize(1) {
        this->type = "LineDashedMaterial";
        this->name = "LineDashedMaterial";
        shaderId = "dashed";
    }

    LineDashedMaterial(const Color& color) : LineDashedMaterial(){
        this->color.copy(color);
    }

    LineDashedMaterial(const LineDashedMaterial& source) :LineBasicMaterial(source) {
        this->type = "LineDashedMaterial";
        this->name = "LineDashedMaterial";
        shaderId = "dashed";
        scale = source.scale;
        dashSize = source.dashSize;
        gapSize = source.gapSize;
    }
    virtual ~LineDashedMaterial() = default;

    virtual LineDashedMaterial* clone() {
        return new LineDashedMaterial(*this);
    }
    virtual LineDashedMaterial& copy(const LineDashedMaterial& source) {
        LineBasicMaterial::copy(source);
        scale = source.scale;
        dashSize = source.dashSize;
        gapSize = source.gapSize;
    }

    static sptr create() {
        return std::make_shared<LineDashedMaterial>();
    }

    static sptr create(const Color& color) {
        return std::make_shared<LineDashedMaterial>(color);
    }
};

//class LineDashedMaterial : public LineBasicMaterial {
//public:
//    bool isLineDashedMaterial = true;
//    std::string type = "LineDashedMaterial";
//
//    double scale = 1;
//    double dashSize = 3;
//    double gapSize = 1;
//
//    LineDashedMaterial()=default;
//
//    LineDashedMaterial(const LineDashedMaterial& l):scale(l.scale),dashSize(l.dashSize),gapSize(l.gapSize){}
//
//};
////constructor( parameters ) {
////
////    super();
////
////    this.type = 'LineDashedMaterial';
////
////    this.scale = 1;
////    this.dashSize = 3;
////    this.gapSize = 1;
////
////    this.setValues( parameters );
////
////}


#endif //THREE_CPP_SRC_MATERIALS_LINE_DASH_MATERIAL_H
