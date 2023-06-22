//
// Created by Willie on 2023/6/22.
//

#ifndef THREE_CPP_COLOR_PARS_VERTEX_GLSL_H
#define THREE_CPP_COLOR_PARS_VERTEX_GLSL_H

namespace shader_chunk {

/*glsl*/
char* color_pars_vertex = R""""(
#if defined( USE_COLOR_ALPHA )

	varying vec4 vColor;

#elif defined( USE_COLOR ) || defined( USE_INSTANCING_COLOR )

	varying vec3 vColor;

#endif
)"""";

}

#endif //THREE_CPP_COLOR_PARS_VERTEX_GLSL_H
