//
// Created by SeanR on 2022/11/6.
//

#ifndef THREE_CPP_SRC_MATERIALS_LINE_DASH_MATERIAL_H
#define THREE_CPP_SRC_MATERIALS_LINE_DASH_MATERIAL_H

#include "line_basic_material.h"

#include <string>

class LineDashedMaterial : public LineBasicMaterial {
public:
    bool isLineDashedMaterial = true;
    std::string type = "LineDashedMaterial";

    double scale = 1;
    double dashSize = 3;
    double gapSize = 1;

    LineDashedMaterial()=default;

    LineDashedMaterial(const LineDashedMaterial& l):scale(l.scale),dashSize(l.dashSize),gapSize(l.gapSize){}

};
//constructor( parameters ) {
//
//    super();
//
//    this.type = 'LineDashedMaterial';
//
//    this.scale = 1;
//    this.dashSize = 3;
//    this.gapSize = 1;
//
//    this.setValues( parameters );
//
//}



#endif //THREE_CPP_SRC_MATERIALS_LINE_DASH_MATERIAL_H
