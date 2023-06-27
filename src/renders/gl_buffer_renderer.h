//
// Created by SeanR on 2023/6/26.
//

#ifndef THREE_CPP_GL_BUFFER_RENDERER_H
#define THREE_CPP_GL_BUFFER_RENDERER_H

#include <GLES3/gl3.h>
#include <memory>

#include "gl_info.h"

class GLBufferRenderer{
public:
    GLBufferRenderer(std::shared_ptr<GLInfo> glInfo):info(glInfo){}

    GLBufferRenderer& setMode(GLenum mode){
        this->mode = mode;
        return *this;
    }

    GLBufferRenderer& render(int start,int count){
        glDrawArrays(mode,start,count);
        info->update(count,mode,1);
    }

    GLBufferRenderer& renderInstances( int start, int count, int primcount ) {

        if ( primcount == 0 ) return *this;

//        let extension, methodName;
//
//        if ( isWebGL2 ) {
//
//            extension = gl;
//            methodName = 'drawArraysInstanced';
//
//        } else {
//
//            extension = extensions.get( 'ANGLE_instanced_arrays' );
//            methodName = 'drawArraysInstancedANGLE';
//
//            if ( extension === null ) {
//
//                console.error( 'THREE.WebGLBufferRenderer: using THREE.InstancedBufferGeometry but hardware does not support extension ANGLE_instanced_arrays.' );
//                return;
//
//            }
//
//        }
//
//        extension[ methodName ]( mode, start, count, primcount );

        glDrawArraysInstanced(mode,start,count,primcount);
        info->update( count, mode, primcount );
        return *this;

    }

private:
    GLenum mode;
    std::shared_ptr<GLInfo> info;

};

#endif //THREE_CPP_GL_BUFFER_RENDERER_H
