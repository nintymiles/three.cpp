//
// Created by SeanR on 2023/12/19.
//

#ifndef THREE_CPP_SHADER_DATA_H
#define THREE_CPP_SHADER_DATA_H

#include "uniform_values.h"

#include <string>
#include <unordered_map>

struct ShaderData{
    std::unordered_map<std::string,float> definesFloat;
    std::unordered_map<std::string,int> definesInt;

    UniformValues::sptr uniforms;
    std::string vertexShader;
    std::string fragmentShader;

    using sptr = std::shared_ptr<ShaderData>;
};

#endif //THREE_CPP_SHADER_DATA_H
