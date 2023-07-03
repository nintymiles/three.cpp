//
// Created by SeanRen on 2022/10/4.
//
#include "color_management.h"
#include "color.h"

bool ColorManagement::legacyMode = true;

rgb_components& ColorManagement::convert(rgb_components& rc,std::string sourceColorSpace,std::string targetColorSpace){
    //Color c{rc.r,rc.g,rc.b};
    if(legacyMode || sourceColorSpace == targetColorSpace || sourceColorSpace=="" || targetColorSpace=="")
        return rc;

    ColorCallBackType fn = FN.at(sourceColorSpace).at(targetColorSpace);
    rc.r = fn(rc.r);
    rc.g = fn(rc.g);
    rc.b = fn(rc.b);
    return rc;
}

Color& ColorManagement::fromWorkingColorSpace( Color& c, std::string targetColorSpace ){
    rgb_components rgbComponents;
    toComponents(c,rgbComponents);
    c.setRGB(ColorManagement::fromWorkingColorSpace(rgbComponents,targetColorSpace));
    return  c;
}

Color& ColorManagement::toWorkingColorSpace( Color& c, std::string sourceColorSpace ){
    rgb_components rgbComponents;
    toComponents(c,rgbComponents);
    c.setRGB(ColorManagement::toWorkingColorSpace(rgbComponents,sourceColorSpace));
    return  c;
}

