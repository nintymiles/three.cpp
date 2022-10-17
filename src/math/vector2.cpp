#include "vector2.h"
#include "buffer_attribute.h"

template<typename T>
Vector2& Vector2::fromBufferAttribute(BufferAttribute<T>& attribute, int index) {
    this->x = attribute.getX( index );
    this->y = attribute.getY( index );

    return *this;
}