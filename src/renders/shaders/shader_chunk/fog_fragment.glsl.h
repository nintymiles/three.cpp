//
// Created by SeanR on 2023/6/23.
//

#ifndef THREE_CPP_FOG_FRAGMENT_GLSL_H
#define THREE_CPP_FOG_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* fog_fragment = R""(
#ifdef USE_FOG

	#ifdef FOG_EXP2

		float fogFactor = 1.0 - exp( - fogDensity * fogDensity * vFogDepth * vFogDepth );

	#else

		float fogFactor = smoothstep( fogNear, fogFar, vFogDepth );

	#endif

	gl_FragColor.rgb = mix( gl_FragColor.rgb, fogColor, fogFactor );

#endif
)"";

}

#endif //THREE_CPP_FOG_FRAGMENT_GLSL_H
