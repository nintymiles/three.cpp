//
// Created by SeanR on 2023/6/22.
//

#ifndef THREE_CPP_DITHERING_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_DITHERING_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* dithering_pars_fragment = R""(
#ifdef DITHERING

	// based on https://www.shadertoy.com/view/MslGR8
	vec3 dithering( vec3 color ) {
		//Calculate grid position
		float grid_position = rand( gl_FragCoord.xy );

		//Shift the individual colors differently, thus making it even harder to see the dithering pattern
		vec3 dither_shift_RGB = vec3( 0.25 / 255.0, -0.25 / 255.0, 0.25 / 255.0 );

		//modify shift according to grid position.
		dither_shift_RGB = mix( 2.0 * dither_shift_RGB, -2.0 * dither_shift_RGB, grid_position );

		//shift the color by dither_shift
		return color + dither_shift_RGB;
	}

#endif
)"";

}

#endif //THREE_CPP_DITHERING_PARS_FRAGMENT_GLSL_H
