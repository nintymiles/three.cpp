#include <cmath>
#include <random>

template<typename T>
T clamp(T value, T min, T max) {
	return fmax(min, fmin(max, value));
}

template<typename T>
T random_gen(){
	std::default_random_engine generator;
    std::uniform_int_distribution<T> distribution(0,1);
	return distribution(generator); 
}