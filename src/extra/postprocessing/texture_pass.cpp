//
// Created by SeanR on 2023/12/30.
//

#include "texture_pass.h"

#include "shader_data.h"
#include "gl_renderer.h"
#include "gl_render_target.h"

namespace threecpp {

TexturePass::TexturePass(Texture::sptr map,float opacity):Pass(),map(map),opacity(opacity) {
    material = ShaderData::getCopyShader();
    uniforms = material->uniforms;

    material->depthTest = false;
    material->depthWrite = false;

    needsSwap = false;

    fsQuad = std::make_shared<FullScreenQuad>( nullptr );
}

TexturePass& TexturePass::render( std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                                std::shared_ptr<GLRenderTarget> readBuffer) {

    auto oldAutoClear = renderer->autoClear;
    renderer->autoClear = false;
    fsQuad->setMaterial(material);

    uniforms->set("opacity",opacity);
    uniforms->set("tDiffuse",map);

    material->transparent = opacity < 1.0;
    renderer->setRenderTarget( renderToScreen ? nullptr : readBuffer );
    if ( clear ) renderer->clear();
    fsQuad->render( renderer );
    renderer->autoClear = oldAutoClear;

    return *this;
}

}