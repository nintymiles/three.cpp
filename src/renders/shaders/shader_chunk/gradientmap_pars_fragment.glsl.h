//
// Created by SeanR on 2023/6/24.
//

#ifndef THREE_CPP_GRADIENTMAP_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_GRADIENTMAP_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* gradientmap_pars_fragment = R""(
#ifdef USE_GRADIENTMAP

	uniform sampler2D gradientMap;

#endif

vec3 getGradientIrradiance( vec3 normal, vec3 lightDirection ) {

	// dotNL will be from -1.0 to 1.0
	float dotNL = dot( normal, lightDirection );
	vec2 coord = vec2( dotNL * 0.5 + 0.5, 0.0 );

	#ifdef USE_GRADIENTMAP

		return vec3( texture2D( gradientMap, coord ).r );

	#else

		return ( coord.x < 0.7 ) ? vec3( 0.7 ) : vec3( 1.0 );

	#endif

}
)"";

}

#endif //THREE_CPP_GRADIENTMAP_PARS_FRAGMENT_GLSL_H
