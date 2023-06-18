//
// Created by SeanR on 2023/6/18.
//

#ifndef THREE_CPP_CLEARCOAT_NORMAL_FRAGMENT_MAPS_GLSL_H
#define THREE_CPP_CLEARCOAT_NORMAL_FRAGMENT_MAPS_GLSL_H

namespace shader_chunk {
/*glsl*/
char *clearcoat_normal_fragment_maps = R""""(
#ifdef USE_CLEARCOAT_NORMALMAP

	vec3 clearcoatMapN = texture2D( clearcoatNormalMap, vUv ).xyz * 2.0 - 1.0;
	clearcoatMapN.xy *= clearcoatNormalScale;

	#ifdef USE_TANGENT

		clearcoatNormal = normalize( vTBN * clearcoatMapN );

	#else

		clearcoatNormal = perturbNormal2Arb( - vViewPosition, clearcoatNormal, clearcoatMapN, faceDirection );

	#endif

#endif
)"""";

}

#endif //THREE_CPP_CLEARCOAT_NORMAL_FRAGMENT_MAPS_GLSL_H
