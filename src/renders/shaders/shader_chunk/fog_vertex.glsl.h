//
// Created by SeanR on 2023/6/23.
//

#ifndef THREE_CPP_FOG_VERTEX_GLSL_H
#define THREE_CPP_FOG_VERTEX_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* fog_vertex = R""""(
#ifdef USE_FOG

	vFogDepth = - mvPosition.z;

#endif
)"""";

}

#endif //THREE_CPP_FOG_VERTEX_GLSL_H
