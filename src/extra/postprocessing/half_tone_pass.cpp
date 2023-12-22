//
// Created by SeanR on 2023/12/21.
//

#include "half_tone_pass.h"

#include "shader_data.h"
#include "gl_renderer.h"
#include "gl_render_target.h"

namespace threecpp{

HalfTonePass::HalfTonePass(int width, int height,
                           std::unordered_map<std::string, std::variant<float, int, bool>> params) {
    material = ShaderData::getHalfToneShader();

    uniforms = material->uniforms;
    uniforms->set("width",(float)width);
    uniforms->set("height",(float)height);
    for(auto pair:params){
        int *iVal = std::get_if<int>(&pair.second);
        if(iVal)
            uniforms->set(pair.first,*iVal);
        float *fVal = std::get_if<float>(&pair.second);
        if(fVal)
            uniforms->set(pair.first,*fVal);
        bool *bVal = std::get_if<bool>(&pair.second);
        if(bVal)
            uniforms->set(pair.first,*bVal);
    }

    fsQuad = std::make_shared<FullScreenQuad>(material);
}

HalfTonePass &HalfTonePass::render(std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                                   std::shared_ptr<GLRenderTarget> readBuffer) {
    material->uniforms->set("tDiffuse",readBuffer->texture);

    if ( renderToScreen ) {
        renderer->setRenderTarget(nullptr );
        fsQuad->render( renderer );
    } else {
        renderer->setRenderTarget( writeBuffer );
        if ( clear ) renderer->clear();
        fsQuad->render( renderer );
    }

    return *this;
}

void HalfTonePass::setSize(int width, int height) {
    uniforms->set("width",(float)width);
    uniforms->set("height",(float)height);
}

}