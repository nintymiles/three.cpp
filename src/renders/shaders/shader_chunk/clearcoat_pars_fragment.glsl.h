//
// Created by SeanR on 2023/6/18.
//

#ifndef THREE_CPP_CLEARCOAT_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_CLEARCOAT_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* clearcoat_pars_fragment = R""(
#ifdef USE_CLEARCOATMAP

	uniform sampler2D clearcoatMap;

#endif

#ifdef USE_CLEARCOAT_ROUGHNESSMAP

	uniform sampler2D clearcoatRoughnessMap;

#endif

#ifdef USE_CLEARCOAT_NORMALMAP

	uniform sampler2D clearcoatNormalMap;
	uniform vec2 clearcoatNormalScale;

#endif
)"";

}

#endif //THREE_CPP_CLEARCOAT_PARS_FRAGMENT_GLSL_H
