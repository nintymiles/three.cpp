//
// Created by SeanR on 2022/10/19.
//

#ifndef THREE_CPP_SRC_MATERIALS_SHADER_MATERAL_H
#define THREE_CPP_SRC_MATERIALS_SHADER_MATERAL_H

#include "material.h"
#include "../renders/shaders/shaders_constant.h"
#include "common_types.h"

//import { Material } from './Material.js';
//import { cloneUniforms } from '../renderers/shaders/UniformsUtils.js';
//import default_vertex from '../renderers/shaders/ShaderChunk/default_vertex.glsl.js';
//import default_fragment from '../renderers/shaders/ShaderChunk/default_fragment.glsl.js';


#include <string>
#include <map>

class ShaderMaterial : public Material{
public:
    std::string type = "ShaderMaterial";

    std::string vertexShader = default_vertex;
    std::string fragmentShader = default_fragment;

    double lineWidth = 1;
    bool wireFrame = false;
    double wireLineWidth = 1;

    bool fog = false; // set to use scene fog
    bool lights = false; // set to use scene lights
    bool clipping = false; // set to use user-defined clipping planes

    std::string glslVersion = "";

    std::map<std::string,int> defines;
    std::map<std::string,threecpp::UniformValue> uniforms;

    std::map<std::string,bool> extensions = {{"derivatives",false},{"fragDepth",false},{"drawBuffers",false},{"shaderTextureLOD",false}};

    // When rendered geometry doesn't include these attributes but the material does,
    // use these default values in WebGL. This avoids errors when buffer data is missing.
    std::map<std::string,std::vector<int>> defaultAttributeValues = {
        {"color", { 1, 1, 1 }},
        {"uv", { 0, 0 }},
        {"uv2", { 0, 0 }}
    };

    std::string index0AttributeName;
    bool uniformsNeedUpdate = false;
};


//
//class ShaderMaterial extends Material {
//
//constructor( parameters ) {
//
//    this.defines = {};
//    this.uniforms = {};

//    if ( parameters !== undefined ) {
//
//        if ( parameters.attributes !== undefined ) {
//
//            console.error( 'THREE.ShaderMaterial: attributes should now be defined in THREE.BufferGeometry instead.' );
//
//        }
//
//        this.setValues( parameters );
//
//    }
//
//}
//
//copy( source ) {
//
//    super.copy( source );
//
//    this.fragmentShader = source.fragmentShader;
//    this.vertexShader = source.vertexShader;
//
//    this.uniforms = cloneUniforms( source.uniforms );
//
//    this.defines = Object.assign( {}, source.defines );
//
//    this.wireframe = source.wireframe;
//    this.wireframeLinewidth = source.wireframeLinewidth;
//
//    this.lights = source.lights;
//    this.clipping = source.clipping;
//
//    this.extensions = Object.assign( {}, source.extensions );
//
//    this.glslVersion = source.glslVersion;
//
//    return this;
//
//}
//
//toJSON( meta ) {
//
//    const data = super.toJSON( meta );
//
//    data.glslVersion = this.glslVersion;
//    data.uniforms = {};
//
//    for ( const name in this.uniforms ) {
//
//        const uniform = this.uniforms[ name ];
//        const value = uniform.value;
//
//        if ( value && value.isTexture ) {
//
//            data.uniforms[ name ] = {
//                type: 't',
//                value: value.toJSON( meta ).uuid
//            };
//
//        } else if ( value && value.isColor ) {
//
//            data.uniforms[ name ] = {
//                type: 'c',
//                value: value.getHex()
//            };
//
//        } else if ( value && value.isVector2 ) {
//
//            data.uniforms[ name ] = {
//                type: 'v2',
//                value: value.toArray()
//            };
//
//        } else if ( value && value.isVector3 ) {
//
//            data.uniforms[ name ] = {
//                type: 'v3',
//                value: value.toArray()
//            };
//
//        } else if ( value && value.isVector4 ) {
//
//            data.uniforms[ name ] = {
//                type: 'v4',
//                value: value.toArray()
//            };
//
//        } else if ( value && value.isMatrix3 ) {
//
//            data.uniforms[ name ] = {
//                type: 'm3',
//                value: value.toArray()
//            };
//
//        } else if ( value && value.isMatrix4 ) {
//
//            data.uniforms[ name ] = {
//                type: 'm4',
//                value: value.toArray()
//            };
//
//        } else {
//
//            data.uniforms[ name ] = {
//                value: value
//            };
//
//            // note: the array variants v2v, v3v, v4v, m4v and tv are not supported so far
//
//        }
//
//    }
//
//    if ( Object.keys( this.defines ).length > 0 ) data.defines = this.defines;
//
//    data.vertexShader = this.vertexShader;
//    data.fragmentShader = this.fragmentShader;
//
//    const extensions = {};
//
//    for ( const key in this.extensions ) {
//
//        if ( this.extensions[ key ] === true ) extensions[ key ] = true;
//
//    }
//
//    if ( Object.keys( extensions ).length > 0 ) data.extensions = extensions;
//
//    return data;
//
//}
//
//}
//
//ShaderMaterial.prototype.isShaderMaterial = true;
//
//export { ShaderMaterial };


#endif //THREE_CPP_SRC_MATERIALS_SHADER_MATERAL_H
