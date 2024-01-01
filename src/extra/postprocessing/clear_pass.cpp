//
// Created by SeanR on 2023/12/31.
//

#include "clear_pass.h"

#include "gl_renderer.h"
#include "gl_render_target.h"

namespace threecpp {

ClearPass::ClearPass(Color clearColor,float clearAlpha):Pass(),clearColor(clearColor),clearAlpha(clearAlpha) {

    needsSwap = false;
    _oldClearColor = Color{};

}

ClearPass& ClearPass::render( std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                                  std::shared_ptr<GLRenderTarget> readBuffer) {

    float oldClearAlpha;

    if ( !clearColor.isNull() ) {
        _oldClearColor = renderer->getClearColor();
        oldClearAlpha = renderer->getClearAlpha();
        renderer->setClearColor( clearColor, clearAlpha );

    }

    renderer->setRenderTarget( renderToScreen ? nullptr : readBuffer );
    renderer->clear();

    if ( !clearColor.isNull() ) {
        renderer->setClearColor( _oldClearColor,oldClearAlpha );

    }

    return *this;
}

}