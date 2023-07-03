//
// Created by SeanRen on 2022/10/4.
//

#ifndef THREE_CPP_COLOR_MANAGEMENT_H
#define THREE_CPP_COLOR_MANAGEMENT_H

#include "constants.h"
#include <map>
#include <string>
#include <cmath>

class Color;
struct rgb_components;

inline double SRGBToLinear( double c ) {
    return ( c < 0.04045 ) ? c * 0.0773993808 : pow( c * 0.9478672986 + 0.0521327014, 2.4 );
}

inline double LinearToSRGB( double c ) {
    return ( c < 0.0031308 ) ? c * 12.92 : 1.055 * ( pow( c, 0.41666 ) ) - 0.055;
}

typedef double(*ColorCallBackType)(double);

//std::string LinearSRGBColorSpace2 = "srgb-linear";
// FN[InputColorSpace][OutputColorSpace] callback functions.
const std::map<std::string,ColorCallBackType> FN1{{LinearSRGBColorSpace, SRGBToLinear}};
const std::map<std::string,ColorCallBackType> FN2{{SRGBColorSpace, LinearToSRGB}};

const std::map<std::string,std::map<std::string,ColorCallBackType>> FN =
        {{SRGBColorSpace,FN1},
         {LinearSRGBColorSpace,FN2}
        };

class ColorManagement{
public:
    static bool legacyMode;

    static std::string workingColorSpace(){
        return LinearSRGBColorSpace;
    }

//    static ColorManagement& workingColorSpace(std::string colorSpace){
//        //cout << 'THREE.ColorManagement: .workingColorSpace is readonly.' << endl;
//    }

    static rgb_components& convert(rgb_components& rc,std::string sourceColorSpace,std::string targetColorSpace);

    static Color& fromWorkingColorSpace( Color& c, std::string targetColorSpace );
    static Color& toWorkingColorSpace( Color& c, std::string sourceColorSpace );

    static rgb_components& fromWorkingColorSpace( rgb_components& rc, std::string targetColorSpace ) {
        return convert( rc, workingColorSpace(), targetColorSpace );
    }

    static rgb_components& toWorkingColorSpace( rgb_components& rc, std::string sourceColorSpace ) {
        return convert( rc, sourceColorSpace, workingColorSpace() );
    }

};


#endif //THREE_CPP_COLOR_MANAGEMENT_H
