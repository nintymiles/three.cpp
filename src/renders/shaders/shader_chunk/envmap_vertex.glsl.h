//
// Created by SeanR on 2023/6/23.
//

#ifndef THREE_CPP_ENVMAP_VERTEX_GLSL_H
#define THREE_CPP_ENVMAP_VERTEX_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* envmap_vertex = R""""(
#ifdef USE_ENVMAP

	#ifdef ENV_WORLDPOS

		vWorldPosition = worldPosition.xyz;

	#else

		vec3 cameraToVertex;

		if ( isOrthographic ) {

			cameraToVertex = normalize( vec3( - viewMatrix[ 0 ][ 2 ], - viewMatrix[ 1 ][ 2 ], - viewMatrix[ 2 ][ 2 ] ) );

		} else {

			cameraToVertex = normalize( worldPosition.xyz - cameraPosition );

		}

		vec3 worldNormal = inverseTransformDirection( transformedNormal, viewMatrix );

		#ifdef ENVMAP_MODE_REFLECTION

			vReflect = reflect( cameraToVertex, worldNormal );

		#else

			vReflect = refract( cameraToVertex, worldNormal, refractionRatio );

		#endif

	#endif

#endif
)"""";

}

#endif //THREE_CPP_ENVMAP_VERTEX_GLSL_H
