//
// Created by SeanR on 2023/6/24.
//

#ifndef THREE_CPP_LIGHTMAP_FRAGMENT_GLSL_H
#define THREE_CPP_LIGHTMAP_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* lightmap_fragment = R""""(
#ifdef USE_LIGHTMAP

	vec4 lightMapTexel = texture2D( lightMap, vUv2 );
	vec3 lightMapIrradiance = lightMapTexel.rgb * lightMapIntensity;

	reflectedLight.indirectDiffuse += lightMapIrradiance;

#endif
)"""";

}

#endif //THREE_CPP_LIGHTMAP_FRAGMENT_GLSL_H
