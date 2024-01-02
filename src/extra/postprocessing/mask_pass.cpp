//
// Created by SeanR on 2023/12/19.
//

#include "mask_pass.h"

#include "gl_render_target.h"
#include "gl_renderer.h"

namespace threecpp{

    MaskPass& MaskPass::render(std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                               std::shared_ptr<GLRenderTarget> readBuffer) {

        //const context = renderer.getContext();
        auto state = renderer->state;

//        state->enable(GL_DEPTH_TEST);
//        state->stencilBuffer.setTest(true);

        // don't update color or depth
        state->colorBuffer.setMask( false );
        state->depthBuffer.setMask( false );

        // lock buffers
        state->colorBuffer.setLocked( true );
        state->depthBuffer.setLocked( true );

        // set up stencil
        int writeValue, clearValue;

        if ( inverse ) {
            writeValue = 0;
            clearValue = 1;

        } else {
            writeValue = 1;
            clearValue = 0;

        }
//        glClear(GL_DEPTH_BUFFER_BIT);
//        glEnable(GL_STENCIL_TEST);
//        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//        glDepthMask(GL_FALSE);
//        glStencilFunc(GL_NEVER, 1, 0xFF);
//        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);  // draw 1s on test fail (always)
//
//        // draw stencil pattern
//        glStencilMask(0xFF);
//        glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF

//        state->depthBuffer.setTest(false);

        state->stencilBuffer.setTest( true );
        //state->depthBuffer.setMask(true);
        state->stencilBuffer.setOp( StencilOp::KeepStencilOp, StencilOp::KeepStencilOp, StencilOp::ReplaceStencilOp );
        state->stencilBuffer.setFunc( StencilFunc::AlwaysStencilFunc, writeValue, 0xff );
        state->stencilBuffer.setMask(0xff);
        state->stencilBuffer.setClear( clearValue );
        state->stencilBuffer.setLocked( true );


        // draw into the stencil buffer
        renderer->setRenderTarget( readBuffer );
        if ( clear ) renderer->clear();
        renderer->render( scene, camera );

        renderer->setRenderTarget( writeBuffer );
        if ( clear ) renderer->clear();
        renderer->render( scene, camera );

        // unlock color and depth buffer for subsequent rendering
        state->colorBuffer.setLocked( false );
        state->depthBuffer.setLocked( false );

        // only render where stencil is set to 1
        state->stencilBuffer.setLocked( false );

        state->stencilBuffer.setFunc( StencilFunc::EqualStencilFunc, 1, 0xff );
        state->stencilBuffer.setMask(0x00);

        // draw if == 1
        state->stencilBuffer.setOp( StencilOp::KeepStencilOp, StencilOp::KeepStencilOp, StencilOp::KeepStencilOp );
        state->stencilBuffer.setLocked( true );

//        state->stencilBuffer.setClear(writeValue);

//        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//        glDepthMask(GL_TRUE);
//        glStencilMask(0x00);
//        // draw where stencil's value is 0
//        glStencilFunc(GL_EQUAL, 0, 0xFF);
//        /* (nothing to draw) */
//        // draw only where stencil's value is 1
//        glStencilFunc(GL_EQUAL, 1, 0xFF);
//        glStencilMask(0xFF);
//        glStencilFunc(GL_ALWAYS, 1, 0xFF);

////        renderer->setRenderTarget( renderToScreen ? nullptr : readBuffer );
////        renderer->render( scene, camera );
//        state->depthBuffer.setTest(false);

        return *this;

    }

    ClearMaskPass &
    ClearMaskPass::render(std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                          std::shared_ptr<GLRenderTarget> readBuffer) {
        renderer->state->stencilBuffer.setLocked( false );
        renderer->state->stencilBuffer.setTest( false );
        return *this;
    }
}