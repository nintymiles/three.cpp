//
// Created by Willie on 2023/6/17.
//

#ifndef THREE_CPP_ALPHAMAP_FRAGMENT_GLSL_H
#define THREE_CPP_ALPHAMAP_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
char* alphamap_fragment = R""""(
#ifdef USE_ALPHAMAP

	diffuseColor.a *= texture2D( alphaMap, vUv ).g;

#endif
)"""";

}

#endif //THREE_CPP_ALPHAMAP_FRAGMENT_GLSL_H
