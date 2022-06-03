#include <cmath>

template<typename T>
T clamp(T value, T min, T max) {
	return fmax(min, fmin(max, value));
}