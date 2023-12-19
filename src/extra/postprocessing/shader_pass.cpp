//
// Created by SeanR on 2023/12/19.
//

#include "shader_pass.h"
#include "uniform_values.h"
#include "material.h"
#include "gl_renderer.h"
#include "gl_render_target.h"

namespace threecpp {

    ShaderPass::ShaderPass(ShaderMaterial::sptr shader, std::string textureId):Pass() {
        this->textureId = textureId.empty()?"tDiffuse":textureId;

        if(shader->type == "ShaderMaterial"){
            uniforms = shader->uniforms;

            material = shader;
        }
        else if ( shader ) {
            //todo:fix uniformValues needs clone?
            uniforms = shader->uniforms;
            material = ShaderMaterial::create();
            material->definesFloat = shader->definesFloat;
            material->definesString = shader->definesString;
            material->uniforms = uniforms;
            material->vertexShader = shader->vertexShader;
            material->fragmentShader = shader->fragmentShader;

        }

        fsQuad = std::make_shared<FullScreenQuad>( material );

    }

    ShaderPass& ShaderPass::render( std::shared_ptr<GLRenderer> renderer, std::shared_ptr<GLRenderTarget> writeBuffer,
                                                        std::shared_ptr<GLRenderTarget> readBuffer) {

        uniforms->set( textureId,readBuffer->texture );


        fsQuad->setMaterial( material );

        if ( renderToScreen ) {
            renderer->setRenderTarget( nullptr );
            fsQuad->render( renderer );

        } else {
            renderer->setRenderTarget( writeBuffer );
            // TODO: Avoid using autoClear properties, see https://github.com/mrdoob/three.js/pull/15571#issuecomment-465669600
            if ( clear ) renderer->clear( renderer->autoClearColor, renderer->autoClearDepth, renderer->autoClearStencil );
            fsQuad->render( renderer );

        }

        return *this;
    }

}