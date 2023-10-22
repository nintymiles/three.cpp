//
// Created by Willie on 2023/6/22.
//

#ifndef THREE_CPP_DISPLACEMENTMAP_VERTEX_GLSL_H
#define THREE_CPP_DISPLACEMENTMAP_VERTEX_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* displacementmap_vertex = R""""(
#ifdef USE_DISPLACEMENTMAP

	transformed += normalize( objectNormal ) * ( texture2D( displacementMap, vUv ).x * displacementScale + displacementBias );

#endif
)"""";

}

#endif //THREE_CPP_DISPLACEMENTMAP_VERTEX_GLSL_H
