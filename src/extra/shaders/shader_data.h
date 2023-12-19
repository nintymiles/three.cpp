//
// Created by SeanR on 2023/12/19.
//

#ifndef THREE_CPP_SHADER_DATA_H
#define THREE_CPP_SHADER_DATA_H

#include "shader_material.h"
#include "uniform_values.h"

#include "vector2.h"

struct ShaderData{
    static ShaderMaterial::sptr getCopyShader(){
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

    static ShaderMaterial::sptr getDotScreenShader(){
        ShaderMaterial::sptr shader = std::make_shared<ShaderMaterial>();

        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        uniforms->set<Texture::sptr>("tDiffuse", nullptr);
        uniforms->set("tSize",Vector2( 256, 256 ));
        uniforms->set("center",Vector2( 0.5, 0.5 ));
        uniforms->set("angle",1.57f);
        uniforms->set("scale",1.f);

        shader->uniforms = uniforms;

        shader->vertexShader = R""(

            varying vec2 vUv;

            void main() {

                vUv = uv;
                gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );

            }

        )"";

        shader->fragmentShader = R""(

            uniform vec2 center;
            uniform float angle;
            uniform float scale;
            uniform vec2 tSize;

            uniform sampler2D tDiffuse;

            varying vec2 vUv;

            float pattern() {

                float s = sin( angle ), c = cos( angle );

                vec2 tex = vUv * tSize - center;
                vec2 point = vec2( c * tex.x - s * tex.y, s * tex.x + c * tex.y ) * scale;

                return ( sin( point.x ) * sin( point.y ) ) * 4.0;

            }

            void main() {

                vec4 color = texture2D( tDiffuse, vUv );

                float average = ( color.r + color.g + color.b ) / 3.0;

                gl_FragColor = vec4( vec3( average * 10.0 - 5.0 + pattern() ), color.a );

            }

        )"";

        return shader;
    }

    static ShaderMaterial::sptr getRGBShiftShader(){
        ShaderMaterial::sptr shader = std::make_shared<ShaderMaterial>();

        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        uniforms->set<Texture::sptr>("tDiffuse", nullptr);
        uniforms->set("amount",.005f);
        uniforms->set("angle",.0f);

        shader->uniforms = uniforms;

        shader->vertexShader = R""(

            varying vec2 vUv;

            void main() {

                vUv = uv;
                gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );

            }

        )"";

        shader->fragmentShader = R""(

            uniform sampler2D tDiffuse;
            uniform float amount;
            uniform float angle;

            varying vec2 vUv;

            void main() {

                vec2 offset = amount * vec2( cos(angle), sin(angle));
                vec4 cr = texture2D(tDiffuse, vUv + offset);
                vec4 cga = texture2D(tDiffuse, vUv);
                vec4 cb = texture2D(tDiffuse, vUv - offset);
                gl_FragColor = vec4(cr.r, cga.g, cb.b, cga.a);

            }

        )"";

        return shader;
    }

};

#endif //THREE_CPP_SHADER_DATA_H
