//
// Created by SeanR on 2023/12/18.
//

#include "render_pass.h"
#include "gl_renderer.h"
#include "gl_render_target.h"

threecpp::RenderPass& threecpp::RenderPass::render( std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                                   std::shared_ptr<GLRenderTarget> readBuffer) {

    auto oldAutoClear = renderer->autoClear;
    renderer->autoClear = false;
    bool oldClearAlpha;
    Material::sptr oldOverrideMaterial;

    if ( overrideMaterial ) {
        oldOverrideMaterial = scene->overrideMaterial;
        scene->overrideMaterial = overrideMaterial;
    }

    if ( !this->clearColor.isNull() ) {
        _oldClearColor = renderer->getClearColor();
        oldClearAlpha = renderer->getClearAlpha();
        renderer->setClearColor( clearColor, clearAlpha );

    }

    if ( this->clearDepth ) {
        renderer->clearDepth();
    }

    renderer->setRenderTarget( renderToScreen ? nullptr : readBuffer ); // TODO: Avoid using autoClear properties, see https://github.com/mrdoob/three.js/pull/15571#issuecomment-465669600

    if ( clear ) renderer->clear( renderer->autoClearColor, renderer->autoClearDepth, renderer->autoClearStencil );
    renderer->render( scene, camera );

    if ( !this->clearColor.isNull() ) {
        renderer->setClearColor( _oldClearColor, oldClearAlpha );
    }

    if ( overrideMaterial ) {
        scene->overrideMaterial = oldOverrideMaterial;
    }

    renderer->autoClear = oldAutoClear;

    return *this;
}
