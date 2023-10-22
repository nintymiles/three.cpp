//
// Created by Willie on 2023/6/22.
//

#ifndef THREE_CPP_CLIPPING_PLANES_PARS_VERTEX_GLSL_H
#define THREE_CPP_CLIPPING_PLANES_PARS_VERTEX_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* clipping_planes_pars_vertex = R""""(
#if NUM_CLIPPING_PLANES > 0

	varying vec3 vClipPosition;

#endif
)"""";

}

#endif //THREE_CPP_CLIPPING_PLANES_PARS_VERTEX_GLSL_H
