//
// Created by SeanR on 2023/6/22.
//

#ifndef THREE_CPP_COLOR_FRAGMENT_GLSL_H
#define THREE_CPP_COLOR_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* color_fragment = R""(
#if defined( USE_COLOR_ALPHA )

	diffuseColor *= vColor;

#elif defined( USE_COLOR )

	diffuseColor.rgb *= vColor;

#endif
)"";

}

#endif //THREE_CPP_COLOR_FRAGMENT_GLSL_H
