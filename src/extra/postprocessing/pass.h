//
// Created by SeanR on 12/18/2023.
//

#ifndef THREE_CPP_PASS_H
#define THREE_CPP_PASS_H

#include <iostream>

#include "material.h"

namespace threecpp{

    class Pass{
        // if set to true, the pass is processed by the composer
        bool enabled;
        // if set to true, the pass indicates to swap read and write buffer after rendering
        bool needsSwap;
        // if set to true, the pass clears its buffer before rendering
        bool clear;
        // if set to true, the result of the pass is rendered to screen. This is set automatically by EffectComposer.
        bool renderToScreen;

        Pass():enabled(true),needsSwap(true),clear(true),renderToScreen(false){};

        virtual void setSize() {}

        virtual void render() {
           std::cout << ( 'THREECPP.Pass: .render() must be implemented in derived pass.' ) << std::endl;

        }
    };

    class GLRenderer;
    class FullScreenQuad {
        FullScreenQuad(Material::sptr material);

        void render(std::shared_ptr<GLRenderer> renderer);

        Material::sptr getMaterial();

        void setMaterial(Material::sptr material);

        void dispose();

    };


}

#endif //THREE_CPP_PASS_H
