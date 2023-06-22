//
// Created by Willie on 2023/6/22.
//

#ifndef THREE_CPP_COLOR_VERTEX_GLSL_H
#define THREE_CPP_COLOR_VERTEX_GLSL_H

namespace shader_chunk {

/*glsl*/
char* color_vertex = R""""(
#if defined( USE_COLOR_ALPHA )

	vColor = vec4( 1.0 );

#elif defined( USE_COLOR ) || defined( USE_INSTANCING_COLOR )

	vColor = vec3( 1.0 );

#endif

#ifdef USE_COLOR

	vColor *= color;

#endif

#ifdef USE_INSTANCING_COLOR

	vColor.xyz *= instanceColor.xyz;

#endif
)"""";

}


#endif //THREE_CPP_COLOR_VERTEX_GLSL_H
