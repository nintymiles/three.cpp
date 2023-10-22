//
// Created by SeanR on 2023/6/24.
//

#ifndef THREE_CPP_LIGHTMAP_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_LIGHTMAP_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* lightmap_pars_fragment = R""""(
#ifdef USE_LIGHTMAP

	uniform sampler2D lightMap;
	uniform float lightMapIntensity;

#endif
)"""";

}

#endif //THREE_CPP_LIGHTMAP_PARS_FRAGMENT_GLSL_H
