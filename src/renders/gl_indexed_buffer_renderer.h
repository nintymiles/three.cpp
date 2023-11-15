//
// Created by SeanR on 2023/10/12.
//

#ifndef THREE_CPP_GL_INDEXED_BUFFER_RENDERER_H
#define THREE_CPP_GL_INDEXED_BUFFER_RENDERER_H

#include "gl_buffer_renderer.h"
#include "gl_headers.h"

class GLIndexedBufferRenderer :public GLBufferRendererInterface{

public :
    GLIndexedBufferRenderer(bool isGLES3 = false) :GLBufferRendererInterface(isGLES3) {}
    GLIndexedBufferRenderer(GLExtensions::sptr& extension, GLInfo::sptr& info, GLCapabilities::sptr& capabilities)
            : GLBufferRendererInterface(extension, info, capabilities) {}

    virtual void setMode(const GLenum value) override {
        mode = value;
    }

    virtual void setIndex(Attribute& value)  override {
        type = value.type;
        bytesPerElement = value.bytesPerElement;
    }
    virtual void render(const GLint start, const GLsizei count) override {
        glDrawElements(mode, count, type, (const unsigned *)(start * bytesPerElement));
        info->update(count, mode,count);
        GLBufferRendererInterface::render(start,count);
    }

    virtual void renderInstances(const Geometry& geometry, const GLint start, const GLsizei count, const GLint primcount) override {
        glDrawElementsInstanced(mode, count, type, (const unsigned *)(start * bytesPerElement), primcount);
        info->update(count, mode, primcount);
        GLBufferRendererInterface::renderInstances(geometry,start, count,primcount);
    }
    GLIndexedBufferRenderer& copy(const GLIndexedBufferRenderer& source) {
        GLBufferRendererInterface::copy(source);
        return *this;
    }
    GLIndexedBufferRenderer& operator = (const GLIndexedBufferRenderer& source) {
        return copy(source);
    }
};

#endif //THREE_CPP_GL_INDEXED_BUFFER_RENDERER_H
