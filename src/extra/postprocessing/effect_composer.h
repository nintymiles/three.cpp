//
// Created by SeanR on 2023/12/19.
//

#ifndef THREE_CPP_EFFECT_COMPOSER_H
#define THREE_CPP_EFFECT_COMPOSER_H

#include "timer.h"

#include <memory>
#include <vector>

class GLRenderer;
class GLRenderTarget;
class RenderPass;
class EffectComposer {
    std::shared_ptr<GLRenderer> renderer;
    std::shared_ptr<GLRenderTarget> renderTarget1,renderTarget2;
    float _pixelRatio;
    int _width,_height;
    std::shared_ptr<GLRenderTarget> writeBuffer,readBuffer;
    bool renderToScreen;
    std::vector<std::shared_ptr<RenderPass>> passes;
    std::shared_ptr<RenderPass> copyPass;
    Timer clock;

public:
    EffectComposer(std::shared_ptr<GLRenderer> renderer,std::shared_ptr<GLRenderTarget> renderTarget);

    EffectComposer& swapBuffers();

    EffectComposer& addPass( std::shared_ptr<RenderPass> pass );

    EffectComposer& insertPass( std::shared_ptr<RenderPass> pass, int index );

    EffectComposer& removePass( std::shared_ptr<RenderPass> pass );

    bool isLastEnabledPass( int passIndex );

    EffectComposer& render( float deltaTime );

    EffectComposer& reset( std::shared_ptr<GLRenderTarget> renderTarget );

    EffectComposer& setSize( int width, int height );

    EffectComposer& setPixelRatio( float pixelRatio );

    void dispose();

};

#endif //THREE_CPP_EFFECT_COMPOSER_H
