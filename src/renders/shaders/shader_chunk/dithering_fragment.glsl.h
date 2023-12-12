//
// Created by SeanR on 2023/6/22.
//

#ifndef THREE_CPP_DITHERING_FRAGMENT_GLSL_H
#define THREE_CPP_DITHERING_FRAGMENT_GLSL_H
namespace shader_chunk {

/*glsl*/
const char* dithering_fragment = R""(
#ifdef DITHERING

	gl_FragColor.rgb = dithering( gl_FragColor.rgb );

#endif
)"";

}

#endif //THREE_CPP_DITHERING_FRAGMENT_GLSL_H
