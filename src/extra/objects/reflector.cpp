//
// Created by SeanR on 2023/12/5.
//
#include "reflector.h"

namespace reflector_shader{
//    Reflector.ReflectorShader = {
//
//            uniforms: {
//
//                    'color': {
//                        value: null
//                    },
//
//                    'tDiffuse': {
//                        value: null
//                    },
//
//                    'textureMatrix': {
//                        value: null
//                    }
//
//            },

            /* glsl */
            const char* vertexShader = R""(
            uniform mat4 textureMatrix;
            varying vec4 vUv;

#include <common>
#include <logdepthbuf_pars_vertex>

            void main() {

                vUv = textureMatrix * vec4( position, 1.0 );

                gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );

#include <logdepthbuf_vertex>

            },
            )"";

            const char* fragmentShader = R""(
            uniform vec3 color;
            uniform sampler2D tDiffuse;
            varying vec4 vUv;

#include <logdepthbuf_pars_fragment>

            float blendOverlay( float base, float blend ) {

                return( base < 0.5 ? ( 2.0 * base * blend ) : ( 1.0 - 2.0 * ( 1.0 - base ) * ( 1.0 - blend ) ) );

            }

            vec3 blendOverlay( vec3 base, vec3 blend ) {

                return vec3( blendOverlay( base.r, blend.r ), blendOverlay( base.g, blend.g ), blendOverlay( base.b, blend.b ) );

            }

            void main() {

#include <logdepthbuf_fragment>

                vec4 base = texture2DProj( tDiffuse, vUv );
                gl_FragColor = vec4( blendOverlay( base.rgb, color ), 1.0 );

#include <tonemapping_fragment>
#include <encodings_fragment>

            }
    )"";
}