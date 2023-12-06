//
// Created by Willie on 2022/10/4.
//

#ifndef THREE_CPP_COLOR_H
#define THREE_CPP_COLOR_H

#include <string>
#include <cmath>
#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>

#include "common_types.h"
#include "math_utils.h"

//enum class threecpp::Colors {
//    aliceblue = 0xf0f8ff,
//    antiquewhite = 0xfaebd7,
//    aqua = 0x00ffff,
//    aquamarine = 0x7fffd4,
//    azure = 0xf0ffff,
//    beige = 0xf5f5dc,
//    bisque = 0xffe4c4,
//    black = 0x000000,
//    blanchedalmond = 0xffebcd,
//    blue = 0x0000ff,
//    blueviolet = 0x8a2be2,
//    brown = 0xa52a2a,
//    burlywood = 0xdeb887,
//    cadetblue = 0x5f9ea0,
//    chartreuse = 0x7fff00,
//    chocolate = 0xd2691e,
//    coral = 0xff7f50,
//    cornflowerblue = 0x6495ed,
//    cornsilk = 0xfff8dc,
//    crimson = 0xdc143c,
//    cyan = 0x00ffff,
//    darkblue = 0x00008b,
//    darkcyan = 0x008b8b,
//    darkgoldenrod = 0xb8860b,
//    darkgray = 0xa9a9a9,
//    darkgreen = 0x006400,
//    darkgrey = 0xa9a9a9,
//    darkkhaki = 0xbdb76b,
//    darkmagenta = 0x8b008b,
//    darkolivegreen = 0x556b2f,
//    darkorange = 0xff8c00,
//    darkorchid = 0x9932cc,
//    darkred = 0x8b0000,
//    darksalmon = 0xe9967a,
//    darkseagreen = 0x8fbc8f,
//    darkslateblue = 0x483d8b,
//    darkslategray = 0x2f4f4f,
//    darkslategrey = 0x2f4f4f,
//    darkturquoise = 0x00ced1,
//    darkviolet = 0x9400d3,
//    deeppink = 0xff1493,
//    deepskyblue = 0x00bfff,
//    dimgray = 0x696969,
//    dimgrey = 0x696969,
//    dodgerblue = 0x1e90ff,
//    firebrick = 0xb22222,
//    floralwhite = 0xfffaf0,
//    forestgreen = 0x228b22,
//    fuchsia = 0xff00ff,
//    gainsboro = 0xdcdcdc,
//    ghostwhite = 0xf8f8ff,
//    gold = 0xffd700,
//    goldenrod = 0xdaa520,
//    gray = 0x808080,
//    green = 0x008000,
//    greenyellow = 0xadff2f,
//    grey = 0x808080,
//    honeydew = 0xf0fff0,
//    hotpink = 0xff69b4,
//    indianred = 0xcd5c5c,
//    indigo = 0x4b0082,
//    ivory = 0xfffff0,
//    khaki = 0xf0e68c,
//    lavender = 0xe6e6fa,
//    lavenderblush = 0xfff0f5,
//    lawngreen = 0x7cfc00,
//    lemonchiffon = 0xfffacd,
//    lightblue = 0xadd8e6,
//    lightcoral = 0xf08080,
//    lightcyan = 0xe0ffff,
//    lightgoldenrodyellow = 0xfafad2,
//    lightgray = 0xd3d3d3,
//    lightgreen = 0x90ee90,
//    lightgrey = 0xd3d3d3,
//    lightpink = 0xffb6c1,
//    lightsalmon = 0xffa07a,
//    lightseagreen = 0x20b2aa,
//    lightskyblue = 0x87cefa,
//    lightslategray = 0x778899,
//    lightslategrey = 0x778899,
//    lightsteelblue = 0xb0c4de,
//    lightyellow = 0xffffe0,
//    lime = 0x00ff00,
//    limegreen = 0x32cd32,
//    linen = 0xfaf0e6,
//    magenta = 0xff00ff,
//    maroon = 0x800000,
//    mediumaquamarine = 0x66cdaa,
//    mediumblue = 0x0000cd,
//    mediumorchid = 0xba55d3,
//    mediumpurple = 0x9370db,
//    mediumseagreen = 0x3cb371,
//    mediumslateblue = 0x7b68ee,
//    mediumspringgreen = 0x00fa9a,
//    mediumturquoise = 0x48d1cc,
//    mediumvioletred = 0xc71585,
//    midnightblue = 0x191970,
//    mintcream = 0xf5fffa,
//    mistyrose = 0xffe4e1,
//    moccasin = 0xffe4b5,
//    navajowhite = 0xffdead,
//    navy = 0x000080,
//    oldlace = 0xfdf5e6,
//    olive = 0x808000,
//    olivedrab = 0x6b8e23,
//    orange = 0xffa500,
//    orangered = 0xff4500,
//    orchid = 0xda70d6,
//    palegoldenrod = 0xeee8aa,
//    palegreen = 0x98fb98,
//    paleturquoise = 0xafeeee,
//    palevioletred = 0xdb7093,
//    papayawhip = 0xffefd5,
//    peachpuff = 0xffdab9,
//    peru = 0xcd853f,
//    pink = 0xffc0cb,
//    plum = 0xdda0dd,
//    powderblue = 0xb0e0e6,
//    purple = 0x800080,
//    rebeccapurple = 0x663399,
//    red = 0xff0000,
//    rosybrown = 0xbc8f8f,
//    royalblue = 0x4169e1,
//    saddlebrown = 0x8b4513,
//    salmon = 0xfa8072,
//    sandybrown = 0xf4a460,
//    seagreen = 0x2e8b57,
//    seashell = 0xfff5ee,
//    sienna = 0xa0522d,
//    silver = 0xc0c0c0,
//    skyblue = 0x87ceeb,
//    slateblue = 0x6a5acd,
//    slategray = 0x708090,
//    slategrey = 0x708090,
//    snow = 0xfffafa,
//    springgreen = 0x00ff7f,
//    steelblue = 0x4682b4,
//    tan = 0xd2b48c,
//    teal = 0x008080,
//    thistle = 0xd8bfd8,
//    tomato = 0xff6347,
//    turquoise = 0x40e0d0,
//    violet = 0xee82ee,
//    wheat = 0xf5deb3,
//    white = 0xffffff,
//    whitesmoke = 0xf5f5f5,
//    yellow = 0xffff00,
//    yellowgreen = 0x9acd32,
//};
inline float hue2rgb(float p, float q, float t) {
    if (t < 0) t += 1;
    if (t > 1) t -= 1;
    if (t < 1.0f / 6.0f) return p + (q - p) * 6 * t;
    if (t < 1.0f / 2.0f) return q;
    if (t < 2.0f / 3.0f) return p + (q - p) * 6 * (2.0f / 3.0f - t);

    return p;
}

inline float SRGBToLinear(float c)
{
    return (c < 0.04045f) ? c * 0.0773993808f : std::pow(c * 0.9478672986f + 0.0521327014f, 2.4f);
}

inline float LinearToSRGB(float c) {
    return (c < 0.0031308f) ? c * 12.92f : 1.055f * std::pow(c, 0.41666f) - 0.055f;
}


typedef struct _HSL {
    union {
        struct {
            float h, s, l;
        };
        float elements[3];
    };
} HSL;

class Color {
public:
    union {
        struct {
            float r, g, b;
        };
        float elements[3];
    };

    using ptr = std::shared_ptr<Color>;

    Color() {
        r = std::numeric_limits<float>::max();
        g = std::numeric_limits<float>::max();
        b = std::numeric_limits<float>::max();
    }

    Color(float r, float g, float b) : r(r), g(g), b(b) {}
    Color(const Color& color) : r(color.r), g(color.g), b(color.b) {}
    Color(unsigned hex) : r((hex >> 16 & 255) / 255.0f), g((hex >> 8 & 255) / 255.0f), b((hex & 255) / 255.0f) {}

    Color(threecpp::Colors name) : Color((unsigned)name) {}

    Color& operator = (const Color& source) {
        return copy(source);
    }
    bool operator == (const Color& source) const {
        return r == source.r && g == source.g && b == source.b;
    }
    bool isNull() const {
        return r == std::numeric_limits<float>::max() ||
               g == std::numeric_limits<float>::max() ||
               b == std::numeric_limits<float>::max();
    }
    Color& set(Color& value) {
        copy(value);
        return *this;
    }
    Color& set(unsigned value) {
        setHex(value);
        return *this;
    }

    Color& set(threecpp::Colors name) {
        setHex((unsigned)name);
        return *this;
    }
    Color& setScalar(float scalar) {

        r = scalar;
        g = scalar;
        b = scalar;

        return *this;

    }

    Color& setHex(unsigned hex) {

        hex = (unsigned)std::floor(hex);

        r = (hex >> 16 & 255) / 255.0f;
        g = (hex >> 8 & 255) / 255.0f;
        b = (hex & 255) / 255.0f;

        return *this;

    }

    Color& setRGB(float r, float g, float b) {

        this->r = r;
        this->g = g;
        this->b = b;

        return *this;

    }

    Color& setHSL(float h, float s, float l) {
        // h,s,l ranges are in 0.0 - 1.0
        h = math::euclideanModulo(h, 1);
        s = math::clamp<float>(s, 0.0f, 1.0f);
        l = math::clamp<float>(l, 0.0f, 1.0f);

        if (s ==  0) {

            r = g = b = l;

        } else {

            auto p = l <= 0.5f ? l * (1 + s) : l + s - (l * s);
            auto q = (2 * l) - p;

            r = hue2rgb(q, p, h + 1 / 3.0f);
            g = hue2rgb(q, p, h);
            b = hue2rgb(q, p, h - 1 / 3.0f);

        }

        return *this;

    }

    Color& setColorName(threecpp::Colors style) {
        setHex((unsigned)style);
        return *this;
    }

    Color& clone(){

        return *this;
    }

    Color& copy(const Color& color) {

        r = color.r;
        g = color.g;
        b = color.b;

        return *this;

    }

    Color& copyGammaToLinear(const Color& color, float gammaFactor=0) {

        if (gammaFactor == 0) gammaFactor = 2.0f;

        r = (float)std::pow(color.r, gammaFactor);
        g = (float)std::pow(color.g, gammaFactor);
        b = (float)std::pow(color.b, gammaFactor);

        return *this;
    }

    Color& copyLinearToGamma(const Color& color, float gammaFactor=0) {

        if (gammaFactor == 0) gammaFactor = 2.0f;

        float safeInverse = (gammaFactor > 0) ? (1.0f / gammaFactor) : 1.0f;

        r = std::pow(color.r, safeInverse);
        g = std::pow(color.g, safeInverse);
        b = std::pow(color.b, safeInverse);

        return *this;

    }

    Color& convertGammaToLinear(float gammaFactor=0) {

        copyGammaToLinear(*this, gammaFactor);

        return *this;

    }

    Color& convertLinearToGamma(float gammaFactor=0) {

        copyLinearToGamma(*this, gammaFactor);

        return *this;

    }

    Color& copySRGBToLinear(const Color& color) {

        r = SRGBToLinear(color.r);
        g = SRGBToLinear(color.g);
        b = SRGBToLinear(color.b);

        return *this;

    }

    Color& copyLinearToSRGB(const Color& color) {

        r = LinearToSRGB(color.r);
        g = LinearToSRGB(color.g);
        b = LinearToSRGB(color.b);

        return *this;

    }

    Color& convertSRGBToLinear() {

        copySRGBToLinear(*this);

        return *this;

    }

    Color& convertLinearToSRGB() {

        copyLinearToSRGB(*this);

        return *this;

    }

    unsigned getHex() {

        return (unsigned)(r * 255.0f) << 16 ^ (unsigned)(g * 255.0f) << 8 ^ (unsigned)(b * 255.0f) << 0;

    }

    std::string getHexString() {

        std::stringstream stream;
        stream << "0x" << std::setfill('0') << std::setw(6) << std::hex << getHex();

        return stream.str();
    }

    void getHSL(HSL* target) const {

        // h,s,l ranges are in 0.0 - 1.0



        const float r = this->r, g = this->g, b = this->b;

        float max = std::max(r, std::max(g, b));
        float min = std::min(r, std::min(g, b));

        float hue = 0, saturation=0;
        float lightness = (min + max) / 2.0f;

        if (min ==  max) {

            hue = 0;
            saturation = 0;

        }
        else {

            float delta = max - min;

            saturation = lightness <= 0.5f ? delta / (max + min) : delta / (2 - max - min);

            if(max==r)
                hue = (g - b) / delta + (g < b ? 6 : 0);
            else if(max==g)
                hue = (b - r) / delta + 2;
            else if(max==b)
                hue = (r - g) / delta + 4;

            hue /= 6;
        }

        target->h = hue;
        target->s = saturation;
        target->l = lightness;



    }

    Color& offsetHSL(float h, float s, float l) {
        HSL _hslA;
        getHSL(&_hslA);

        _hslA.h += h; _hslA.s += s; _hslA.l += l;

        setHSL(_hslA.h, _hslA.s, _hslA.l);

        return *this;

    }

    Color& add(Color& color) {

        r += color.r;
        g += color.g;
        b += color.b;

        return *this;

    }

    Color& addColors(Color& color1, Color& color2) {

        r = color1.r + color2.r;
        g = color1.g + color2.g;
        b = color1.b + color2.b;

        return *this;

    }

    Color& addScalar(float s) {

        r += s;
        g += s;
        b += s;

        return *this;

    }

    Color& sub(const Color& color) {

        r = std::max(0.0f, r - color.r);
        g = std::max(0.0f, g - color.g);
        b = std::max(0.0f, b - color.b);

        return *this;

    }

    Color& multiply(const Color&color) {

        r *= color.r;
        g *= color.g;
        b *= color.b;

        return *this;

    }

    Color& multiplyScalar(float s) {

        r *= s;
        g *= s;
        b *= s;

        return *this;

    }

    Color& lerp(const Color& color, float alpha) {

        r += (color.r - r) * alpha;
        g += (color.g - g) * alpha;
        b += (color.b - b) * alpha;

        return *this;

    }

    Color& lerpHSL(const Color& color, float alpha) {
        HSL _hslA,_hslB;
        getHSL(&_hslA);
        color.getHSL(&_hslB);

        float h = math::lerp(_hslA.h, _hslB.h, alpha);
        float s = math::lerp(_hslA.s, _hslB.s, alpha);
        float l = math::lerp(_hslA.l, _hslB.l, alpha);

        setHSL(h, s, l);

        return *this;

    }

    bool equals(const Color& c) {

        return (c.r ==  r) && (c.g ==  g) && (c.b ==  b);

    }

    Color& fromArray(const std::vector<float>& array, unsigned offset = 0) {

        unsigned arrayLength = array.size();
        r = offset < arrayLength ? array[offset] : std::numeric_limits<float>::quiet_NaN();
        g = offset < arrayLength ? array[offset + 1] : std::numeric_limits<float>::quiet_NaN();
        b = offset < arrayLength ? array[offset + 2] : std::numeric_limits<float>::quiet_NaN();

        return *this;

    }

    Color& fromArray(const float* array, unsigned arrayLength,unsigned offset=0) {
        r = offset < arrayLength ? array[offset] : std::numeric_limits<float>::quiet_NaN();
        g = offset < arrayLength ? array[offset + 1] : std::numeric_limits<float>::quiet_NaN();
        b = offset < arrayLength ? array[offset + 2] : std::numeric_limits<float>::quiet_NaN();

        return *this;

    }
    std::string to_string() {
        return getHexString();
    }
    Color& toArray(float* array, unsigned offset=0) {
        array[offset] = r;
        array[offset + 1] = g;
        array[offset + 2] = b;

        return *this;
    }
    Color& appendToVector(std::vector<float>& arrayVec) {
        arrayVec.push_back(r);
        arrayVec.push_back(g);
        arrayVec.push_back(b);

        return *this;
    }
};
//
//#include <unordered_map>
//#include <string>
//#include <cmath>
//#include <locale>
//#include <iostream>
//#include <sstream>
//#include <bitset>
//#include <algorithm>
//
//#include "constants.h"
//#include "math_utils.h"
//#include "color_management.h"
//
//////named colors map
////const std::map<std::string,int> _threecpp::Colors = {{"aliceblue",0xF0F8FF},{"antiquewhite", 0xFAEBD7}, {"aqua", 0x00FFFF}, {"aquamarine", 0x7FFFD4}, {"azure", 0xF0FFFF},
////{"beige", 0xF5F5DC}, {"bisque", 0xFFE4C4}, {"black", 0x000000}, {"blanchedalmond", 0xFFEBCD}, {"blue", 0x0000FF}, {"blueviolet", 0x8A2BE2},
////{"brown", 0xA52A2A}, {"burlywood", 0xDEB887}, {"cadetblue", 0x5F9EA0}, {"chartreuse", 0x7FFF00}, {"chocolate", 0xD2691E}, {"coral", 0xFF7F50},
////{"cornflowerblue", 0x6495ED}, {"cornsilk", 0xFFF8DC}, {"crimson", 0xDC143C}, {"cyan", 0x00FFFF}, {"darkblue", 0x00008B}, {"darkcyan", 0x008B8B},
////{"darkgoldenrod", 0xB8860B}, {"darkgray", 0xA9A9A9}, {"darkgreen", 0x006400}, {"darkgrey", 0xA9A9A9}, {"darkkhaki", 0xBDB76B}, {"darkmagenta", 0x8B008B},
////{"darkolivegreen", 0x556B2F}, {"darkorange", 0xFF8C00}, {"darkorchid", 0x9932CC}, {"darkred", 0x8B0000}, {"darksalmon", 0xE9967A}, {"darkseagreen", 0x8FBC8F},
////{"darkslateblue", 0x483D8B}, {"darkslategray", 0x2F4F4F}, {"darkslategrey", 0x2F4F4F}, {"darkturquoise", 0x00CED1}, {"darkviolet", 0x9400D3},
////{"deeppink", 0xFF1493}, {"deepskyblue", 0x00BFFF}, {"dimgray", 0x696969}, {"dimgrey", 0x696969}, {"dodgerblue", 0x1E90FF}, {"firebrick", 0xB22222},
////{"floralwhite", 0xFFFAF0}, {"forestgreen", 0x228B22}, {"fuchsia", 0xFF00FF}, {"gainsboro", 0xDCDCDC}, {"ghostwhite", 0xF8F8FF}, {"gold", 0xFFD700},
////{"goldenrod", 0xDAA520}, {"gray", 0x808080}, {"green", 0x008000}, {"greenyellow", 0xADFF2F}, {"grey", 0x808080}, {"honeydew", 0xF0FFF0}, {"hotpink", 0xFF69B4},
////{"indianred", 0xCD5C5C}, {"indigo", 0x4B0082}, {"ivory", 0xFFFFF0}, {"khaki", 0xF0E68C}, {"lavender", 0xE6E6FA}, {"lavenderblush", 0xFFF0F5}, {"lawngreen", 0x7CFC00},
////{"lemonchiffon", 0xFFFACD}, {"lightblue", 0xADD8E6}, {"lightcoral", 0xF08080}, {"lightcyan", 0xE0FFFF}, {"lightgoldenrodyellow", 0xFAFAD2}, {"lightgray", 0xD3D3D3},
////{"lightgreen", 0x90EE90}, {"lightgrey", 0xD3D3D3}, {"lightpink", 0xFFB6C1}, {"lightsalmon", 0xFFA07A}, {"lightseagreen", 0x20B2AA}, {"lightskyblue", 0x87CEFA},
////{"lightslategray", 0x778899}, {"lightslategrey", 0x778899}, {"lightsteelblue", 0xB0C4DE}, {"lightyellow", 0xFFFFE0}, {"lime", 0x00FF00}, {"limegreen", 0x32CD32},
////{"linen", 0xFAF0E6}, {"magenta", 0xFF00FF}, {"maroon", 0x800000}, {"mediumaquamarine", 0x66CDAA}, {"mediumblue", 0x0000CD}, {"mediumorchid", 0xBA55D3},
////{"mediumpurple", 0x9370DB}, {"mediumseagreen", 0x3CB371}, {"mediumslateblue", 0x7B68EE}, {"mediumspringgreen", 0x00FA9A}, {"mediumturquoise", 0x48D1CC},
////{"mediumvioletred", 0xC71585}, {"midnightblue", 0x191970}, {"mintcream", 0xF5FFFA}, {"mistyrose", 0xFFE4E1}, {"moccasin", 0xFFE4B5}, {"navajowhite", 0xFFDEAD},
////{"navy", 0x000080}, {"oldlace", 0xFDF5E6}, {"olive", 0x808000}, {"olivedrab", 0x6B8E23}, {"orange", 0xFFA500}, {"orangered", 0xFF4500}, {"orchid", 0xDA70D6},
////{"palegoldenrod", 0xEEE8AA}, {"palegreen", 0x98FB98}, {"paleturquoise", 0xAFEEEE}, {"palevioletred", 0xDB7093}, {"papayawhip", 0xFFEFD5}, {"peachpuff", 0xFFDAB9},
////{"peru", 0xCD853F}, {"pink", 0xFFC0CB}, {"plum", 0xDDA0DD}, {"powderblue", 0xB0E0E6}, {"purple", 0x800080}, {"rebeccapurple", 0x663399}, {"red", 0xFF0000}, {"rosybrown", 0xBC8F8F},
////{"royalblue", 0x4169E1}, {"saddlebrown", 0x8B4513}, {"salmon", 0xFA8072}, {"sandybrown", 0xF4A460}, {"seagreen", 0x2E8B57}, {"seashell", 0xFFF5EE},
////{"sienna", 0xA0522D}, {"silver", 0xC0C0C0}, {"skyblue", 0x87CEEB}, {"slateblue", 0x6A5ACD}, {"slategray", 0x708090}, {"slategrey", 0x708090}, {"snow", 0xFFFAFA},
////{"springgreen", 0x00FF7F}, {"steelblue", 0x4682B4}, {"tan", 0xD2B48C}, {"teal", 0x008080}, {"thistle", 0xD8BFD8}, {"tomato", 0xFF6347}, {"turquoise", 0x40E0D0},
////{"violet", 0xEE82EE}, {"wheat", 0xF5DEB3}, {"white", 0xFFFFFF}, {"whitesmoke", 0xF5F5F5}, {"yellow", 0xFFFF00}, {"yellowgreen", 0x9ACD32 }};
//
//struct rgb_components{float r,g,b;};
//struct hsl_components{double h,s,l;};
//
////std::shared_ptr<hsl_components> _hslB = std::make_shared<hsl_components>();
//
//class Color;
//double hue2rgb(double p,double q,double t);
//rgb_components& toComponents( Color& source, rgb_components& target );
//
//class Color {
//public:
//    //named colors const map reference
//    static const std::unordered_map<std::string,int>& NAMED_COLORS;
//    float r=1.0,g=1.0,b=1.0;
//    bool isColor = true;
//
//    //Color()=default;
//
//    Color(float r=1,float g=1,float b=1):r(r),g(g),b(b){};
//
//    Color(const Color& c):r(c.r),g(c.g),b(c.b){};
//
//    Color(unsigned hex) : r((hex >> 16 & 255) / 255.0f), g((hex >> 8 & 255) / 255.0f), b((hex & 255) / 255.0f) {}
//
//    Color& setScalar( double scalar ) {
//       r = scalar;
//       g = scalar;
//       b = scalar;
//
//       return *this;
//    }
//
//    Color& setHex( double hex, std::string colorSpace = SRGBColorSpace ) {
//        long fhex = (long)std::floor( hex );
//
//        r = ( fhex >> 16 & 255 ) / 255.0;
//        g = ( fhex >> 8 & 255 ) / 255.0;
//        b = (fhex & 255 ) / 255.0;
//
//        ColorManagement::toWorkingColorSpace( *this, colorSpace );
//        return *this;
//    }
//
//    Color& setHex(int hex,std::string colorSpace = SRGBColorSpace){
//        r = ( hex >> 16 & 255 ) / 255.0;
//        g = ( hex >> 8 & 255 ) / 255.0;
//        b = ( hex & 255 ) / 255.0;
//
//        ColorManagement::toWorkingColorSpace( *this, colorSpace );
//        return *this;
//    }
//
//
//    Color& setRGB( double r, double g, double b, std::string colorSpace = LinearSRGBColorSpace) {
//       this->r = r;
//       this->g = g;
//       this->b = b;
//
//       ColorManagement::toWorkingColorSpace( *this, colorSpace );
//       return *this;
//    }
//
//    Color& setRGB( rgb_components& rc) {
//        this->r = rc.r;
//        this->g = rc.g;
//        this->b = rc.b;
//
//        return *this;
//    }
//
//    Color& setHSL(double h, double s, double l,std::string colorSpace = LinearSRGBColorSpace) {
//        // h,s,l ranges are in 0.0 - 1.0
//        double hue = MathUtils::euclideanModulo<double>( h, 1 );
//        double saturate = MathUtils::clamp<double>( s, 0, 1 );
//        double light = MathUtils::clamp<double>( l, 0, 1 );
//
//        if ( saturate == 0 ) {
//           this->r = this->g = this->b = light;
//        } else {
//           const double p = light <= 0.5 ? light * ( 1 + saturate ) : light + saturate - ( light * saturate );
//           const double q = ( 2 * light ) - p;
//
//           this->r = hue2rgb( q, p, hue + 1 / 3 );
//           this->g = hue2rgb( q, p, hue );
//           this->b = hue2rgb( q, p, hue - 1 / 3 );
//        }
//
//        ColorManagement::toWorkingColorSpace( *this, colorSpace );
//        return *this;
//    }
//
//    Color& setColorName( std::string colorName, std::string colorSpace = SRGBColorSpace ) {
//        std::string lowerName = colorName;
//        //cpp标准库中没有string.toLower函数，需要逐字符转换
//        for(auto& c : lowerName){
//            c = tolower(c);
//        }
//        if(NAMED_COLORS.find(lowerName)!=NAMED_COLORS.end()) {
//            // color keywords
//            const int hex = NAMED_COLORS.at(lowerName);
//
//            setHex(hex, colorSpace);
//        }
//
//        return *this;
//    }
//
//    Color clone() {
//        return *this;
//    }
//
//    Color& copy( const Color& color ) {
//        this->r = color.r;
//        this->g = color.g;
//        this->b = color.b;
//
//        return *this;
//    }
//
//    Color& copySRGBToLinear( const Color& color ) {
//        this->r = SRGBToLinear( color.r );
//        this->g = SRGBToLinear( color.g );
//        this->b = SRGBToLinear( color.b );
//
//        return *this;
//    }
//
//    Color& copyLinearToSRGB( const Color& color ) {
//        this->r = LinearToSRGB( color.r );
//        this->g = LinearToSRGB( color.g );
//        this->b = LinearToSRGB( color.b );
//
//        return *this;
//    }
//
//    Color& convertSRGBToLinear() {
//        copySRGBToLinear( *this );
//
//        return *this;
//    }
//
//    Color& convertLinearToSRGB() {
//        copyLinearToSRGB( *this );
//
//        return *this;
//    }
//
//    int getHex( std::string colorSpace = SRGBColorSpace );
//
//    std::string getHexString( std::string colorSpace = SRGBColorSpace ) {
//        //return ( '000000' + getHex( colorSpace ).toString( 16 ) ).slice( - 6 );
//        std::stringstream ss;
//        ss << std::hex << getHex(colorSpace) ;
//        return  ss.str();
//    }
//
//    hsl_components& getHSL(hsl_components& target, std::string colorSpace = ColorManagement::workingColorSpace());
//
//    rgb_components& getRGB(rgb_components& target, std::string colorSpace = LinearSRGBColorSpace);
//
//
//    Color& multiplyScalar( double s ) {
//            this->r *= s;
//            this->g *= s;
//            this->b *= s;
//
//            return *this;
//    }
//
//
//    Color& multiply( const Color& color ) {
//
//            this->r *= color.r;
//            this->g *= color.g;
//            this->b *= color.b;
//
//            return *this;
//    }
//
//    Color& offsetHSL(double h, double s, double l);
//
//    Color& add( const Color& color ) {
//        this->r += color.r;
//        this->g += color.g;
//        this->b += color.b;
//
//        return *this;
//    }
//
//    Color& addColors( const Color& color1, const Color& color2 ) {
//        this->r = color1.r + color2.r;
//        this->g = color1.g + color2.g;
//        this->b = color1.b + color2.b;
//
//        return *this;
//    }
//
//    Color& addScalar( int s ) {
//        this->r += s;
//        this->g += s;
//        this->b += s;
//
//        return *this;
//    }
//
//    Color& sub( const Color& color ) {
//        this->r = std::max<int>( 0, this->r - color.r );
//        this->g = std::max<int>( 0, this->g - color.g );
//        this->b = std::max<int>( 0, this->b - color.b );
//
//        return *this;
//    }
//
//    Color& lerp( const Color& color, double alpha ) {
//        this->r += ( color.r - this->r ) * alpha;
//        this->g += ( color.g - this->g ) * alpha;
//        this->b += ( color.b - this->b ) * alpha;
//
//        return *this;
//
//    }
//
//    Color& lerpColors( const Color& color1, const Color& color2, double alpha ) {
//        this->r = color1.r + ( color2.r - color1.r ) * alpha;
//        this->g = color1.g + ( color2.g - color1.g ) * alpha;
//        this->b = color1.b + ( color2.b - color1.b ) * alpha;
//
//        return *this;
//    }
//
//    Color& lerpHSL(Color& color, double alpha);
//
//    bool equals( const Color& c ) {
//        return ( c.r == r ) && ( c.g == g ) && ( c.b == b );
//    }
//
//    Color& fromArray( double* array, int offset = 0 ) {
//        this->r = array[ offset ];
//        this->g = array[ offset + 1 ];
//        this->b = array[ offset + 2 ];
//
//        return *this;
//    }
//
//    double* toArray( double* array, int offset = 0 ) {
//        array[ offset ] = this->r;
//        array[ offset + 1 ] = this->g;
//        array[ offset + 2 ] = this->b;
//
//        return array;
//    }
//
//    Color& operator = (const Color& source) {
//        return copy(source);
//    }
//
//    bool operator == (const Color& source) const {
//        return r == source.r && g == source.g && b == source.b;
//    }
//
//    bool isNull() const {
//        return r == std::numeric_limits<float>::infinity() ||
//               g == std::numeric_limits<float>::infinity() ||
//               b == std::numeric_limits<float>::infinity();
//    }
//
//
//};
//
////所有较大对象先皆用shared_ptr,以type alias简化。根据情形辅以unique_ptr。
////针对小型对象，则以值传递为主
////对象内传递，则以reference为主
//using ColorSptr = std::shared_ptr<Color>;
//
//inline rgb_components& toComponents(Color& source,rgb_components& target){
//    target.r = source.r;
//    target.g = source.g;
//    target.b = source.b;
//
//    return target;
//};
//
//inline double hue2rgb(double p,double q,double t ) {
//    if ( t < 0 ) t += 1;
//    if ( t > 1 ) t -= 1;
//    if ( t < 1 / 6 ) return p + ( q - p ) * 6 * t;
//    if ( t < 1 / 2 ) return q;
//    if ( t < 2 / 3 ) return p + ( q - p ) * 6 * ( 2 / 3 - t );
//    return p;
//}
//
//
//// 使用variant作为参数，variant是C++17的元素
////    set( value ) {
////
////            if ( value && value.isColor ) {
////
////                this.copy( value );
////
////            } else if ( typeof value === 'number' ) {
////
////                this.setHex( value );
////
////            } else if ( typeof value === 'string' ) {
////
////                this.setStyle( value );
////
////            }
////
////            return this;
////
////    }
////
////    setStyle( style, colorSpace = SRGBColorSpace ) {
////
////        function handleAlpha( string ) {
////
////            if ( string === undefined ) return;
////
////            if ( parseFloat( string ) < 1 ) {
////
////                console.warn( 'THREE.Color: Alpha component of ' + style + ' will be ignored.' );
////
////            }
////
////        }
////
////
////        let m;
////
////        if ( m = /^((?:rgb|hsl)a?)\(([^\)]*)\)/.exec( style ) ) {
////
////            // rgb / hsl
////
////            let color;
////            const name = m[ 1 ];
////            const components = m[ 2 ];
////
////            switch ( name ) {
////
////                case 'rgb':
////                case 'rgba':
////
////                    if ( color = /^\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*(?:,\s*(\d*\.?\d+)\s*)?$/.exec( components ) ) {
////
////                // rgb(255,0,0) rgba(255,0,0,0.5)
////                this.r = Math.min( 255, parseInt( color[ 1 ], 10 ) ) / 255;
////                this.g = Math.min( 255, parseInt( color[ 2 ], 10 ) ) / 255;
////                this.b = Math.min( 255, parseInt( color[ 3 ], 10 ) ) / 255;
////
////                ColorManagement.toWorkingColorSpace( this, colorSpace );
////
////                handleAlpha( color[ 4 ] );
////
////                return this;
////
////            }
////
////                    if ( color = /^\s*(\d+)\%\s*,\s*(\d+)\%\s*,\s*(\d+)\%\s*(?:,\s*(\d*\.?\d+)\s*)?$/.exec( components ) ) {
////
////                // rgb(100%,0%,0%) rgba(100%,0%,0%,0.5)
////                this.r = Math.min( 100, parseInt( color[ 1 ], 10 ) ) / 100;
////                this.g = Math.min( 100, parseInt( color[ 2 ], 10 ) ) / 100;
////                this.b = Math.min( 100, parseInt( color[ 3 ], 10 ) ) / 100;
////
////                ColorManagement.toWorkingColorSpace( this, colorSpace );
////
////                handleAlpha( color[ 4 ] );
////
////                return this;
////
////            }
////
////                    break;
////
////                case 'hsl':
////                case 'hsla':
////
////                    if ( color = /^\s*(\d*\.?\d+)\s*,\s*(\d*\.?\d+)\%\s*,\s*(\d*\.?\d+)\%\s*(?:,\s*(\d*\.?\d+)\s*)?$/.exec( components ) ) {
////
////                // hsl(120,50%,50%) hsla(120,50%,50%,0.5)
////                const h = parseFloat( color[ 1 ] ) / 360;
////                const s = parseFloat( color[ 2 ] ) / 100;
////                const l = parseFloat( color[ 3 ] ) / 100;
////
////                handleAlpha( color[ 4 ] );
////
////                return this.setHSL( h, s, l, colorSpace );
////
////            }
////
////                    break;
////
////            }
////
////        } else if ( m = /^\#([A-Fa-f\d]+)$/.exec( style ) ) {
////
////            // hex color
////
////            const hex = m[ 1 ];
////            const size = hex.length;
////
////            if ( size === 3 ) {
////
////                // #ff0
////                this.r = parseInt( hex.charAt( 0 ) + hex.charAt( 0 ), 16 ) / 255;
////                this.g = parseInt( hex.charAt( 1 ) + hex.charAt( 1 ), 16 ) / 255;
////                this.b = parseInt( hex.charAt( 2 ) + hex.charAt( 2 ), 16 ) / 255;
////
////                ColorManagement.toWorkingColorSpace( this, colorSpace );
////
////                return this;
////
////            } else if ( size === 6 ) {
////
////                // #ff0000
////                this.r = parseInt( hex.charAt( 0 ) + hex.charAt( 1 ), 16 ) / 255;
////                this.g = parseInt( hex.charAt( 2 ) + hex.charAt( 3 ), 16 ) / 255;
////                this.b = parseInt( hex.charAt( 4 ) + hex.charAt( 5 ), 16 ) / 255;
////
////                ColorManagement.toWorkingColorSpace( this, colorSpace );
////
////                return this;
////
////            }
////
////        }
////
////        if ( style && style.length > 0 ) {
////
////            return this.setColorName( style, colorSpace );
////
////        }
////
////        return this;
////
////    }
//
//
//
////    getStyle( colorSpace = SRGBColorSpace ) {
////
////        ColorManagement.fromWorkingColorSpace( toComponents( this, _rgb ), colorSpace );
////
////        if ( colorSpace !== SRGBColorSpace ) {
////
////            // Requires CSS Color Module Level 4 (https://www.w3.org/TR/css-color-4/).
////            return `color(${ colorSpace } ${ _rgb.r } ${ _rgb.g } ${ _rgb.b })`;
////
////        }
////
////        return `rgb(${( _rgb.r * 255 ) | 0},${( _rgb.g * 255 ) | 0},${( _rgb.b * 255 ) | 0})`;
////
////    }
//
//
////    fromBufferAttribute( attribute, index ) {
////
////        this.r = attribute.getX( index );
////        this.g = attribute.getY( index );
////        this.b = attribute.getZ( index );
////
////        return this;
////
////    }
////
////    toJSON() {
////
////        return this.getHex();
////
////    }
////
////    *[ Symbol.iterator ]() {
////
////        yield this.r;
////        yield this.g;
////        yield this.b;
////
////    }
////
////}
////
////        Color.NAMES = _threecpp::Colors;


#endif //THREE_CPP_COLOR_H
