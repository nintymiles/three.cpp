//
// Created by SeanR on 12/28/2023.
//

#include "after_image_pass.h"

#include "shader_data.h"
#include "gl_render_target.h"

#include "mesh_basic_material.h"
#include "gl_renderer.h"

namespace threecpp {
    AfterImagePass::AfterImagePass(int width,int height,float damp) {
        material = ShaderData::getAfterImageShader();

        uniforms = material->uniforms;
        uniforms->set("damp", damp);

        textureComp = GLRenderTarget::create( width,height );
        textureComp->texture = Texture::create();
        textureOld = GLRenderTarget::create( width,height );
        textureOld->texture = Texture::create();

        compFsQuad = std::make_shared<FullScreenQuad>( material );
        auto basicMaterial = MeshBasicMaterial::create();
        copyFsQuad = std::make_shared<FullScreenQuad>( basicMaterial );
    }

    AfterImagePass &AfterImagePass::render(std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                                       std::shared_ptr<GLRenderTarget> readBuffer) {

        uniforms->set<Texture::sptr>("tOld", textureOld->texture);
        uniforms->set<Texture::sptr>("tNew", readBuffer->texture);

        renderer->setRenderTarget( textureComp );
        compFsQuad->render( renderer );
        copyFsQuad->getMaterial()->map = textureComp->texture;

        if ( renderToScreen ) {
            renderer->setRenderTarget( nullptr );
            copyFsQuad->render( renderer );

        } else {

            renderer->setRenderTarget( writeBuffer );
            if ( clear ) renderer->clear();
            copyFsQuad->render( renderer );

        } // Swap buffers.


        auto temp = textureOld;
        textureOld = textureComp;
        textureComp = temp; // Now textureOld contains the latest image, ready for the next frame.

        return *this;
    }

    void AfterImagePass::setSize(int width, int height) {
        textureComp->setSize( width, height );
        textureOld->setSize( width, height );
    }

}

