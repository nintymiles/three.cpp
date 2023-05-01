//
// Created by SeanRen on 2023/5/1.
//

#ifndef THREE_CPP_OPENGL_BUFFER_RENDERER_H
#define THREE_CPP_OPENGL_BUFFER_RENDERER_H

#include<GLES3/gl3.h>

class OpenGLIndexedBufferRenderer{
private:
    GLenum mode;

public:
    OpenGLIndexedBufferRenderer& setMode(GLenum modeVal){
        mode = modeVal;
        return *this;
    }

    OpenGLIndexedBufferRenderer& render(int start,int count){
        glDrawArrays(mode,start,count);
        return *this;
    }

    OpenGLIndexedBufferRenderer& renderInstances(int start,int count,int instanceCount){
        glDrawArraysInstanced(mode,start,count, instanceCount);
        return *this;
    }
};

#endif //THREE_CPP_OPENGL_BUFFER_RENDERER_H
