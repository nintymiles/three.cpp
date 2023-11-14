//
// Created by SeanR on 2023/11/14.
//

#ifndef THREE_CPP_ROUGHNESSMAP_FRAGMENT_H
#define THREE_CPP_ROUGHNESSMAP_FRAGMENT_H

namespace shader_chunk {
const char *roughnessmap_fragment = R"(
float roughnessFactor = roughness;

#ifdef USE_ROUGHNESSMAP

	vec4 texelRoughness = texture2D( roughnessMap, vUv );

	// reads channel G, compatible with a combined OcclusionRoughnessMetallic (RGB) texture
	roughnessFactor *= texelRoughness.g;

#endif
)";
}

#endif //THREE_CPP_ROUGHNESSMAP_FRAGMENT_H
