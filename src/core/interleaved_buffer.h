//
// Created by SeanR on 2023/10/15.
//

#ifndef THREE_CPP_INTERLEAVED_BUFFER_H
#define THREE_CPP_INTERLEAVED_BUFFER_H

#include "buffer_attribute.h"

#include <vector>
#include <memory>

template<typename T>
class InterleavedBuffer : public BufferAttribute<T> {
public:
    using sptr = std::shared_ptr<InterleavedBuffer>;
    std::vector<T> _array;

    unsigned stride;

    InterleavedBuffer() : BufferAttribute<T>(), stride(0) {
        this->usage = Usage::StaticDrawUsage;
    }

    InterleavedBuffer(const InterleavedBuffer& source) : BufferAttribute<T>(source) {
        stride = source.stride;
    }

    InterleavedBuffer(const std::vector<T>& array, unsigned stride) : _array(array), stride(stride){
        this->updateRange.start = 0;
        this->updateRange.count = -1;
        this->count = array.size() / stride;
    }

    static sptr create() {
        return std::make_shared<InterleavedBuffer>();
    }

    static sptr create(const std::vector<T>& array, unsigned stride) {
        return std::make_shared<InterleavedBuffer>(array, stride);
    }

    InterleavedBuffer& copyAt(unsigned index1, const InterleavedBuffer& attribute, unsigned index2) {
        index1 *= stride;
        index2 *= attribute.stride;

        if (_array.size() < index1 + stride) {
            _array.resize(index1 + stride);
        }
        for (unsigned i = 0; i < stride; i++) {

            _array[index1 + i] = attribute._array[index2 + i];
        }
        return *this;
    }

    InterleavedBuffer& set(std::vector<T> value, unsigned offset) {
        for (unsigned i = offset; i < value.size(); i++) {
            _array.push_back(value[i - offset]);
        }

        return *this;
    }

    InterleavedBuffer* clone() {
        return new InterleavedBuffer(*this);
    }

    InterleavedBuffer& copy(const InterleavedBuffer& source) {
        BufferAttribute<T>::copy(source);
        if (source._array.size() > 0) {
            _array.reset(source._array.size());
            std::copy(source._array.begin(), source._array.end(), _array.begin());
        }
        stride = source = stride;
        return *this;
    }
};

#endif //THREE_CPP_INTERLEAVED_BUFFER_H
