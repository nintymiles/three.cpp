//
// Created by SeanR on 2023/6/18.
//

#ifndef THREE_CPP_CLEARCOAT_NORMAL_FRAGMENT_BEGIN_GLSL_H
#define THREE_CPP_CLEARCOAT_NORMAL_FRAGMENT_BEGIN_GLSL_H

namespace shader_chunk {

/*glsl*/
char* clearcoat_normal_fragment_begin = R""""(
#ifdef USE_CLEARCOAT

	vec3 clearcoatNormal = geometryNormal;

#endif
)"""";

}

#endif //THREE_CPP_CLEARCOAT_NORMAL_FRAGMENT_BEGIN_GLSL_H
