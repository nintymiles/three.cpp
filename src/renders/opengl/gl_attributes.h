//
// Created by SeanR on 2023/10/15.
//

#ifndef THREE_CPP_GL_ATTRIBUTES_H
#define THREE_CPP_GL_ATTRIBUTES_H

#include "gl_headers.h"

#include "sole.h"
#include "attribute.h"
#include "buffer_attribute.h"
#include "interleaved_buffer_attribute.h"
#include "common_types.h"
#include "osdecl.h"

#include <unordered_map>
#include <memory>
#include <cstdint>

class GLAttributes {
    bool isGLES3 = false;

    std::unordered_map<sole::uuid, Attribute> buffers;
public:
    using sptr = std::shared_ptr<GLAttributes>;

    GLAttributes(bool isGLES3=false):isGLES3(isGLES3){}

    ~GLAttributes() {
        for (auto attr = buffers.begin();attr != buffers.end();) {
            GLuint buffer = attr->second.buffer;
            glDeleteBuffers(1, &buffer);
            attr++;
        }
    }

    template<typename T>
    void createBuffer(Attribute& attributeBuffer,BufferAttribute<T>& attribute, unsigned bufferType){
        T* array;
        bool isInterleavedBuffer = false;
        InterleavedBufferAttribute<T>* interleaved;

        if (instanceOf<InterleavedBufferAttribute<T>>(&attribute)) {
            interleaved = dynamic_cast<InterleavedBufferAttribute<T> *>(&attribute);
            array = &(interleaved->data->_array[0]);
            isInterleavedBuffer = true;
        }else {
            array = &(attribute.array[0]);
        }
        Usage usage = attribute.usage;

        GLuint buffer;
        int bytePerElement = sizeof(T);
        GLsizeiptr ptr;
        glGenBuffers(1, &buffer);
        glBindBuffer(bufferType, buffer);

        unsigned type = (unsigned)BufferUsageHint::Float;

        if (typeid(float)==typeid(T)) {
            if (!isInterleavedBuffer) {
                glBufferData(bufferType, attribute.size * sizeof(float), &(attribute.array[0]), (GLenum)usage);
            }
            else {
                glBufferData(bufferType, interleaved->data->_array.size() * sizeof(float), array, (GLenum)usage);
            }
            type = (unsigned)BufferUsageHint::Float;

        }
        else if (typeid(uint16_t) == typeid(T)) {
            glBufferData(bufferType, attribute.size * sizeof(uint16_t), array, (GLenum)usage);
            type = (unsigned)BufferUsageHint::UnsignedShort;
        }
        else if (typeid(int16_t) == typeid(T)) {
            glBufferData(bufferType, attribute.size * sizeof(int16_t), array, (GLenum)usage);
            type = (unsigned)BufferUsageHint::Short;
        }
        else if (typeid(uint32_t) == typeid(T)) {
            glBufferData(bufferType, attribute.size * sizeof(uint32_t), array, (GLenum)usage);
            type = (unsigned)BufferUsageHint::UnsignedInt;
        }
        else if (typeid(int32_t)==typeid(T)) {
            glBufferData(bufferType, attribute.size * sizeof(int32_t), array, (GLenum)usage);
            type = (unsigned)BufferUsageHint::Int;
        }
        else if (typeid(int8_t)== typeid(T)) {
            glBufferData(bufferType, attribute.size * sizeof(int8_t), array, (GLenum)usage);
            type = (unsigned)BufferUsageHint::Byte;
        }
        else if (typeid(uint8_t)==typeid(T)) {
            glBufferData(bufferType, attribute.size * sizeof(uint8_t), array, (GLenum)usage);
            type = (unsigned)BufferUsageHint::UnsignedByte;
        }

        attributeBuffer.set(buffer, type, bytePerElement, attribute.version, false);
    }

    template<typename T>
    void updateBuffer(unsigned buffer, BufferAttribute<T>& attribute, unsigned bufferType)
    {
        T* array;

        if (instanceOf<InterleavedBufferAttribute<T>>(&attribute)) {
            InterleavedBufferAttribute<T>* interleaved;
            interleaved = dynamic_cast<InterleavedBufferAttribute<T>*>(&attribute);
            array = &(interleaved->data->_array[0]);
        }
        else {
            array = &(attribute.array[0]);
        }

        threecpp::UpdateRange updateRange = attribute.updateRange;

        glBindBuffer(bufferType, buffer);


        if (updateRange.count == -1) {

            glBufferSubData(bufferType, 0, attribute.size * sizeof(T), array);

        }
        else {

            glBufferSubData(bufferType, updateRange.start * sizeof(T), updateRange.count * sizeof(T), &array[updateRange.start]);

            attribute.updateRange = -1;
        }


    }

    template<typename T>
    Attribute& get(const BufferAttribute<T>& attribute){
        return buffers[attribute.uuid];
    }

    //void remove(std::string attribute);

    template<typename T>
    void update(BufferAttribute<T>& attribute, unsigned bufferType){
        Attribute data;

        auto found = buffers.find(attribute.uuid);

        if (found == buffers.end()) {
            if(attribute.array.size()>0)
                createBuffer(buffers[attribute.uuid], attribute, bufferType);
        }
        else {
            Attribute& buffer = found->second;
            if (buffer.version < attribute.version) {
                updateBuffer<T>(buffer.buffer, attribute, bufferType);
                buffer.version = attribute.version;
            }
        }
    }

    template<typename T>
    void remove(BufferAttribute<T>& attribute) {
        if (buffers.find(attribute.uuid) != buffers.end()) {
            const Attribute& data = buffers[attribute.uuid];
            glDeleteBuffers(1, &data.buffer);
            buffers.erase(attribute.uuid);
        }
    }
};

#endif //THREE_CPP_GL_ATTRIBUTES_H
