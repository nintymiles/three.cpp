//
// Created by Willie on 2023/6/22.
//

#ifndef THREE_CPP_DEFAULT_FRAGMENT_GLSL_H
#define THREE_CPP_DEFAULT_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* default_fragment = R""""(
void main() {
	gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
}
)"""";

}

#endif //THREE_CPP_DEFAULT_FRAGMENT_GLSL_H
