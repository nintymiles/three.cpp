//
// Created by SeanR on 2023/12/19.
//

#include "effect_composer.h"

#include "gl_renderer.h"
#include "gl_render_target.h"
#include "shader_data.h"
#include "shader_pass.h"

#include "mask_pass.h"

#include <algorithm>

namespace threecpp {
    EffectComposer::EffectComposer(std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> renderTarget)
            : renderer(renderer) {

        if (!renderTarget) {
            Vector2 size{};
            renderer->getSize(size);
            _pixelRatio = renderer->getPixelRatio();
            _width = size.x;
            _height = size.y;

            renderTarget = GLRenderTarget::create(_width * _pixelRatio, _height * _pixelRatio);
            renderTarget->texture = Texture::create();
            renderTarget->texture->name = "EffectComposer.rt1";

        } else {
            _pixelRatio = 1;
            _width = renderTarget->width;
            _height = renderTarget->height;

        }

        renderTarget1 = renderTarget;

        renderTarget2 = GLRenderTarget::create();
        renderTarget2->copy(*renderTarget1);
        renderTarget2->texture = Texture::create();
        renderTarget2->texture->name = "EffectComposer.rt2";

        writeBuffer = renderTarget1;
        readBuffer = renderTarget2;

        renderToScreen = true;

        passes = {};

        copyPass = std::make_shared<ShaderPass>(ShaderData::getCopyShader());

        clock = Timer();
    }

    EffectComposer& EffectComposer::swapBuffers(){
        auto tmp = readBuffer;
        readBuffer = writeBuffer;
        writeBuffer = tmp;

        return *this;
    }

    EffectComposer& EffectComposer::addPass( std::shared_ptr<Pass> pass ){
        passes.push_back( pass );
        pass->setSize( _width * _pixelRatio, _height * _pixelRatio );
        return *this;
    }

    EffectComposer& EffectComposer::insertPass( std::shared_ptr<Pass> pass, int index ){
        passes.insert( passes.begin() + index, pass );
        pass->setSize( _width * _pixelRatio, _height * _pixelRatio  );

        return *this;
    }

    EffectComposer& EffectComposer::removePass( std::shared_ptr<Pass> pass ){
        auto index = std::find(passes.begin(),passes.end(),pass);

        if ( index != passes.end() ) {
            passes.erase( index );
        }

        return *this;
    }

    bool EffectComposer::isLastEnabledPass( int passIndex ){
        for ( int i = passIndex + 1; i < passes.size(); i ++ ) {
            if ( passes[ i ]->enabled ) {
                return false;
            }

        }

        return true;
    }

    EffectComposer& EffectComposer::render( float deltaTime ){
        // deltaTime value is in seconds
        if ( deltaTime <= 0 ) {
            deltaTime = clock.getDelta();
        }

        auto currentRenderTarget = renderer->getRenderTarget();

        auto maskActive = false;

        for ( int i = 0, il = passes.size(); i < il; i ++ ) {
            auto pass = passes[ i ];

            if ( pass->enabled == false ) continue;

            pass->renderToScreen = ( renderToScreen && isLastEnabledPass( i ) );
            pass->render( renderer, writeBuffer, readBuffer /*, deltaTime, maskActive*/ );

            if ( pass->needsSwap ) {
                if ( maskActive ) {
                    //const context = renderer->getContext();
                    auto stencil = renderer->state->stencilBuffer;

                    //context.stencilFunc( context.NOTEQUAL, 1, 0xffffffff );
                    stencil.setFunc( StencilFunc::NotEqualStencilFunc, 1, 0xffffffff );

                    copyPass->render( renderer, writeBuffer, readBuffer/*, deltaTime*/ );

                    //context.stencilFunc( context.EQUAL, 1, 0xffffffff );
                    stencil.setFunc( StencilFunc::EqualStencilFunc, 1, 0xffffffff );
                }

                swapBuffers();

            }


            if ( std::dynamic_pointer_cast<MaskPass>(pass) ) {
                maskActive = true;
            } else if ( std::dynamic_pointer_cast<ClearMaskPass>(pass) ) {
                maskActive = false;
            }


        }

        renderer->setRenderTarget( currentRenderTarget );

        return *this;
    }


    EffectComposer& EffectComposer::reset( std::shared_ptr<GLRenderTarget> renderTarget ){
        return *this;
    }

    EffectComposer& EffectComposer::setSize( int width, int height ){
        return *this;
    }

    EffectComposer& EffectComposer::setPixelRatio( float pixelRatio ){
        return *this;
    }

    void EffectComposer::dispose(){

    }


}








//EffectComposer& setPixelRatio( float pixelRatio );
//
//void dispose();



//    render( deltaTime ) {

//
//    }
//
//    reset( renderTarget ) {
//
//            if ( renderTarget === undefined ) {
//
//                const size = this.renderer.getSize( new Vector2() );
//                this._pixelRatio = this.renderer.getPixelRatio();
//                this._width = size.width;
//                this._height = size.height;
//
//                renderTarget = this.renderTarget1.clone();
//                renderTarget.setSize( this._width * this._pixelRatio, this._height * this._pixelRatio );
//
//            }
//
//            this.renderTarget1.dispose();
//            this.renderTarget2.dispose();
//            this.renderTarget1 = renderTarget;
//            this.renderTarget2 = renderTarget.clone();
//
//            this.writeBuffer = this.renderTarget1;
//            this.readBuffer = this.renderTarget2;
//
//    }
//
//    setSize( width, height ) {
//
//        this._width = width;
//        this._height = height;
//
//        const effectiveWidth = this._width * this._pixelRatio;
//        const effectiveHeight = this._height * this._pixelRatio;
//
//        this.renderTarget1.setSize( effectiveWidth, effectiveHeight );
//        this.renderTarget2.setSize( effectiveWidth, effectiveHeight );
//
//        for ( let i = 0; i < this.passes.length; i ++ ) {
//
//            this.passes[ i ].setSize( effectiveWidth, effectiveHeight );
//
//        }
//
//    }
//
//    setPixelRatio( pixelRatio ) {
//
//            this._pixelRatio = pixelRatio;
//
//            this.setSize( this._width, this._height );
//
//    }
//
//    dispose() {
//
//        this.renderTarget1.dispose();
//        this.renderTarget2.dispose();
//
//        this.copyPass.dispose();
//
//    }
//
//}