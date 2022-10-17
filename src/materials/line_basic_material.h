//
// Created by SeanR on 2022/10/15.
//

#ifndef THREE_CPP_SRC_MATERIALS_LINE_BASIC_MATERIAL_H
#define THREE_CPP_SRC_MATERIALS_LINE_BASIC_MATERIAL_H

#include "material.h"
#include "color.h"

#include <string>

class LineBasicMaterial : public Material{
public:
    std::string type = "LineBasicMaterial";
    Color color{0xffffff};
    double lineWidth = 1;
    std::string linecap = "round";
    std::string linejoin = "round";
    bool isLineBasicMaterial = true;

    LineBasicMaterial() = default;
    LineBasicMaterial(const LineBasicMaterial& l): type(l.type),color(color),
            lineWidth(lineWidth),linecap(linecap),linejoin(linejoin),isLineBasicMaterial(isLineBasicMaterial){}

    LineBasicMaterial& copy(const LineBasicMaterial& l){
        *this = l;

        return *this;
    }



};

//this.setValues( parameters );


#endif //THREE_CPP_SRC_MATERIALS_LINE_BASIC_MATERIAL_H
