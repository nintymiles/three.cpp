//
// Created by SeanR on 2023/6/26.
//

#ifndef THREE_CPP_GL_BUFFER_RENDERER_H
#define THREE_CPP_GL_BUFFER_RENDERER_H

#include <GLES3/gl3.h>
#include <memory>

#include "gl_info.h"
#include "gl_extensions.h"
#include "gl_capabilities.h"

#include "attribute.h"

#include "common_utils.h"

class GLBufferRendererInterface {

public :
    bool isGLES3 ;

    GLExtensions::sptr extensions;

    GLInfo::sptr info;

    GLCapabilities::sptr capabilities;

    unsigned type = 0;

    unsigned bytesPerElement = 4;

    GLenum mode = 0;

    using ptr = std::shared_ptr<GLBufferRendererInterface>;

    GLBufferRendererInterface(bool isGLES3 = false) : isGLES3(isGLES3) {}
    GLBufferRendererInterface(GLExtensions::sptr& extension, GLInfo::sptr& info, GLCapabilities::sptr& capabilities)
            :extensions(extensions), info(info), capabilities(capabilities) {
        isGLES3 = capabilities->isGLES3;
    }
    virtual ~GLBufferRendererInterface() = default;

    virtual void setMode(const GLenum value) {}

    virtual void setIndex(Attribute& value) {}

    virtual void render(const GLint start, const GLsizei count){
        threecpp::checkError();
    }

    virtual void renderInstances(const Geometry& geometry, const GLint start, const GLsizei count, const GLint primcount) {
        threecpp::checkError();
    }

    virtual GLBufferRendererInterface& copy(const GLBufferRendererInterface& source) {
        isGLES3 = source.isGLES3;
        mode = source.mode;
        type = source.type;
        bytesPerElement = source.bytesPerElement;
        extensions = source.extensions;
        info = source.info;
        capabilities = source.capabilities;
        return *this;
    }

    virtual GLBufferRendererInterface& operator = (const GLBufferRendererInterface& source) {
        return copy(source);
    }
};

class GLBufferRenderer : public GLBufferRendererInterface {

public:


    GLBufferRenderer(bool isGLES3 = false) : GLBufferRendererInterface(isGLES3) {}

    GLBufferRenderer(GLExtensions::sptr& extension, GLInfo::sptr& info, GLCapabilities::sptr& capabilities)
            : GLBufferRendererInterface(extension,info, capabilities){}

    virtual void setMode(const GLenum value) override {
        mode = value;
    }

    virtual void render(const GLint start, const GLsizei count) override {
        glDrawArrays(mode, start, count);
        info->update(count, (unsigned)mode);
        GLBufferRendererInterface::render(start, count);
    }

    virtual void renderInstances(const Geometry& geometry,const GLint start,const GLsizei count,const GLint primcount) override {
        if (primcount == 0) return;

        glDrawArraysInstanced(mode, start, count, primcount);

        info->update(count, (int)mode, primcount);
        GLBufferRendererInterface::renderInstances(geometry,start, count,primcount);
    }

    virtual GLBufferRenderer& copy(const GLBufferRenderer& source)  {
        GLBufferRendererInterface::copy(source);
        return *this;
    }
    virtual GLBufferRenderer& operator = (const GLBufferRenderer& source) {
        return copy(source);
    }
};


//class GLBufferRenderer{
//public:
//    GLBufferRenderer(std::shared_ptr<GLInfo> glInfo):info(glInfo){}
//
//    GLBufferRenderer& setMode(GLenum mode){
//        this->mode = mode;
//        return *this;
//    }
//
//    GLBufferRenderer& render(int start,int count){
//        glDrawArrays(mode,start,count);
//        info->update(count,mode,1);
//    }
//
//    GLBufferRenderer& renderInstances( int start, int count, int primcount ) {
//
//        if ( primcount == 0 ) return *this;
//
////        let extension, methodName;
////
////        if ( isWebGL2 ) {
////
////            extension = gl;
////            methodName = 'drawArraysInstanced';
////
////        } else {
////
////            extension = extensions.get( 'ANGLE_instanced_arrays' );
////            methodName = 'drawArraysInstancedANGLE';
////
////            if ( extension === null ) {
////
////                console.error( 'THREE.WebGLBufferRenderer: using THREE.InstancedBufferGeometry but hardware does not support extension ANGLE_instanced_arrays.' );
////                return;
////
////            }
////
////        }
////
////        extension[ methodName ]( mode, start, count, primcount );
//
//        glDrawArraysInstanced(mode,start,count,primcount);
//        info->update( count, mode, primcount );
//        return *this;
//
//    }
//
//private:
//    GLenum mode;
//    std::shared_ptr<GLInfo> info;
//
//};

#endif //THREE_CPP_GL_BUFFER_RENDERER_H
