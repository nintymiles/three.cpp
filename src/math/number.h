#ifndef THREE_CPP_NUMBER_H
#define THREE_CPP_NUMBER_H

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#include <math.h>
#endif

#include <cmath>
#include <limits>
namespace Number{
    //when for const error: in-class initializer for static data member of type 'const double' requires 'constexpr' specifier [-Wstatic-float-init]
    constexpr double EPSILON = std::numeric_limits<double>::epsilon();
    constexpr double PI = M_PI;

    constexpr double MAX = std::numeric_limits<double>::max();
    constexpr double MIN = std::numeric_limits<double>::min();

    constexpr int MAX_INT = std::numeric_limits<int>::max();
    constexpr int MIN_INT = std::numeric_limits<int>::min();
};


#endif //THREE_CPP_NUMBER_H
