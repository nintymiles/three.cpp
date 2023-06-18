//
// Created by Willie on 2023/6/17.
//

#ifndef THREE_CPP_BEGIN_VERTEX_GLSL_H
#define THREE_CPP_BEGIN_VERTEX_GLSL_H

namespace shader_chunk {

/*glsl*/
char* begin_vertex = R""""(
vec3 objectNormal = vec3( normal );

#ifdef USE_TANGENT

	vec3 objectTangent = vec3( tangent.xyz );

#endif
)"""";

}

#endif //THREE_CPP_BEGIN_VERTEX_GLSL_H
