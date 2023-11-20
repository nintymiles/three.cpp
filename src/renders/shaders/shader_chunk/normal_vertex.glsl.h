//
// Created by ultraman on 11/20/2023.
//

#ifndef THREE_CPP_NORMAL_VERTEX_H
#define THREE_CPP_NORMAL_VERTEX_H

namespace shader_chunk {
const char *normal_vertex = R""(
#ifndef FLAT_SHADED // normal is computed with derivatives when FLAT_SHADED

	vNormal = normalize( transformedNormal );

	#ifdef USE_TANGENT

		vTangent = normalize( transformedTangent );
		vBitangent = normalize( cross( vNormal, vTangent ) * tangent.w );

	#endif

#endif
)"";
}

#endif //THREE_CPP_NORMAL_VERTEX_H
