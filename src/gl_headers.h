//
// Created by SeanR on 2023/10/11.
//

#ifndef THREE_CPP_GL_HEADERS_H
#define THREE_CPP_GL_HEADERS_H

#ifdef IMGUI_IMPL_OPENGL_ES3
#include <GLES3/gl3.h>
#else  //注意此处如果使用#elif,会导致C1017编译错误
#include <glad/glad.h>
#endif

#endif //THREE_CPP_GL_HEADERS_H
