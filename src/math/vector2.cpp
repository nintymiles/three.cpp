#include "vector2.h"
#include "matrix3.h"
#include "buffer_attribute.h"

Vector2& Vector2::applyMatrix3(const Matrix3& m){
    const float* e = m.elements;

    float _x = this->x;
    float _y = this->y;

    x = e[0] * _x + e[3] * _y + e[6];
    y = e[1] * _x + e[4] * _y + e[7];

    return *this;
}

template<typename T>
Vector2& Vector2::fromBufferAttribute(const BufferAttribute<T>& attribute, unsigned index) {
    this->x = attribute.getX( index );
    this->y = attribute.getY( index );

    return *this;
}