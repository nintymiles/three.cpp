#ifndef MATH_UTILS_H
#define MATH_UTILS_H
 
#ifdef _MSC_VER
#define _USE_MATH_DEFINES 
#include <math.h>
#endif

#include <cmath>
#include <random>
#include <sstream>
#include <iostream>
#include <numeric>

namespace MathUtils {

    const double DEG2RAD = M_PI / 180;
    const double RAD2DEG = 180 / M_PI;

    template<typename T>
    T clamp(T value, T min, T max) {
        return (T)fmax(min, fmin(max, value));
    }

    //cpp中，运算符%不能用于float和double类型
    // compute euclidean modulo of m % n
    // https://en.wikipedia.org/wiki/Modulo_operation
    template<typename T>
    T euclideanModulo( T n, T m ) {
        return fmod( ((fmod(n,m)) + m) , m );
    }

    template<typename T>
    T random_gen() {
        std::default_random_engine generator;
        //std::uniform_int_distribution<T> distribution(0,1);
        std::uniform_real_distribution<T> distribution(0.0, 1.0);
        return distribution(generator);
    }

//https://stackoverflow.com/questions/24365331/how-can-i-generate-uuid-in-c-without-using-boost-library
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    inline std::string generate_uuid_v4() {
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++) {
            ss << dis(gen);
        };
        return ss.str();
    }

//https://stackoverflow.com/questions/24365331/how-can-i-generate-uuid-in-c-without-using-boost-library
    inline std::string generate_uuid() {
        static std::random_device dev;
        static std::mt19937 rng(dev());

        std::uniform_int_distribution<int> dist(0, 15);

        const char *v = "0123456789abcdef";
        const bool dash[] = {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0};

        std::string res;
        for (int i = 0; i < 16; i++) {
            if (dash[i]) res += "-";
            res += v[dist(rng)];
            res += v[dist(rng)];
        }
        return res;
    }

    inline std::string generate_uuid(std::string &seedstring) {
        auto sum = std::accumulate(seedstring.begin(), seedstring.end(), 0);

        std::mt19937 gen(sum);
        std::uniform_int_distribution<> dis(0, 15);
        std::uniform_int_distribution<> dis2(8, 11);

        std::stringstream ss;
        ss.clear();
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++) {
            ss << dis(gen);
        };
        std::cout << "uuid is" << ss.str() << std::endl;
        return ss.str();
    }

    // Random float from <-range/2, range/2> interval
    float randFloatSpread( float range ) {
        return range * ( 0.5 - random_gen<float>() );
    }

    //模板函数的别名如何设置
    //typedef clamp<double> clampd;
    //using clampd = double clamp<double>(double,double,double);

}

#endif //MATH_UTILS_H