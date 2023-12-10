//
// Created by ultraman on 11/20/2023.
//

#ifndef THREE_CPP_NORMAL_PARS_VERTEX_H
#define THREE_CPP_NORMAL_PARS_VERTEX_H

namespace shader_chunk {
const char *normal_pars_vertex = R""(
#ifndef FLAT_SHADED

	varying vec3 vNormal;

	#ifdef USE_TANGENT

		varying vec3 vTangent;
		varying vec3 vBitangent;

	#endif

#endif
)"";
}
#endif //THREE_CPP_NORMAL_PARS_VERTEX_H
