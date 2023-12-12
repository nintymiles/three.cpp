//
// Created by SeanR on 2023/6/22.
//

#ifndef THREE_CPP_EMISSIVEMAP_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_EMISSIVEMAP_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* emissivemap_pars_fragment = R""(
#ifdef USE_EMISSIVEMAP

	uniform sampler2D emissiveMap;

#endif
)"";

}

#endif //THREE_CPP_EMISSIVEMAP_PARS_FRAGMENT_GLSL_H
