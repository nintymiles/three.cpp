//
// Created by SeanR on 2023/6/24.
//

#ifndef THREE_CPP_LIGHTS_FRAGMENT_MAPS_GLSL_H
#define THREE_CPP_LIGHTS_FRAGMENT_MAPS_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* lights_fragment_maps = R""""(
#if defined( RE_IndirectDiffuse )

	#ifdef USE_LIGHTMAP

		vec4 lightMapTexel = texture2D( lightMap, vUv2 );
		vec3 lightMapIrradiance = lightMapTexel.rgb * lightMapIntensity;

		irradiance += lightMapIrradiance;

	#endif

	#if defined( USE_ENVMAP ) && defined( STANDARD ) && defined( ENVMAP_TYPE_CUBE_UV )

		iblIrradiance += getIBLIrradiance( geometry.normal );

	#endif

#endif

#if defined( USE_ENVMAP ) && defined( RE_IndirectSpecular )

	radiance += getIBLRadiance( geometry.viewDir, geometry.normal, material.roughness );

	#ifdef USE_CLEARCOAT

		clearcoatRadiance += getIBLRadiance( geometry.viewDir, geometry.clearcoatNormal, material.clearcoatRoughness );

	#endif

#endif
)"""";

}

#endif //THREE_CPP_LIGHTS_FRAGMENT_MAPS_GLSL_H
