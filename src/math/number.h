#ifndef THREE_CPP_NUMBER_H
#define THREE_CPP_NUMBER_H

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#include <math.h>
#endif

#include <cmath>
#include <limits>
namespace math_number{
    //when for const error: in-class initializer for static data member of type 'const double' requires 'constexpr' specifier [-Wstatic-float-init]
    constexpr double EPSILON = std::numeric_limits<double>::epsilon();
    constexpr double PI = M_PI;

    constexpr float LN2 =0.6931471805599453094f;

    constexpr float INF = std::numeric_limits<float>::infinity();

    constexpr double E = 2.7182818284590451;

    constexpr double MAX_FLT = std::numeric_limits<float>::max();
    constexpr double MIN_FLT = std::numeric_limits<float>::min();

    constexpr double MAX = std::numeric_limits<double>::max();
    constexpr double MIN = std::numeric_limits<double>::min();

    constexpr int MAX_INT = std::numeric_limits<int>::max();
    constexpr int MIN_INT = std::numeric_limits<int>::min();

    constexpr double DEG2RAD = M_PI / 180;
    constexpr double RAD2DEG = 180 / M_PI;

    constexpr float SQRT1_2 = 0.7071067811865476f;
    constexpr double PI2 = (2 * M_PI);
    constexpr double HalfPI = (M_PI / 2.0f);


};


#endif //THREE_CPP_NUMBER_H
