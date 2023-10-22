//
// Created by Willie on 2022/10/4.
//

#include "color.h"

namespace __color {
    //named colors map
    const std::unordered_map<std::string, int> _colorKeywords =
    { {"aliceblue",            0xF0F8FF},
     {"antiquewhite",         0xFAEBD7},
     {"aqua",                 0x00FFFF},
     {"aquamarine",           0x7FFFD4},
     {"azure",                0xF0FFFF},
     {"beige",                0xF5F5DC},
     {"bisque",               0xFFE4C4},
     {"black",                0x000000},
     {"blanchedalmond",       0xFFEBCD},
     {"blue",                 0x0000FF},
     {"blueviolet",           0x8A2BE2},
     {"brown",                0xA52A2A},
     {"burlywood",            0xDEB887},
     {"cadetblue",            0x5F9EA0},
     {"chartreuse",           0x7FFF00},
     {"chocolate",            0xD2691E},
     {"coral",                0xFF7F50},
     {"cornflowerblue",       0x6495ED},
     {"cornsilk",             0xFFF8DC},
     {"crimson",              0xDC143C},
     {"cyan",                 0x00FFFF},
     {"darkblue",             0x00008B},
     {"darkcyan",             0x008B8B},
     {"darkgoldenrod",        0xB8860B},
     {"darkgray",             0xA9A9A9},
     {"darkgreen",            0x006400},
     {"darkgrey",             0xA9A9A9},
     {"darkkhaki",            0xBDB76B},
     {"darkmagenta",          0x8B008B},
     {"darkolivegreen",       0x556B2F},
     {"darkorange",           0xFF8C00},
     {"darkorchid",           0x9932CC},
     {"darkred",              0x8B0000},
     {"darksalmon",           0xE9967A},
     {"darkseagreen",         0x8FBC8F},
     {"darkslateblue",        0x483D8B},
     {"darkslategray",        0x2F4F4F},
     {"darkslategrey",        0x2F4F4F},
     {"darkturquoise",        0x00CED1},
     {"darkviolet",           0x9400D3},
     {"deeppink",             0xFF1493},
     {"deepskyblue",          0x00BFFF},
     {"dimgray",              0x696969},
     {"dimgrey",              0x696969},
     {"dodgerblue",           0x1E90FF},
     {"firebrick",            0xB22222},
     {"floralwhite",          0xFFFAF0},
     {"forestgreen",          0x228B22},
     {"fuchsia",              0xFF00FF},
     {"gainsboro",            0xDCDCDC},
     {"ghostwhite",           0xF8F8FF},
     {"gold",                 0xFFD700},
     {"goldenrod",            0xDAA520},
     {"gray",                 0x808080},
     {"green",                0x008000},
     {"greenyellow",          0xADFF2F},
     {"grey",                 0x808080},
     {"honeydew",             0xF0FFF0},
     {"hotpink",              0xFF69B4},
     {"indianred",            0xCD5C5C},
     {"indigo",               0x4B0082},
     {"ivory",                0xFFFFF0},
     {"khaki",                0xF0E68C},
     {"lavender",             0xE6E6FA},
     {"lavenderblush",        0xFFF0F5},
     {"lawngreen",            0x7CFC00},
     {"lemonchiffon",         0xFFFACD},
     {"lightblue",            0xADD8E6},
     {"lightcoral",           0xF08080},
     {"lightcyan",            0xE0FFFF},
     {"lightgoldenrodyellow", 0xFAFAD2},
     {"lightgray",            0xD3D3D3},
     {"lightgreen",           0x90EE90},
     {"lightgrey",            0xD3D3D3},
     {"lightpink",            0xFFB6C1},
     {"lightsalmon",          0xFFA07A},
     {"lightseagreen",        0x20B2AA},
     {"lightskyblue",         0x87CEFA},
     {"lightslategray",       0x778899},
     {"lightslategrey",       0x778899},
     {"lightsteelblue",       0xB0C4DE},
     {"lightyellow",          0xFFFFE0},
     {"lime",                 0x00FF00},
     {"limegreen",            0x32CD32},
     {"linen",                0xFAF0E6},
     {"magenta",              0xFF00FF},
     {"maroon",               0x800000},
     {"mediumaquamarine",     0x66CDAA},
     {"mediumblue",           0x0000CD},
     {"mediumorchid",         0xBA55D3},
     {"mediumpurple",         0x9370DB},
     {"mediumseagreen",       0x3CB371},
     {"mediumslateblue",      0x7B68EE},
     {"mediumspringgreen",    0x00FA9A},
     {"mediumturquoise",      0x48D1CC},
     {"mediumvioletred",      0xC71585},
     {"midnightblue",         0x191970},
     {"mintcream",            0xF5FFFA},
     {"mistyrose",            0xFFE4E1},
     {"moccasin",             0xFFE4B5},
     {"navajowhite",          0xFFDEAD},
     {"navy",                 0x000080},
     {"oldlace",              0xFDF5E6},
     {"olive",                0x808000},
     {"olivedrab",            0x6B8E23},
     {"orange",               0xFFA500},
     {"orangered",            0xFF4500},
     {"orchid",               0xDA70D6},
     {"palegoldenrod",        0xEEE8AA},
     {"palegreen",            0x98FB98},
     {"paleturquoise",        0xAFEEEE},
     {"palevioletred",        0xDB7093},
     {"papayawhip",           0xFFEFD5},
     {"peachpuff",            0xFFDAB9},
     {"peru",                 0xCD853F},
     {"pink",                 0xFFC0CB},
     {"plum",                 0xDDA0DD},
     {"powderblue",           0xB0E0E6},
     {"purple",               0x800080},
     {"rebeccapurple",        0x663399},
     {"red",                  0xFF0000},
     {"rosybrown",            0xBC8F8F},
     {"royalblue",            0x4169E1},
     {"saddlebrown",          0x8B4513},
     {"salmon",               0xFA8072},
     {"sandybrown",           0xF4A460},
     {"seagreen",             0x2E8B57},
     {"seashell",             0xFFF5EE},
     {"sienna",               0xA0522D},
     {"silver",               0xC0C0C0},
     {"skyblue",              0x87CEEB},
     {"slateblue",            0x6A5ACD},
     {"slategray",            0x708090},
     {"slategrey",            0x708090},
     {"snow",                 0xFFFAFA},
     {"springgreen",          0x00FF7F},
     {"steelblue",            0x4682B4},
     {"tan",                  0xD2B48C},
     {"teal",                 0x008080},
     {"thistle",              0xD8BFD8},
     {"tomato",               0xFF6347},
     {"turquoise",            0x40E0D0},
     {"violet",               0xEE82EE},
     {"wheat",                0xF5DEB3},
     {"white",                0xFFFFFF},
     {"whitesmoke",           0xF5F5F5},
     {"yellow",               0xFFFF00},
     {"yellowgreen",          0x9ACD32} };


rgb_components _rgb = { 0,0,0 };

hsl_components _hslA = { 0, 0, 0 };
hsl_components _hslB = { 0, 0, 0 };

}

//rgb_components _rgb = { 0, 0, 0 };
//hsl_components _hslA = { 0, 0, 0 };
//hsl_components _hslB = { 0, 0, 0 };

using namespace __color;

//double hue2rgb(double p,double q,double t ) {
//    if ( t < 0 ) t += 1;
//    if ( t > 1 ) t -= 1;
//    if ( t < 1 / 6 ) return p + ( q - p ) * 6 * t;
//    if ( t < 1 / 2 ) return q;
//    if ( t < 2 / 3 ) return p + ( q - p ) * 6 * ( 2 / 3 - t );
//    return p;
//}

//rgb_components& toComponents( Color& source, rgb_components& target ) {
//
//    target.r = source.r;
//    target.g = source.g;
//    target.b = source.b;
//
//    return target;
//}



const std::unordered_map<std::string,int>& Color::NAMED_COLORS = _colorKeywords;

int Color::getHex( std::string colorSpace) {
    ColorManagement::fromWorkingColorSpace( toComponents( *this, _rgb ), colorSpace );

    return MathUtils::clamp<int>( (int)(_rgb.r * 255), 0, 255 ) << 16 ^ MathUtils::clamp<int>( (int)(_rgb.g * 255), 0, 255 ) << 8 ^ MathUtils::clamp<int>( (int)(_rgb.b * 255), 0, 255 ) << 0;
}

hsl_components& Color::getHSL(hsl_components& target, std::string colorSpace) {
    // h,s,l ranges are in 0.0 - 1.0
    ColorManagement::fromWorkingColorSpace(toComponents(*this, _rgb), colorSpace);

    const int r = _rgb.r, g = _rgb.g, b = _rgb.b;

    int max = std::max<int>(std::max<int>(r, g), b);
    int min = std::min<int>(std::min<int>(r, g), b);

    double  hue, saturation;
    const double lightness = (min + max) / 2.0;

    if (min == max) {
        hue = 0;
        saturation = 0;

    }
    else {
        const int delta = max - min;

        saturation = lightness <= 0.5 ? delta / (max + min) : delta / (2 - max - min);

        if (max == r)
            hue = (g - b) / delta + (g < b ? 6 : 0);
        else if (max == g)
            hue = (b - r) / delta + 2;
        else if (max == b)
            hue = (r - g) / delta + 4;

        hue /= 6;

    }

    target.h = hue;
    target.s = saturation;
    target.l = lightness;

    return target;
}

rgb_components& Color::getRGB(rgb_components& target, std::string colorSpace) {
    ColorManagement::fromWorkingColorSpace(toComponents(*this, _rgb), colorSpace);

    target.r = _rgb.r;
    target.g = _rgb.g;
    target.b = _rgb.b;

    return target;
}

Color& Color::lerpHSL(Color& color, double alpha) {
    this->getHSL(_hslA);
    color.getHSL(_hslB);

    double h = MathUtils::lerp(_hslA.h, _hslB.h, alpha);
    double s = MathUtils::lerp(_hslA.s, _hslB.s, alpha);
    double l = MathUtils::lerp(_hslA.l, _hslB.l, alpha);

    this->setHSL(h, s, l);

    return *this;
}

Color& Color::offsetHSL(double h, double s, double l) {
    this->getHSL(_hslA);

    _hslA.h += h; _hslA.s += s; _hslA.l += l;

    this->setHSL(_hslA.h, _hslA.s, _hslA.l);

    return *this;
}

