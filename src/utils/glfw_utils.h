#ifndef SRC_UTILS_GLFW_UTILS_H_
#define SRC_UTILS_GLFW_UTILS_H_

#include "app_env.h"

void glfw_basic_setting() {
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 300 es"; //version必须匹配
    //EGL也必须在mac上指定，否则glfw提示不存在，能找到EGL lib，意味着从/usr/local/lib中获得
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GLES 3.0
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); //目前angle最高可以指定到opengl es 3.1
    // GL 3.0 + GLSL 130
//    const char* glsl_version = "#version 300 es";
//    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,1);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);
//    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
//    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only


#endif
}

constexpr char* GLSL_VERSION(){
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    retun "#version 100";
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    return "#version 300 es"; //version必须匹配
#else
    // GLES 3.0
    return "#version 300 es";  
#endif
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void printGLDriverInfo(std::ostream& out) {
    out << "GL_VERSION                  : " << glGetString(GL_VERSION) << std::endl;
    out << "GL_VENDOR                   : " << glGetString(GL_VENDOR) << std::endl;
    out << "GL_RENDERER                 : " << glGetString(GL_RENDERER) << std::endl;
    out << "GL_SHADING_LANGUAGE_VERSION : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    out << "GLFW_Info                 : " << glfwGetVersionString() << std::endl;
}

#endif /* SRC_UTILS_GLFW_UTILS_H_ */
