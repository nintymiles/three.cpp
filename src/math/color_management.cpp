//
// Created by SeanRen on 2022/10/4.
//
#include "color_management.h"
#include "color.h"

bool ColorManagement::legacyMode = true;

Color& ColorManagement::convert(Color& c,std::string sourceColorSpace,std::string targetColorSpace){
    if(legacyMode || sourceColorSpace == targetColorSpace || sourceColorSpace=="" || targetColorSpace=="")
        return c;

    ColorCallBackType fn = FN.at(sourceColorSpace).at(targetColorSpace);
    c.r = fn(c.r);
    c.g = fn(c.g);
    c.b = fn(c.b);
    return c;
}

