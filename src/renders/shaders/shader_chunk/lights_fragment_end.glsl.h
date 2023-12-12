//
// Created by SeanR on 2023/6/24.
//

#ifndef THREE_CPP_LIGHTS_FRAGMENT_END_GLSL_H
#define THREE_CPP_LIGHTS_FRAGMENT_END_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* lights_fragment_end = R""(
#if defined( RE_IndirectDiffuse )

	RE_IndirectDiffuse( irradiance, geometry, material, reflectedLight );

#endif

#if defined( RE_IndirectSpecular )

	RE_IndirectSpecular( radiance, iblIrradiance, clearcoatRadiance, geometry, material, reflectedLight );

#endif
)"";

}

#endif //THREE_CPP_LIGHTS_FRAGMENT_END_GLSL_H
