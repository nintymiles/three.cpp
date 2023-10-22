//
// Created by SeanR on 2023/6/17.
//

#ifndef THREE_CPP_SHADER_CHUNK_H
#define THREE_CPP_SHADER_CHUNK_H
#include "constants.h"

#include <string>

namespace shaderlib_name {
    ShaderLibID get(std::string name);
}

const char* getShaderChunk(ShaderLibID chunk);


#endif //THREE_CPP_SHADER_CHUNK_H
