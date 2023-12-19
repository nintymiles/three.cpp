//
// Created by SeanR on 2023/12/19.
//

#ifndef THREE_CPP_COPY_SHADER_H
#define THREE_CPP_COPY_SHADER_H

#include "shader_material.h"
#include "uniform_values.h"

namespace shader_data {
    ShaderMaterial::sptr initCopyShader(){
        ShaderMaterial::sptr copyShader = std::make_shared<ShaderMaterial>();

        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        uniforms->set<Texture::sptr>("tDiffuse", nullptr);
        uniforms->set("opacity",1.0f);

        copyShader->uniforms = uniforms;

        copyShader->vertexShader = R""(

            varying vec2 vUv;

            void main() {

                vUv = uv;
                gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );

            }

        )"";

        copyShader->fragmentShader = R""(

            uniform float opacity;

            uniform sampler2D tDiffuse;

            varying vec2 vUv;

            void main() {

                gl_FragColor = texture2D( tDiffuse, vUv );
                gl_FragColor.a *= opacity;


            }

        )"";

        return copyShader;
    }

    ShaderMaterial::sptr CopyShader =  initCopyShader();

}

#endif //THREE_CPP_COPY_SHADER_H
