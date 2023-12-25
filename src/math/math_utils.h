#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include "number.h"

#include <cmath>
#include <random>
#include <sstream>
#include <iostream>
#include <numeric>
#include <climits>

namespace math_utils{

//    template<typename T>
//    inline T random_gen() {
//        std::default_random_engine generator;
//        //std::uniform_int_distribution<T> distribution(0,1);
//        std::uniform_real_distribution<T> distribution(0.0, 1.0);
//        return distribution(generator);
//    }

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

}

namespace math {

//const float PI= 3.1415926535897932384f;//std::atan(1.f)/4;
//#ifndef _MSC_VER
//const float M_PI = PI;
//#endif


template < typename T > inline T sqrt(T t) { return std::sqrt(t); }

template < typename T > inline T abs(T t) { return std::abs(t); }

template < typename T > inline T acos(T t) { return std::acos(t); }

template < typename T > inline T asin(T t) { return std::asin(t); }

template < typename T > inline T atan2(T y, T x) { return std::atan2(y, x); }

template < typename T > inline T atan(T t) { return std::atan(t); }

template < typename T > inline T cos(T t) { return std::cos(t); }

template < typename T > inline T sin(T t) { return std::sin(t); }

template < typename T > inline T tan(T t) { return std::tan(t); }

template < typename T > inline T log(T t) { return std::log(t); }

template < typename T, typename U >  inline T pow(T a, U b) { return std::pow(a, b); }

#if defined(_MSC_VER)
template < typename T > inline T round(T n) { return (n > (T)0) ? std::floor(n + (T)0.5) : std::ceil(n - (T)0.5); }
#else
template < typename T > inline T round(T t) { return std::round(t); }
#endif
template < typename T > inline T ceil(T t) { return std::ceil(t); }
template < typename T > inline T floor(T t) { return std::floor(t); }

template < typename T > inline T fmod(T a, T b) { return std::fmod(a, b); }


//    template<typename T>
//    inline T clamp(T value, T min, T max) {
//        return (T)fmax(min, fmin(max, value));
//    }
template < typename T > inline T clamp(T x, T a, T b) { return x < a ? a : ((x > b) ? b : x); }
template < typename T > inline T clampBottom(T x, T a) { return x < a ? a : x; }

// Linear mapping from range <a1, a2> to range <b1, b2>
template < typename T >

inline T mapLinear(T x, T a1, T a2, T b1, T b2) {
    return b1 + (x - a1) * (b2 - b1) / (a2 - a1);
}

// MinGW crashes on std::random_device initialization
#if !defined(__MINGW32__)

template < typename T >

inline typename std::enable_if<std::is_floating_point<T>::value, T>::type
randomT(T low, T high) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<T> dis(low, high);
    return dis(rng);
}

#else

template< typename T >
		inline T randomT(T low, T high) {
			return (low + _cast<double>(rand()) / ((unsigned long long)RAND_MAX + 1) * (high - low));
		}

#endif // !defined(__MINGW32__)

inline float random(float low = 0, float high = 1) {
    return randomT(low, high);
}

// Random float from <0, 1> with 16 bits of randomness
// (standard Math.random() creates repetitive patterns when applied over larger space)
inline float random16() {
    return (65280 * random() + 255 * random()) / 65535;
}

// Random integer from <low, high> interval
inline int randInt(int low, int high) {
    return low + (int)floor(random() * (high - low + 1));
}

// Random float from <low, high> interval
inline float randFloat(float low, float high) {
    return random(low, high);
}

// Random float from <-range/2, range/2> interval
inline float randFloatSpread(float range) {
    return range * (0.5f - random());
}

template < typename T >
inline int sign(T x) {
    return (x < 0) ? -1 : ((x > 0) ? 1 : 0);
}

inline bool isPowerOfTwo(int value) {
    return (value != 0) && ((value & (value - 1)) == 0);
}

inline int upperPowerOfTwo(int value) {
    return (int)pow(2.f, ceil(log((float)value) / math_number::LN2));
}

inline int lowerPowerOfTwo(int value) {
    return (int)pow(2.f, floor(log((float)value) / math_number::LN2));
}

inline int nearestPowerOfTwo(int value) {
    return (int)pow(2.f, round(log((float)value) / math_number::LN2));
}

//inline std::string generateUUID()
//{
//#ifdef _MSC_VER
////    UUID id;
////
////    if (RPC_S_OK != UuidCreate(&id))
////        id = GUID_NULL;
////
////    unsigned char* str;
////
////    if (UuidToStringA(&id, (RPC_CSTR*)&str))
////        return std::string();
////
////    std::string s((char*)str);
////
////    RpcStringFreeA(&str);
//#else
//    uuid_t uuid;
//			uuid_generate_random(uuid);
//			char s[37];
//			uuid_unparse(uuid, s);
//#endif
//    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
//    return s;
//}

//cpp中，运算符%不能用于float和double类型
// compute euclidean modulo of m % n
// https://en.wikipedia.org/wiki/Modulo_operation
//template<typename T>
//inline T euclideanModulo( T n, T m ) {
//    return fmod( ((fmod(n,m)) + m) , m );
//}
inline float euclideanModulo(float n, float m){
    if (m == 0) return (float)NAN;
    return std::fmod(std::fmod(n, m) + m, m);
    //return std::remainder((std::remainder(n, m) + m), m);
}

//    inline float lerp(double a, double b, double t){
//        return a + t * (b - a);
//    }
template <typename T> T lerp(T x, T y, T t){
    return (1 - t) * x + t * y;
}

template <typename T> T smoothstep(T x, T min, T max) {

    if (x <= min) return 0;
    if (x >= max) return 1;

    x = (x - min) / (max - min);

    return x * x * (3 - 2 * x);
}

template <typename T> T smootherstep(T x, T min, T max){
    if (x <= min) return 0;
    if (x >= max) return 1;

    x = (x - min) / (max - min);

    return x * x * x * (x * (x * 6 - 15) + 10);
}

inline double degToRad(double degrees){
    return degrees * math_number::PI / 180.0;
}

inline double radToDeg(double radians){
    return radians * 180.0 / math_number::PI;
}

inline int ceilPowerOfTwo(double value){
    return (int)std::pow(2, std::ceil(std::log(value) / M_LN2));
}

inline int floorPowerOfTwo(double value){
    return (int)std::pow(2, std::floor(std::log(value) / M_LN2));
}

}

#endif //MATH_UTILS_H