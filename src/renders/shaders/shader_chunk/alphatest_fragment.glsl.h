//
// Created by Willie on 2023/6/17.
//

#ifndef THREE_CPP_ALPHATEST_FRAGMENT_GLSL_H
#define THREE_CPP_ALPHATEST_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
char* alphatest_fragment = R""""(
#ifdef USE_ALPHATEST

	if ( diffuseColor.a < alphaTest ) discard;

#endif
)"""";

}

#endif //THREE_CPP_ALPHATEST_FRAGMENT_GLSL_H
