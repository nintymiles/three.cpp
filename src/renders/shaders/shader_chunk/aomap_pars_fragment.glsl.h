//
// Created by SeanR on 2023/6/17.
//

#ifndef THREE_CPP_AOMAP_PAR_FRAGMENT_GLSL_H
#define THREE_CPP_AOMAP_PAR_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
char* aomap_pars_fragment = R""""(
#ifdef USE_AOMAP

	uniform sampler2D aoMap;
	uniform float aoMapIntensity;

#endif
)"""";

}

#endif //THREE_CPP_AOMAP_PAR_FRAGMENT_GLSL_H
