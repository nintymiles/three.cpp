//
// Created by SeanR on 2023/6/18.
//

#ifndef THREE_CPP_AOMAP_FRAGMENT_GLSL_H
#define THREE_CPP_AOMAP_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* aomap_fragment = R""""(
#ifdef USE_AOMAP

	// reads channel R, compatible with a combined OcclusionRoughnessMetallic (RGB) texture
	float ambientOcclusion = ( texture2D( aoMap, vUv2 ).r - 1.0 ) * aoMapIntensity + 1.0;

	reflectedLight.indirectDiffuse *= ambientOcclusion;

	#if defined( USE_ENVMAP ) && defined( STANDARD )

		float dotNV = saturate( dot( geometry.normal, geometry.viewDir ) );

		reflectedLight.indirectSpecular *= computeSpecularOcclusion( dotNV, ambientOcclusion, material.roughness );

	#endif

#endif
)"""";

}

#endif //THREE_CPP_AOMAP_FRAGMENT_GLSL_H
