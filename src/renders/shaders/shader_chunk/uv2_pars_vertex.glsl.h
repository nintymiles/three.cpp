
#ifndef THREE_CPP_UV2_PARS_VERTEX_GLSL_H
#define THREE_CPP_UV2_PARS_VERTEX_GLSL_H

namespace shader_chunk {
const char *uv2_pars_vertex = R""(
#if defined( USE_LIGHTMAP ) || defined( USE_AOMAP )

	attribute vec2 uv2;
	varying vec2 vUv2;

	uniform mat3 uv2Transform;

#endif

)"";
}

#endif //THREE_CPP_UV2_PARS_VERTEX_GLSL_H