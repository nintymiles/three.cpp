//
// Created by SeanR on 2023/6/24.
//

#ifndef THREE_CPP_IRIDESCENCE_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_IRIDESCENCE_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* iridescence_pars_fragment = R""""(
#ifdef USE_IRIDESCENCEMAP

	uniform sampler2D iridescenceMap;

#endif

#ifdef USE_IRIDESCENCE_THICKNESSMAP

	uniform sampler2D iridescenceThicknessMap;

#endif
)"""";

}

#endif //THREE_CPP_IRIDESCENCE_PARS_FRAGMENT_GLSL_H
