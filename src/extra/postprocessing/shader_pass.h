//
// Created by SeanR on 2023/12/19.
//

#ifndef THREE_CPP_SHADER_PASS_H
#define THREE_CPP_SHADER_PASS_H

#include "pass.h"

#include "shader_material.h"

class Material;
namespace threecpp {

    class ShaderPass : public Pass {
        std::string textureId;

        std::shared_ptr<Material> material;
        std::shared_ptr<FullScreenQuad> fsQuad;

    public:
        std::shared_ptr<UniformValues> uniforms;

        ShaderPass(ShaderMaterial::sptr shader,std::string textureId = "");

        ShaderPass& render( std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                            std::shared_ptr<GLRenderTarget> readBuffer );

        //void dispose();
    };

}


#endif //THREE_CPP_SHADER_PASS_H
