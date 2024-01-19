//
// Created by SeanR on 2024/1/18.
//

#ifndef THREE_CPP_BACKGROUNDCUBE_VERT_H
#define THREE_CPP_BACKGROUNDCUBE_VERT_H

const char* backgroundCube_vert =R""(
varying vec3 vWorldDirection;

#include <common>

void main() {

	vWorldDirection = transformDirection( position, modelMatrix );

	#include <begin_vertex>
	#include <project_vertex>

	gl_Position.z = gl_Position.w; // set z to camera.far

}
)"";

#endif //THREE_CPP_BACKGROUNDCUBE_VERT_H
