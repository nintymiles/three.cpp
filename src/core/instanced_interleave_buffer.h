//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_INSTANCED_INTERLEAVE_BUFFER_H
#define THREE_CPP_INSTANCED_INTERLEAVE_BUFFER_H

#include "interleaved_buffer.h"

template<typename T>
class InstancedInterleavedBuffer : public InterleavedBuffer<T> {
public :
    using ptr = std::shared_ptr<InstancedInterleavedBuffer>;
    int meshPerAttribute = 1;

    InstancedInterleavedBuffer() : InterleavedBuffer<T>(), meshPerAttribute(1) {}

    InstancedInterleavedBuffer(const std::vector<T>& array, unsigned stride,int meshPerAttribute=1) : InterleavedBuffer<T>(array,stride) {
    }
    InstancedInterleavedBuffer(const InstancedInterleavedBuffer& source) : InterleavedBuffer<T>(source) {
        meshPerAttribute = source.meshPerAttribute;
    }
    InstancedInterleavedBuffer* clone() {
        return new InstancedInterleavedBuffer(*this);
    }
    InstancedInterleavedBuffer& copy(const InstancedInterleavedBuffer& source) {
        InterleavedBuffer<T>::copy(source);
        meshPerAttribute = source.meshPerAttribute;
        return *this;
    }
};

#endif //THREE_CPP_INSTANCED_INTERLEAVE_BUFFER_H
