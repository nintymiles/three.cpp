//
// Created by SeanR on 1/3/2024.
//

#ifndef THREE_CPP_GLITCH_PASS_H
#define THREE_CPP_GLITCH_PASS_H

#include "pass.h"

#include "shader_material.h"

#include <unordered_map>
#include <variant>

namespace threecpp {


    class GlitchPass : public Pass {
        ShaderMaterial::sptr material;
        std::shared_ptr<FullScreenQuad> fsQuad;

        int curF;
        int randX;

        Texture::sptr generateHeightmap( int dt_size );
        GlitchPass& generateTrigger();
    public:
        bool goWild;
        std::shared_ptr<UniformValues> uniforms;

        GlitchPass(int dt_size = 64);

        GlitchPass& render( std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                              std::shared_ptr<GLRenderTarget> readBuffer/*, deltaTime, maskActive */) override;

        void setSize(int width,int height) override;
    };

}

#endif //THREE_CPP_GLITCH_PASS_H
