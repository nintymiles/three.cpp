//
// Created by SeanR on 11/29/2023.
//

#ifndef THREE_CPP_LIGHTS_LAMBERT_PARS_FRAGMENT_GLSL_H
#define THREE_CPP_LIGHTS_LAMBERT_PARS_FRAGMENT_GLSL_H

namespace shader_chunk {

/*glsl*/
const char* lights_lambert_pars_fragment = R""(
varying vec3 vViewPosition;

struct LambertMaterial {

	vec3 diffuseColor;
	float specularStrength;

};

void RE_Direct_Lambert( const in IncidentLight directLight, const in GeometricContext geometry, const in LambertMaterial material, inout ReflectedLight reflectedLight ) {

	float dotNL = saturate( dot( geometry.normal, directLight.direction ) );
	vec3 irradiance = dotNL * directLight.color;

	reflectedLight.directDiffuse += irradiance * BRDF_Lambert( material.diffuseColor );

}

void RE_IndirectDiffuse_Lambert( const in vec3 irradiance, const in GeometricContext geometry, const in LambertMaterial material, inout ReflectedLight reflectedLight ) {

	reflectedLight.indirectDiffuse += irradiance * BRDF_Lambert( material.diffuseColor );

}

#define RE_Direct				RE_Direct_Lambert
#define RE_IndirectDiffuse		RE_IndirectDiffuse_Lambert

#define Material_LightProbeLOD( material )	(0)
)"";

}

#endif //THREE_CPP_LIGHTS_LAMBERT_PARS_FRAGMENT_GLSL_H
