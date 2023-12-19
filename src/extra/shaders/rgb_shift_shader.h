//
// Created by SeanR on 2023/12/19.
//

#ifndef THREE_CPP_RGB_SHIFT_SHADER_H
#define THREE_CPP_RGB_SHIFT_SHADER_H

//#include "shader_material.h"
//#include "uniform_values.h"
//#include "vector2.h"
//
//namespace shader_data {
//    ShaderMaterial::sptr initRGBShiftShader(){
//        ShaderMaterial::sptr shader = std::make_shared<ShaderMaterial>();
//
//        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
//        uniforms->set<Texture::sptr>("tDiffuse", nullptr);
//        uniforms->set("amount",.005f);
//        uniforms->set("angle",.0f);
//
//        shader->uniforms = uniforms;
//
//        shader->vertexShader = R""(
//
//            varying vec2 vUv;
//
//            void main() {
//
//                vUv = uv;
//                gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );
//
//            }
//
//        )"";
//
//        shader->fragmentShader = R""(
//
//            uniform sampler2D tDiffuse;
//            uniform float amount;
//            uniform float angle;
//
//            varying vec2 vUv;
//
//            void main() {
//
//                vec2 offset = amount * vec2( cos(angle), sin(angle));
//                vec4 cr = texture2D(tDiffuse, vUv + offset);
//                vec4 cga = texture2D(tDiffuse, vUv);
//                vec4 cb = texture2D(tDiffuse, vUv - offset);
//                gl_FragColor = vec4(cr.r, cga.g, cb.b, cga.a);
//
//            }
//
//        )"";
//
//        return shader;
//    }
//
//    const ShaderMaterial::sptr RGBShiftShader;
//
//}

#endif //THREE_CPP_RGB_SHIFT_SHADER_H
