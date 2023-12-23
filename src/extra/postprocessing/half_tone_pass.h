//
// Created by SeanR on 2023/12/21.
//

#ifndef THREE_CPP_HALF_TONE_PASS_H
#define THREE_CPP_HALF_TONE_PASS_H

#include "pass.h"

#include "shader_material.h"

#include <unordered_map>
#include <variant>

namespace threecpp {

    /**
    * RGB Halftone pass for effects composer. Requires HalftoneShader.
    */
    class HalfTonePass : public Pass {
        ShaderMaterial::sptr material;
        std::shared_ptr<FullScreenQuad> fsQuad;
    public:
        std::shared_ptr<UniformValues> uniforms;

        HalfTonePass(int width,int height,std::unordered_map<std::string,std::variant<float,int,bool>> params);

        HalfTonePass& render( std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                            std::shared_ptr<GLRenderTarget> readBuffer/*, deltaTime, maskActive */) override;

        void setSize(int width,int height) override;
    };

}


#endif //THREE_CPP_HALF_TONE_PASS_H
