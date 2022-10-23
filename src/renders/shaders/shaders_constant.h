//
// Created by SeanR on 2022/10/20.
//

#ifndef THREE_CPP_SRC_RENDERS_SHADERS_SHADERS_CONSTANT_H
#define THREE_CPP_SRC_RENDERS_SHADERS_SHADERS_CONSTANT_H

#include <string>

std::string default_vertex = " void main() {"
                             "   gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );"
                             "}";

std::string default_fragment = " void main() {"
                             "gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );"
                             "}";

#endif //THREE_CPP_SRC_RENDERS_SHADERS_SHADERS_CONSTANT_H
