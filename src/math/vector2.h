#ifndef SRC_MATH_VECTOR2_H_
#define SRC_MATH_VECTOR2_H_

#include <cassert>
#include <algorithm>
#include <cmath>
#include <vector>
#include <sstream>

#include "osdecl.h"
#include "math_utils.h"

class Matrix3;
template<class T> class BufferAttribute;


class Vector2
{
public:
    union {
        struct {
            float x, y;
        };
        float elements[2];
    };

    Vector2() : x(0), y(0) {}

    Vector2(float _x, float _y) : x(_x), y(_y) {}

    explicit Vector2(float value) : x(value), y(value) {}

    explicit Vector2(float* values) :x(values[0]), y(values[1]) {}

    const float width() const { return x; }

    const float height() const { return y; }


    Vector2& set(float _x, float _y)
    {

        x = _x;
        y = _y;

        return *this;
    }

    Vector2& setScalar(float scalar)
    {
        x = scalar;
        y = scalar;

        return *this;
    }

    Vector2& operator = (float scalar) {
        return setScalar(scalar);
    }

    Vector2& setX(float _x)
    {
        x = _x;

        return *this;
    }

    Vector2& setY(float _y)
    {
        y = _y;

        return *this;
    }

    Vector2& setComponent(int index, float value)
    {
        assert(index < 2);

        switch (index)
        {
            case 0: x = value; break;
            case 1: y = value; break;
        }

        return *this;
    }

    float getComponent(int index)
    {
        assert(index < 2);

        switch (index)
        {
            case 0: return x;
            case 1: return y;
            default:return x;
        }
    }

    Vector2& clone(Vector2* v)
    {
        v->x = x;
        v->y = y;

        return *v;
    }
    Vector2& clone()
    {
        return *this;
    }

    Vector2& copy(const Vector2& v)
    {
        x = v.x;
        y = v.y;

        return *this;
    }

    Vector2& add(const Vector2& v)
    {
        x += v.x;
        y += v.y;

        return *this;
    }

    Vector2& operator+=(const Vector2 a) {
        return add(a);
    }

    Vector2& addScalar(const float s)
    {
        x += s;
        y += s;

        return *this;
    }

    Vector2& operator +=(float scalar) {
        return addScalar(scalar);
    }

    Vector2& addVectors(const Vector2& a, const Vector2& b)
    {
        x = a.x + b.x;
        y = a.y + b.y;

        return *this;
    }

    Vector2& addScaledVector(const Vector2& v, float s)
    {
        x += v.x * s;
        y += v.y * s;

        return *this;
    }

    Vector2& sub(const Vector2& v)
    {
        x -= v.x;
        y -= v.y;

        return *this;
    }

    Vector2& operator -=(const Vector2 v) {
        return sub(v);
    }

    Vector2& subScalar(const float s)
    {
        x -= s;
        y -= s;

        return *this;
    }

    Vector2& operator -=(const float s) {
        return subScalar(s);
    }

    Vector2& subVectors(const Vector2& a, const Vector2& b)
    {
        x = a.x - b.x;
        y = a.y - b.y;

        return *this;
    }

    Vector2& multiply(const Vector2& v)
    {
        x *= v.x;
        y *= v.y;

        return *this;
    }
    Vector2& operator *= (const Vector2 v) {
        return multiply(v);
    }

    Vector2& multiplyScalar(const float scalar)
    {
        x *= scalar;
        y *= scalar;

        return *this;
    }

    Vector2& operator *= (const float scalar) {
        return multiplyScalar(scalar);
    }

    Vector2& divide(const Vector2& v)
    {
        x /= v.x;
        y /= v.y;

        return *this;
    }

    Vector2& operator /= (const Vector2& v) {
        return divide(v);
    }
    Vector2& divideScalar(const float scalar)
    {
        return multiplyScalar(1 / scalar);
    }

    Vector2& operator /= (const float scalar) {
        return divideScalar(scalar);
    }

    Vector2& applyMatrix3(const Matrix3& m);

    Vector2& min(const Vector2& v)
    {
        x = std::min(x, v.x);
        y = std::min(y, v.y);

        return *this;
    }

    Vector2& max(const Vector2& v)
    {
        x = std::max(x, v.x);
        y = std::max(y, v.y);

        return *this;
    }

    Vector2& clamp(const Vector2& min, const Vector2& max)
    {
        x = std::max(min.x, std::min(max.x, x));
        y = std::max(min.y, std::min(max.y, y));

        return *this;
    }

    Vector2& clampScalar(float minVal, float maxVal)
    {
        x = std::max(minVal, std::min(maxVal, x));
        y = std::max(minVal, std::min(maxVal, y));

        return *this;
    }

    Vector2& clampLength(float min, float max)
    {
        float len = length();

        return divideScalar(len > 0 ? len : 1).multiplyScalar(std::max(min, std::min(max, len)));
    }

    Vector2& floor()
    {
        x = std::floor(x);
        y = std::floor(y);

        return *this;
    }

    Vector2& ceil()
    {
        x = std::ceil(x);
        y = std::ceil(y);

        return *this;
    }

    Vector2& round()
    {
        x = std::round(x);
        y = std::round(y);

        return *this;
    }

    Vector2& roundToZero()
    {
        x = (x < 0) ? std::ceil(x) : std::floor(x);
        y = (y < 0) ? std::ceil(y) : std::floor(y);

        return *this;
    }

    Vector2& negate()
    {
        x = -x;
        y = -y;

        return *this;
    }

    float dot(const Vector2& v)
    {
        return x * v.x + y * v.y;
    }

    float cross(const Vector2& v)
    {
        return x * v.x - y * v.y;
    }

    const float lengthSq() const
    {
        return x * x + y * y;
    }

    const float length() const
    {
        return (float)std::sqrt(x * x + y * y);
    }

    const float manhattanLength()
    {
        return std::abs(x) + std::abs(y);
    }

    Vector2& normalize()
    {
        float len = length();

        return divideScalar(len > 0 ? len : 1);
    }

    float angle()
    {
        float angle = std::atan2(-y, -x) + math_number::PI;
        return angle;
    }

    float distanceTo(const Vector2& v)
    {
        return std::sqrt(distanceToSquared(v));
    }

    float distanceToSquared(const Vector2& v)
    {
        float dx = x - v.x;
        float dy = y - v.y;

        return dx * dx + dy * dy;
    }

    float manhattanDistanceTo(const Vector2& v)
    {
        return std::abs(x - v.x) + std::abs(y - v.y);
    }

    Vector2& setLength(float length)
    {
        return normalize().multiplyScalar(length);
    }

    Vector2& lerp(const Vector2& v, float alpha)
    {
        x += (v.x - x) * alpha;
        y += (v.y - y) * alpha;

        return *this;
    }

    Vector2& lerpVectors(const Vector2& v1, const Vector2& v2, float alpha)
    {
        return subVectors(v2, v1).multiplyScalar(alpha).add(v1);
    }

    bool equals(const Vector2& v) const
    {
        return (v.x == x && v.y == y);
    }
    Vector2& fromArray(const std::vector<float> array, unsigned offset = 0)
    {
        x = offset < array.size() ? array[offset] : std::numeric_limits<float>::quiet_NaN();
        y = offset < array.size() ? array[offset + 1] : std::numeric_limits<float>::quiet_NaN();

        return *this;
    }

    Vector2& fromArray(const float* array, unsigned arrayLength,unsigned offset = 0)
    {
        x = offset < arrayLength ? array[offset] : std::numeric_limits<float>::quiet_NaN();
        y = offset < arrayLength ? array[offset + 1] : std::numeric_limits<float>::quiet_NaN();

        return *this;
    }

    const float* toArray(float* array, unsigned offset = 0) const
    {
        float a[2];
        if (array == nullptr) array = a;

        array[offset] = x;
        array[offset + 1] = y;

        return array;
    }

    template<typename T>
    Vector2& fromBufferAttribute(const BufferAttribute<T>& attribute, unsigned index);

    Vector2& rotateAround(const Vector2& center, float angle)
    {
        float c = std::cos(angle), s = std::sin(angle);

        float _x = x - center.x;
        float _y = y - center.y;

        x = _x * c - _y * s + center.x;
        y = _x * s + _y * c + center.y;

        return *this;
    }
    std::string to_string() {
        std::stringstream stream;
        stream << "x=" << std::to_string(x) << ",y=" << std::to_string(y);
        return stream.str();
    }
};

//#include <stdexcept>
//#include <vector>
//
//#include "matrix3.h"
//#include "math_utils.h"
//
//template <typename T> class BufferAttribute;
//
//class Vector2 {
//public:
//    union {
//        struct {
//            double x,y;
//        };
//        double elements[2];
//    };
//
//        Vector2(double x = 0,double y = 0):x(x),y(y){};
//
//        double width() {
//            return x;
//        }
//
//	    Vector2& width( double value ) {
//            x = value;
//            return *this;
//        }
//
//        double height() {
//            return y;
//        }
//
//        Vector2& height( double value ) {
//            y = value;
//            return *this;
//        }
//
//        Vector2& set( double x, double y ) {
//            this->x = x;
//            this->y = y;
//
//            return *this;
//        }
//
//        Vector2& setScalar( double scalar ) {
//            this->x = scalar;
//            this->y = scalar;
//
//            return *this;
//        }
//
//        Vector2& setX( double x ) {
//            this->x = x;
//
//            return *this;
//        }
//
//        Vector2& setY( double y ) {
//            this->y = y;
//
//            return *this;
//        }
//
//        Vector2& setComponent( int index, double value ) {
//            switch ( index ) {
//                case 0: this->x = value; break;
//                case 1: this->y = value; break;
//                default: throw std::out_of_range( "index is out of range: " + std::to_string(index) );
//            }
//
//            return *this;
//        }
//
//        double getComponent( int index ) {
//            switch ( index ) {
//                case 0: return this->x;
//                case 1: return this->y;
//                default: throw std::out_of_range( "index is out of range: " + std::to_string(index) );
//            }
//        }
//
//        Vector2 clone() {
//            return Vector2( x,y );
//        }
//
//        Vector2& copy(const Vector2& v) {
//            x = v.x;
//            y = v.y;
//
//            return *this;
//        }
//
//        Vector2& add( Vector2& v){
//            x += v.x;
//            y += v.y;
//
//            return *this;
//        }
//
//        Vector2& addScalar( double s ) {
//            x += s;
//            y += s;
//
//            return *this;
//        }
//
//        Vector2& addVectors( Vector2& a,Vector2& b ) {
//            x = a.x + b.x;
//            y = a.y + b.y;
//
//            return *this;
//        }
//
//        Vector2& addScaledVector( Vector2& v,double s ) {
//            x += v.x * s;
//            y += v.y * s;
//
//            return *this;
//        }
//
//        Vector2& sub( Vector2& v ) {
//            x -= v.x;
//            y -= v.y;
//
//            return *this;
//        }
//
//        Vector2& subScalar( double s ) {
//            x -= s;
//            y -= s;
//
//            return *this;
//        }
//
//        Vector2& subVectors( Vector2& a,Vector2& b ) {
//            x = a.x - b.x;
//            y = a.y - b.y;
//
//            return *this;
//        }
//
//        Vector2& multiply( Vector2& v ) {
//            x *= v.x;
//            y *= v.y;
//
//            return *this;
//        }
//
//        Vector2& multiplyScalar( double scalar ) {
//            x *= scalar;
//            y *= scalar;
//
//            return *this;
//        }
//
//        Vector2& divide( Vector2& v ) {
//            x /= v.x;
//            y /= v.y;
//
//            return *this;
//        }
//
//        Vector2& divideScalar( double scalar ) {
//            return multiplyScalar( 1 / scalar );
//        }
//
//        Vector2& applyMatrix3( Matrix3& m ) {
//            const double x = this->x, y = this->y;
//
//            this->x = m.elements[ 0 ] * x + m.elements[ 3 ] * y + m.elements[ 6 ];
//            this->y = m.elements[ 1 ] * x + m.elements[ 4 ] * y + m.elements[ 7 ];
//
//            return *this;
//        }
//
//        Vector2& min( Vector2& v ) {
//            this->x = fmin( this->x, v.x );
//            this->y = fmin( this->y, v.y );
//
//            return *this;
//        }
//
//        Vector2& max( Vector2& v ) {
//            x = fmax( x, v.x );
//            y = fmax( y, v.y );
//
//            return *this;
//        }
//
//        Vector2& clamp( Vector2& min,Vector2& max ) {
//            // assumes min < max, componentwise
//            x = fmax( min.x, fmin( max.x, x ) );
//            y = fmax( min.y, fmin( max.y, y ) );
//
//            return *this;
//        }
//
//        Vector2& clampScalar( double minVal, double maxVal ) {
//            x = fmax( minVal, fmin( maxVal, x ) );
//            y = fmax( minVal, fmin( maxVal, y ) );
//
//            return *this;
//        }
//
//        Vector2& clampLength( double min,double max ) {
//            const double length = this->length();
//
//            return divideScalar( length > 0 ? length : 1 ).multiplyScalar( fmax( min, fmin( max, length ) ) );
//        }
//
//    	double length() {
//            return sqrt( x * x + y * y );
//        }
//
//        double lengthSq() {
//            return x * x + y * y;
//        }
//
//        Vector2& floor() {
//            this->x = std::floor( this->x );
//            this->y = std::floor( this->y );
//
//            return *this;
//        }
//
//        Vector2& ceil() {
//            this->x = std::ceil( this->x );
//            this->y = std::ceil( this->y );
//
//            return *this;
//        }
//
//        Vector2& round() {
//            this->x = std::round( this->x );
//            this->y = std::round( this->y );
//
//            return *this;
//        }
//
//        Vector2& roundToZero() {
//            x = ( x < 0 ) ? std::ceil( x ) : std::floor( x );
//            y = ( y < 0 ) ? std::ceil( y ) : std::floor( y );
//
//            return *this;
//        }
//
//        Vector2& negate() {
//            x = - x;
//            y = - y;
//
//            return *this;
//        }
//
//        double dot( Vector2& v ) {
//            return x * v.x + y * v.y;
//        }
//
//        double cross( Vector2& v ) {
//            return x * v.y - y * v.x;
//        }
//
//        double manhattanLength() {
//            return abs( x ) + abs( y );
//        }
//
//        Vector2& normalize() {
//            const double len = length();
//            return divideScalar( len > 0 ? len : 1 );
//        }
//
//        double angle() {
//            // computes the angle in radians with respect to the positive x-axis
//            const double angle = atan2( - this->y, - this->x ) + M_PI;
//
//            return angle;
//        }
//
//        double distanceTo( Vector2& v ) {
//            return sqrt( distanceToSquared( v ) );
//        }
//
//        double distanceToSquared( Vector2& v ) {
//            const double dx = x - v.x, dy = y - v.y;
//            return dx * dx + dy * dy;
//        }
//
//        double manhattanDistanceTo( Vector2& v ) {
//            return abs( x - v.x ) + abs( y - v.y );
//        }
//
//        Vector2& setLength( double length ) {
//            return normalize().multiplyScalar( length );
//        }
//
//        Vector2& lerp( Vector2& v, double alpha ) {
//            x += ( v.x - x ) * alpha;
//            y += ( v.y - y ) * alpha;
//
//            return *this;
//        }
//
//        Vector2& lerpVectors( Vector2& v1, Vector2& v2, double alpha ) {
//            x = v1.x + ( v2.x - v1.x ) * alpha;
//            y = v1.y + ( v2.y - v1.y ) * alpha;
//
//            return *this;
//        }
//
//        bool equals( Vector2& v ) {
//            return ( ( v.x == x ) && ( v.y == y ) );
//        }
//
//        Vector2& fromArray( double array[], int offset = 0 ) {
//            x = array[ offset ];
//            y = array[ offset + 1 ];
//
//            return *this;
//        }
//
//        Vector2& fromArrayVec( std::vector<float> array, int offset = 0 ) {
//            x = (double)array[ offset ];
//            y = (double)array[ offset + 1 ];
//
//            return *this;
//        }
//
//        double* toArray( double array[], int offset = 0 ) {
//            array[ offset ] = x;
//            array[ offset + 1 ] = y;
//
//            return array;
//        }
//
//        template<typename T>
//        Vector2& fromBufferAttribute(BufferAttribute<T>& attribute,int index);
//
//        Vector2& rotateAround( Vector2& center, double angle ) {
//
//            const double c = cos( angle ), s = sin( angle );
//
//            const double dx = x - center.x;
//            const double dy = y - center.y;
//
//            x = dx * c - dy * s + center.x;
//            y = dx * s + dy * c + center.y;
//
//            return *this;
//        }
//
//        Vector2& random() {
//            x = MathUtils::random_gen<double>();
//            y = MathUtils::random_gen<double>();
//
//            return *this;
//        }
//
//        // *[ Symbol.iterator ]() {
//
//        //     yield this.x;
//        //     yield this.y;
//
//        // }
//
//};

#endif /* SRC_MATH_VECTOR2_H_ */
