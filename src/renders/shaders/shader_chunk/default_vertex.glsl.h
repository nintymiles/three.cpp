//
// Created by Willie on 2023/6/22.
//

#ifndef THREE_CPP_DEFAULT_VERTEX_GLSL_H
#define THREE_CPP_DEFAULT_VERTEX_GLSL_H

namespace shader_chunk {

/*glsl*/
char* default_vertex = R""""(
void main() {
	gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );
}
)"""";

}

#endif //THREE_CPP_DEFAULT_VERTEX_GLSL_H
