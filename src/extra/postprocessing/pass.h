//
// Created by SeanR on 12/18/2023.
//

#ifndef THREE_CPP_PASS_H
#define THREE_CPP_PASS_H

#include <iostream>

class GLRenderer;
class BufferGeometry;
class Material;
class Mesh;
class GLRenderTarget;
namespace threecpp{

    class Pass{
    public:
        // if set to true, the pass is processed by the composer
        bool enabled;
        // if set to true, the pass indicates to swap read and write buffer after rendering
        bool needsSwap;
        // if set to true, the pass clears its buffer before rendering
        bool clear;
        // if set to true, the result of the pass is rendered to screen. This is set automatically by EffectComposer.
        bool renderToScreen;

        Pass():enabled(true),needsSwap(true),clear(true),renderToScreen(false){}

        //virtual void setSize(int width,int height) {}

        virtual Pass& render(std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                            std::shared_ptr<GLRenderTarget> readBuffer) {
           std::cout << "THREECPP.Pass: .render() must be implemented in derived pass."  << std::endl;

            return *this;
        }
    };


    class FullScreenQuad {
        std::shared_ptr<Mesh> _mesh;
        std::shared_ptr<BufferGeometry> _geometry;

    public:
        FullScreenQuad(std::shared_ptr<Material> material);

        void render(std::shared_ptr<GLRenderer> renderer);

        std::shared_ptr<Material> getMaterial();

        void setMaterial(std::shared_ptr<Material> material);

        void dispose();

    };


}

#endif //THREE_CPP_PASS_H
