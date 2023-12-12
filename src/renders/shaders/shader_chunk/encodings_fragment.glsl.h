//
// Created by SeanR on 2023/6/22.
//

#ifndef THREE_CPP_ENCODINGS_FRAGMENT_GLSL_H
#define THREE_CPP_ENCODINGS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* encodings_fragment = R""(
gl_FragColor = linearToOutputTexel( gl_FragColor );
)"";

}

#endif //THREE_CPP_ENCODINGS_FRAGMENT_GLSL_H
