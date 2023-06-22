//
// Created by Willie on 2023/6/22.
//

#ifndef THREE_CPP_DEFAULTNORMAL_VERTEX_GLSL_H
#define THREE_CPP_DEFAULTNORMAL_VERTEX_GLSL_H

namespace shader_chunk {

/*glsl*/
char* defaultnormal_vertex = R""""(
vec3 transformedNormal = objectNormal;

#ifdef USE_INSTANCING

	// this is in lieu of a per-instance normal-matrix
	// shear transforms in the instance matrix are not supported

	mat3 m = mat3( instanceMatrix );

	transformedNormal /= vec3( dot( m[ 0 ], m[ 0 ] ), dot( m[ 1 ], m[ 1 ] ), dot( m[ 2 ], m[ 2 ] ) );

	transformedNormal = m * transformedNormal;

#endif

transformedNormal = normalMatrix * transformedNormal;

#ifdef FLIP_SIDED

	transformedNormal = - transformedNormal;

#endif

#ifdef USE_TANGENT

	vec3 transformedTangent = ( modelViewMatrix * vec4( objectTangent, 0.0 ) ).xyz;

	#ifdef FLIP_SIDED

		transformedTangent = - transformedTangent;

	#endif

#endif
)"""";

}

#endif //THREE_CPP_DEFAULTNORMAL_VERTEX_GLSL_H
