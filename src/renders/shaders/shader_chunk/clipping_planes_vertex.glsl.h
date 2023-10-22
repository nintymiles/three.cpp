//
// Created by Willie on 2023/6/22.
//

#ifndef THREE_CPP_CLIPPING_PLANES_VERTEX_GLSL_H
#define THREE_CPP_CLIPPING_PLANES_VERTEX_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* clipping_planes_vertex = R""""(
#if NUM_CLIPPING_PLANES > 0

	vClipPosition = - mvPosition.xyz;

#endif
)"""";

}

#endif //THREE_CPP_CLIPPING_PLANES_VERTEX_GLSL_H
