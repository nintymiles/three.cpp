//
// Created by SeanR on 2023/12/30.
//

#ifndef THREE_CPP_TEXTURE_PASS_H
#define THREE_CPP_TEXTURE_PASS_H

#include "pass.h"

#include "texture.h"
#include "uniform_values.h"

class Material;
namespace threecpp {

class TexturePass : public Pass {
    std::string textureId;

    std::shared_ptr<Material> material;
    std::shared_ptr<FullScreenQuad> fsQuad;

    Texture::sptr map;
    float opacity;

public:
    std::shared_ptr<UniformValues> uniforms;

    TexturePass(Texture::sptr map,float opacity = 1.0f);

    TexturePass& render( std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                        std::shared_ptr<GLRenderTarget> readBuffer ) override;

    //void dispose();
};

}


#endif //THREE_CPP_TEXTURE_PASS_H
