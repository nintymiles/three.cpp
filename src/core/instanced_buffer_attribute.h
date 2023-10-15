//
// Created by SeanR on 2023/10/15.
//

#ifndef THREE_CPP_INSTANCED_BUFFER_ATTRIBUTE_H
#define THREE_CPP_INSTANCED_BUFFER_ATTRIBUTE_H

#include "buffer_attribute.h"

#include <memory>

template<typename T>
class InstancedBufferAttribute : public BufferAttribute<T> {
public :
    using sptr = std::shared_ptr<InstancedBufferAttribute>;

    int meshPerAttribute = 1;

    InstancedBufferAttribute() : BufferAttribute(), meshPerAttribute(1) {}

    InstancedBufferAttribute(std::vector<T> array, unsigned itemSize, bool normalized, int meshPerAttribute) : BufferAttribute(array, itemSize, normalized), meshPerAttribute(meshPerAttribute){}

    InstancedBufferAttribute(const InstancedBufferAttribute& source) : BufferAttribute(source) {
        meshPerAttribute = source.meshPerAttribute;
    }
    InstancedBufferAttribute* clone() {
        return new InstancedBufferAttribute(*this);
    }
    InstancedBufferAttribute* copy(const InstancedBufferAttribute& source) {
        BufferAttribute::copy(source);
        meshPerAttribute = source.meshPerAttribute;
        return *this;
    }

};

#endif //THREE_CPP_INSTANCED_BUFFER_ATTRIBUTE_H
