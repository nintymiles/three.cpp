#ifndef SRC_MATH_VECTOR2_H_
#define SRC_MATH_VECTOR2_H_

#include <stdexcept>
#include <vector>

#include "matrix3.h"
#include "math_utils.h"

template <typename T> class BufferAttribute;

class Vector2 {
public:
    union {
        struct {
            double x,y;
        };
        double elements[2];
    };

        Vector2(double x = 0,double y = 0):x(x),y(y){};

        double width() {
            return x;
        }

	    Vector2& width( double value ) {
            x = value;
            return *this;
        }

        double height() {
            return y;
        }

        Vector2& height( double value ) {
            y = value;
            return *this;
        }

        Vector2& set( double x, double y ) {
            this->x = x;
            this->y = y;

            return *this;
        }

        Vector2& setScalar( double scalar ) {
            this->x = scalar;
            this->y = scalar;

            return *this;
        }

        Vector2& setX( double x ) {
            this->x = x;

            return *this;
        }

        Vector2& setY( double y ) {
            this->y = y;

            return *this;
        }

        Vector2& setComponent( int index, double value ) {
            switch ( index ) {
                case 0: this->x = value; break;
                case 1: this->y = value; break;
                default: throw std::out_of_range( "index is out of range: " + std::to_string(index) );
            }

            return *this;
        }

        double getComponent( int index ) {
            switch ( index ) {
                case 0: return this->x;
                case 1: return this->y;
                default: throw std::out_of_range( "index is out of range: " + std::to_string(index) );
            }
        }

        Vector2 clone() {
            return Vector2( x,y );
        }

        Vector2& copy(const Vector2& v) {
            x = v.x;
            y = v.y;

            return *this;
        }

        Vector2& add( Vector2& v){
            x += v.x;
            y += v.y;

            return *this;
        }

        Vector2& addScalar( double s ) {
            x += s;
            y += s;

            return *this;
        }

        Vector2& addVectors( Vector2& a,Vector2& b ) {
            x = a.x + b.x;
            y = a.y + b.y;

            return *this;
        }

        Vector2& addScaledVector( Vector2& v,double s ) {
            x += v.x * s;
            y += v.y * s;

            return *this;
        }

        Vector2& sub( Vector2& v ) {
            x -= v.x;
            y -= v.y;

            return *this;
        }

        Vector2& subScalar( double s ) {
            x -= s;
            y -= s;

            return *this;
        }

        Vector2& subVectors( Vector2& a,Vector2& b ) {
            x = a.x - b.x;
            y = a.y - b.y;

            return *this;
        }

        Vector2& multiply( Vector2& v ) {
            x *= v.x;
            y *= v.y;

            return *this;
        }

        Vector2& multiplyScalar( double scalar ) {
            x *= scalar;
            y *= scalar;

            return *this;
        }

        Vector2& divide( Vector2& v ) {
            x /= v.x;
            y /= v.y;

            return *this;
        }

        Vector2& divideScalar( double scalar ) {
            return multiplyScalar( 1 / scalar );
        }

        Vector2& applyMatrix3( Matrix3& m ) {
            const double x = this->x, y = this->y;
 
            this->x = m.elements[ 0 ] * x + m.elements[ 3 ] * y + m.elements[ 6 ];
            this->y = m.elements[ 1 ] * x + m.elements[ 4 ] * y + m.elements[ 7 ];

            return *this;
        }

        Vector2& min( Vector2& v ) {
            this->x = fmin( this->x, v.x );
            this->y = fmin( this->y, v.y );

            return *this;
        }

        Vector2& max( Vector2& v ) {
            x = fmax( x, v.x );
            y = fmax( y, v.y );

            return *this;
        }

        Vector2& clamp( Vector2& min,Vector2& max ) {
            // assumes min < max, componentwise
            x = fmax( min.x, fmin( max.x, x ) );
            y = fmax( min.y, fmin( max.y, y ) );

            return *this;
        }

        Vector2& clampScalar( double minVal, double maxVal ) {
            x = fmax( minVal, fmin( maxVal, x ) );
            y = fmax( minVal, fmin( maxVal, y ) );

            return *this;
        }

        Vector2& clampLength( double min,double max ) {
            const double length = this->length();

            return divideScalar( length > 0 ? length : 1 ).multiplyScalar( fmax( min, fmin( max, length ) ) );
        }

    	double length() {
            return sqrt( x * x + y * y );
        }

        double lengthSq() {
            return x * x + y * y;
        }

        Vector2& floor() {
            this->x = std::floor( this->x );
            this->y = std::floor( this->y );

            return *this;
        }

        Vector2& ceil() {
            this->x = std::ceil( this->x );
            this->y = std::ceil( this->y );

            return *this;
        }

        Vector2& round() {
            this->x = std::round( this->x );
            this->y = std::round( this->y );

            return *this;
        }

        Vector2& roundToZero() {
            x = ( x < 0 ) ? std::ceil( x ) : std::floor( x );
            y = ( y < 0 ) ? std::ceil( y ) : std::floor( y );

            return *this;
        }

        Vector2& negate() {
            x = - x;
            y = - y;

            return *this;
        }

        double dot( Vector2& v ) {
            return x * v.x + y * v.y;
        }

        double cross( Vector2& v ) {
            return x * v.y - y * v.x;
        }

        double manhattanLength() {
            return abs( x ) + abs( y );
        }

        Vector2& normalize() {
            const double len = length();
            return divideScalar( len > 0 ? len : 1 );
        }

        double angle() {
            // computes the angle in radians with respect to the positive x-axis
            const double angle = atan2( - this->y, - this->x ) + M_PI;

            return angle;
        }

        double distanceTo( Vector2& v ) {
            return sqrt( distanceToSquared( v ) );
        }

        double distanceToSquared( Vector2& v ) {
            const double dx = x - v.x, dy = y - v.y;
            return dx * dx + dy * dy;
        }

        double manhattanDistanceTo( Vector2& v ) {
            return abs( x - v.x ) + abs( y - v.y );
        }

        Vector2& setLength( double length ) {
            return normalize().multiplyScalar( length );
        }

        Vector2& lerp( Vector2& v, double alpha ) {
            x += ( v.x - x ) * alpha;
            y += ( v.y - y ) * alpha;

            return *this;
        }

        Vector2& lerpVectors( Vector2& v1, Vector2& v2, double alpha ) {
            x = v1.x + ( v2.x - v1.x ) * alpha;
            y = v1.y + ( v2.y - v1.y ) * alpha;

            return *this;
        }

        bool equals( Vector2& v ) {
            return ( ( v.x == x ) && ( v.y == y ) );
        }

        Vector2& fromArray( double array[], int offset = 0 ) {
            x = array[ offset ];
            y = array[ offset + 1 ];

            return *this;
        }

        Vector2& fromArray( std::vector<double> array, int offset = 0 ) {
            x = array[ offset ];
            y = array[ offset + 1 ];

            return *this;
        }

        double* toArray( double array[], int offset = 0 ) {
            array[ offset ] = x;
            array[ offset + 1 ] = y;

            return array;
        }

        template<typename T>
        Vector2& fromBufferAttribute(BufferAttribute<T>& attribute,int index);

        Vector2& rotateAround( Vector2& center, double angle ) {

            const double c = cos( angle ), s = sin( angle );

            const double dx = x - center.x;
            const double dy = y - center.y;

            x = dx * c - dy * s + center.x;
            y = dx * s + dy * c + center.y;

            return *this;
        }

        Vector2& random() {
            x = MathUtils::random_gen<double>();
            y = MathUtils::random_gen<double>();

            return *this;
        }

        // *[ Symbol.iterator ]() {

        //     yield this.x;
        //     yield this.y;

        // }

};

#endif /* SRC_MATH_VECTOR2_H_ */
