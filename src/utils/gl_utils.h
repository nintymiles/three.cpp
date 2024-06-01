#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <iostream>
#include <stdexcept>
#include <map>
#include <string>

#include "gl_headers.h"

#define GLSL(version, shader) "#version " #version "\n" #shader


// call this function after every OpenGL function call
void checkGlError(const char* op);
void printProgramInfoLog(GLuint program);

// Reads and compiles a pair of vertex shader and fragment shader files into a
// GL shader program. Throws runtime_error on error
void readAndCompileShader(GLuint programHandle,
                          const char *vertexShaderFileName, const char *fragmentShaderFileName);

// Link two compiled vertex shader and fragment shader into a GL shader program
void linkShader(GLuint programHandle, GLuint vertexShaderHandle, GLuint fragmentShaderHandle);

// Reads and compiles a single shader (vertex, fragment, etc) file into a GL
// shader. Throws runtime_error on error
void readAndCompileSingleShader(GLuint shaderHandle, const char* shaderFileName);

void readAndCompileShader(GLuint programHandle, const char * vertexShaderFileName, const char * fragmentShaderFileName,std::map<std::string, std::string> &map_parameters);

// load and compile shader from source by created shaderHandle
void loadAndCompileSingleShader(GLuint shaderHandle, const char* shaderSource);

// Loads and compiles a pair of vertex shader and fragment shader sources
void loadAndCompileShader(GLuint programHandle,const char * vertexShaderSource, const char * fragmentShaderSource);

//void drawTriangleO();

#endif
