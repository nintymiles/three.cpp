//
// Created by SeanR on 2023/6/22.
//

#ifndef THREE_CPP_ENCODINGS_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_ENCODINGS_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* encodings_pars_fragment = R""""(

vec4 LinearToLinear( in vec4 value ) {
	return value;
}

vec4 LinearTosRGB( in vec4 value ) {
	return vec4( mix( pow( value.rgb, vec3( 0.41666 ) ) * 1.055 - vec3( 0.055 ), value.rgb * 12.92, vec3( lessThanEqual( value.rgb, vec3( 0.0031308 ) ) ) ), value.a );
}

)"""";

}


#endif //THREE_CPP_ENCODINGS_PARS_FRAGMENT_GLSL_H
