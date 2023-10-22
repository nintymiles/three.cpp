//
// Created by SeanR on 2023/6/18.
//

#ifndef THREE_CPP_ALPHATEST_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_ALPHATEST_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* alphatest_pars_fragment = R""""(
#ifdef USE_ALPHATEST
	uniform float alphaTest;
#endif
)"""";

}


#endif //THREE_CPP_ALPHATEST_PARS_FRAGMENT_GLSL_H
