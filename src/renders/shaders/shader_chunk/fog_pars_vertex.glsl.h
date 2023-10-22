//
// Created by Willie on 2023/6/23.
//

#ifndef THREE_CPP_FOG_PARS_VERTEX_GLSL_H
#define THREE_CPP_FOG_PARS_VERTEX_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* fog_pars_vertex = R""""(
#ifdef USE_FOG

	varying float vFogDepth;

#endif
)"""";

}

#endif //THREE_CPP_FOG_PARS_VERTEX_GLSL_H
