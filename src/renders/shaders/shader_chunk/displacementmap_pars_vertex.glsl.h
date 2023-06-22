//
// Created by Willie on 2023/6/22.
//

#ifndef THREE_CPP_DISPLACEMENTMAP_PARS_VERTEX_GLSL_H
#define THREE_CPP_DISPLACEMENTMAP_PARS_VERTEX_GLSL_H

namespace shader_chunk {

/*glsl*/
char* displacementmap_pars_vertex = R""""(
#ifdef USE_DISPLACEMENTMAP

	uniform sampler2D displacementMap;
	uniform float displacementScale;
	uniform float displacementBias;

#endif
)"""";

}

#endif //THREE_CPP_DISPLACEMENTMAP_PARS_VERTEX_GLSL_H
