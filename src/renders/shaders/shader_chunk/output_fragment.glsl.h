#ifndef THREE_CPP_OUTPUT_FRAGMENT_GLSL_H
#define THREE_CPP_OUTPUT_FRAGMENT_GLSL_H

namespace shader_chunk {
const char *output_fragment = R""(

#ifdef OPAQUE
diffuseColor.a = 1.0;
#endif

// https://github.com/mrdoob/three.js/pull/22425
#ifdef USE_TRANSMISSION
diffuseColor.a *= material.transmissionAlpha + 0.1;
#endif

gl_FragColor = vec4( outgoingLight, diffuseColor.a );

)"";
}
#endif //THREE_CPP_OUTPUT_FRAGMENT_GLSL_H