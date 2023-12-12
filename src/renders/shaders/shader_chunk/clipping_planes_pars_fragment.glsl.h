//
// Created by SeanR on 2023/6/18.
//

#ifndef THREE_CPP_CLIPPING_PLANES_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_CLIPPING_PLANES_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* clipping_planes_pars_fragment = R""(
#if NUM_CLIPPING_PLANES > 0

	varying vec3 vClipPosition;

	uniform vec4 clippingPlanes[ NUM_CLIPPING_PLANES ];

#endif
)"";

}

#endif //THREE_CPP_CLIPPING_PLANES_PARS_FRAGMENT_GLSL_H
