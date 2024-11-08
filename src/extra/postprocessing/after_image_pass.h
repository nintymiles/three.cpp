//
// Created by SeanR on 12/28/2023.
//

#ifndef THREE_CPP_AFTER_IMAGE_PASS_H
#define THREE_CPP_AFTER_IMAGE_PASS_H


#include "pass.h"

#include "shader_material.h"

#include <unordered_map>
#include <variant>
#include <memory>

class GLRenderTarget;
namespace threecpp {

    class AfterImagePass : public Pass {
        ShaderMaterial::sptr material;
        std::shared_ptr<FullScreenQuad> compFsQuad,copyFsQuad;
        std::shared_ptr<GLRenderTarget> textureComp,textureOld;


    public:
        std::shared_ptr<UniformValues> uniforms;

        AfterImagePass(int width,int height,float damp = 0.96);

        AfterImagePass& render( std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                              std::shared_ptr<GLRenderTarget> readBuffer/*, deltaTime, maskActive */) override;

        void setSize(int width,int height) override;
    };

}


#endif //THREE_CPP_AFTER_IMAGE_PASS_H
