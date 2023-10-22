//
// Created by SeanR on 2023/6/22.
//

#ifndef THREE_CPP_EMISSIVEMAP_FRAGMENT_GLSL_H
#define THREE_CPP_EMISSIVEMAP_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* emissivemap_fragment = R""""(
#ifdef USE_EMISSIVEMAP

	vec4 emissiveColor = texture2D( emissiveMap, vUv );

	totalEmissiveRadiance *= emissiveColor.rgb;

#endif
)"""";

}

#endif //THREE_CPP_EMISSIVEMAP_FRAGMENT_GLSL_H
