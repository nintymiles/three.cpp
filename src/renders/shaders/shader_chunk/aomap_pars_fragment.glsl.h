//
// Created by Willie on 2023/6/17.
//

#ifndef THREE_CPP_BEGIN_VERTEX_GLSL_H
#define THREE_CPP_BEGIN_VERTEX_GLSL_H

namespace shader_chunk {

/*glsl*/
char* aomap_pars_fragment = R""""(
#ifdef USE_AOMAP

	uniform sampler2D aoMap;
	uniform float aoMapIntensity;

#endif
)"""";

}

#endif //THREE_CPP_BEGIN_VERTEX_GLSL_H
