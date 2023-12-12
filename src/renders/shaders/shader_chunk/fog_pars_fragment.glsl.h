//
// Created by SeanR on 2023/6/23.
//

#ifndef THREE_CPP_FOG_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_FOG_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* fog_pars_fragment = R""(
#ifdef USE_FOG

	uniform vec3 fogColor;
	varying float vFogDepth;

	#ifdef FOG_EXP2

		uniform float fogDensity;

	#else

		uniform float fogNear;
		uniform float fogFar;

	#endif

#endif
)"";

}

#endif //THREE_CPP_FOG_PARS_FRAGMENT_GLSL_H
