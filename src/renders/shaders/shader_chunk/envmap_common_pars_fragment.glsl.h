//
// Created by SeanR on 2023/6/23.
//

#ifndef THREE_CPP_ENVMAP_COMMON_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_ENVMAP_COMMON_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
char* envmap_common_pars_fragment = R""""(
#ifdef USE_ENVMAP

	uniform float envMapIntensity;
	uniform float flipEnvMap;

	#ifdef ENVMAP_TYPE_CUBE
		uniform samplerCube envMap;
	#else
		uniform sampler2D envMap;
	#endif

#endif
)"""";

}

#endif //THREE_CPP_ENVMAP_COMMON_PARS_FRAGMENT_GLSL_H
