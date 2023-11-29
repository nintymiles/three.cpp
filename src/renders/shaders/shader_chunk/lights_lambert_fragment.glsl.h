//
// Created by SeanR on 11/29/2023.
//

#ifndef THREE_CPP_LIGHTS_LAMBERT_FRAGMENT_GLSL_H
#define THREE_CPP_LIGHTS_LAMBERT_FRAGMENT_GLSL_H


namespace shader_chunk {
/*glsl*/
const char* lights_lambert_fragment = R""(
LambertMaterial material;
material.diffuseColor = diffuseColor.rgb;
material.specularStrength = specularStrength;
)"";

}

#endif //THREE_CPP_LIGHTS_LAMBERT_FRAGMENT_GLSL_H
