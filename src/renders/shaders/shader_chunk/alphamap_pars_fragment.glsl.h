//
// Created by SeanR on 2023/6/17.
//

#ifndef THREE_CPP_ALPHAMAP_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_ALPHAMAP_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* alphamap_pars_fragment = R""""(
#ifdef USE_ALPHAMAP

	uniform sampler2D alphaMap;

#endif
)"""";

}

#endif //THREE_CPP_ALPHAMAP_PARS_FRAGMENT_GLSL_H
