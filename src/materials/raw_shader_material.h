//
// Created by SeanR on 2023/11/3.
//

#ifndef THREE_CPP_RAW_SHADER_MATERIAL_H
#define THREE_CPP_RAW_SHADER_MATERIAL_H

#include "shader_material.h"

class RawShaderMaterial : public ShaderMaterial {
public:
    using sptr = std::shared_ptr<RawShaderMaterial>;
    RawShaderMaterial() :ShaderMaterial() {
        type = "RawShaderMaterial";
        isRawShaderMaterial = true;
    }

    static sptr create() {
        return std::make_shared<RawShaderMaterial>();
    }
};

#endif //THREE_CPP_RAW_SHADER_MATERIAL_H
