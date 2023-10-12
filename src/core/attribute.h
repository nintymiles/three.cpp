//
// Created by SeanR on 2023/10/12.
//

#ifndef THREE_CPP_ATTRIBUTE_H
#define THREE_CPP_ATTRIBUTE_H

#include <memory>

class Attribute {
public:
    unsigned buffer;
    unsigned type;
    unsigned bytesPerElement;
    unsigned version;
    bool isEmpty;

    using ptr = std::shared_ptr<Attribute>;

    Attribute() : buffer(0), type(0), bytesPerElement(0), version(0), isEmpty(true) {}

    Attribute(unsigned buffer, unsigned type, unsigned bytesPerElement, unsigned version, bool isEmpty)
            : buffer(buffer), type(type), bytesPerElement(bytesPerElement), version(version), isEmpty(isEmpty) {}
    Attribute(const Attribute& source) {
        buffer = source.buffer;
        type = source.type;
        bytesPerElement = source.bytesPerElement;
        version = source.version;
        isEmpty = source.isEmpty;
    }
    void set(unsigned buffer, unsigned type, unsigned bytesPerElement, unsigned version, bool isEmpty) {
        this->buffer = buffer;
        this->type = type;
        this->bytesPerElement = bytesPerElement;
        this->version = version;
        this->isEmpty = isEmpty;
    }
};

#endif //THREE_CPP_ATTRIBUTE_H
