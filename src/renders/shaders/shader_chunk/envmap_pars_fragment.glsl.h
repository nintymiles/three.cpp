//
// Created by SeanR on 2023/6/23.
//

#ifndef THREE_CPP_ENVMAP_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_ENVMAP_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* envmap_pars_fragment = R""""(
#ifdef USE_ENVMAP

	uniform float reflectivity;

	#if defined( USE_BUMPMAP ) || defined( USE_NORMALMAP ) || defined( PHONG ) || defined( LAMBERT )

		#define ENV_WORLDPOS

	#endif

	#ifdef ENV_WORLDPOS

		varying vec3 vWorldPosition;
		uniform float refractionRatio;
	#else
		varying vec3 vReflect;
	#endif

#endif
)"""";

}

#endif //THREE_CPP_ENVMAP_PARS_FRAGMENT_GLSL_H
