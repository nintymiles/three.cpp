//
// Created by SeanR on 2023/10/22.
//
#include "gl_info.h"

#include "gl_headers.h"
#include "gl_program.h"


void GLInfo::update(const unsigned count, const unsigned mode, unsigned instanceCount){
    instanceCount = instanceCount > 0 ? instanceCount : 1;

    render.calls++;

    switch (mode) {
        case GL_TRIANGLES:
            render.triangles += instanceCount * (count / 3);
            break;

        case GL_LINES:
            render.lines += instanceCount * (count / 2);
            break;

        case GL_LINE_STRIP:
            render.lines += instanceCount * (count - 1);
            break;

        case GL_LINE_LOOP:
            render.lines += instanceCount * count;
            break;

        case GL_POINTS:
            render.points += instanceCount * count;
            break;

    }
}

void GLInfo::reset(){
    render.frame++;
    render.calls = 0;
    render.triangles = 0;
    render.points = 0;
    render.lines = 0;
}