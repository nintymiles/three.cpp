//
// Created by Willie on 2023/6/22.
//

#ifndef THREE_CPP_COLOR_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_COLOR_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* color_pars_fragment = R""""(
#if defined( USE_COLOR_ALPHA )

	varying vec4 vColor;

#elif defined( USE_COLOR )

	varying vec3 vColor;

#endif
)"""";

}

#endif //THREE_CPP_COLOR_PARS_FRAGMENT_GLSL_H
