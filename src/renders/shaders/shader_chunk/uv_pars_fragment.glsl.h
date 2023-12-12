#ifndef THREE_CPP_UV_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_UV_PARS_FRAGMENT_GLSL_H
namespace shader_chunk {
const char *uv_pars_fragment = R""(
#if ( defined( USE_UV ) && ! defined( UVS_VERTEX_ONLY ) )

	varying vec2 vUv;

#endif
)"";
}
#endif //THREE_CPP_UV_PARS_FRAGMENT_GLSL_H