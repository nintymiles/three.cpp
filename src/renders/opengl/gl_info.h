//
// Created by SeanR on 2023/6/14.
//

#ifndef THREE_CPP_GL_INFO_H
#define THREE_CPP_GL_INFO_H

#include "gl_program.h"

#include <vector>
#include <GLES3/gl3.h>

class GLInfo{
public:
    struct MemoryInfo{
        int geometries;
        int textures;
    };
    struct RenderInfo{
        int frame;
        int calls;
        int triangles;
        int points;
        int lines;
    };

    MemoryInfo memory;
    RenderInfo render;
    //std::vector<OpenGLProgram> progams;
    bool autoReset = true;
    
    GLInfo& reset(){
        render.calls = 0;
        render.frame = 0;
        render.lines = 0;
        render.points = 0;
        render.triangles = 0;
        
        return *this;
    }
    
    GLInfo& update(int count,GLenum mode,int instanceCount){
        render.calls ++;
        switch (mode) {
            case GL_TRIANGLES:
                render.triangles += instanceCount * ( count / 3 );
                break;

            case GL_LINES:
                render.lines += instanceCount * ( count / 2 );
                break;

            case GL_LINE_STRIP:
                render.lines += instanceCount * ( count - 1 );
                break;

            case GL_LINE_LOOP:
                render.lines += instanceCount * count;
                break;

            case GL_POINTS:
                render.points += instanceCount * count;
                break;

            default:
                break;
            
        }
        return *this;
    }
    

};

#endif //THREE_CPP_GL_INFO_H
