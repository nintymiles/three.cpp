//
// Created by SeanR on 2023/6/18.
//

#ifndef THREE_CPP_CLIPPING_PLANES_FRAGMENT_GLSL_H
#define THREE_CPP_CLIPPING_PLANES_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* clipping_planes_fragment = R""""(
#if NUM_CLIPPING_PLANES > 0

	vec4 plane;

	#pragma unroll_loop_start
	for ( int i = 0; i < UNION_CLIPPING_PLANES; i ++ ) {

		plane = clippingPlanes[ i ];
		if ( dot( vClipPosition, plane.xyz ) > plane.w ) discard;

	}
	#pragma unroll_loop_end

	#if UNION_CLIPPING_PLANES < NUM_CLIPPING_PLANES

		bool clipped = true;

		#pragma unroll_loop_start
		for ( int i = UNION_CLIPPING_PLANES; i < NUM_CLIPPING_PLANES; i ++ ) {

			plane = clippingPlanes[ i ];
			clipped = ( dot( vClipPosition, plane.xyz ) > plane.w ) && clipped;

		}
		#pragma unroll_loop_end

		if ( clipped ) discard;

	#endif

#endif
)"""";

}

#endif //THREE_CPP_CLIPPING_PLANES_FRAGMENT_GLSL_H
