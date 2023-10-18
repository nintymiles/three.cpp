//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_SHADER_MATERIAL_H
#define THREE_CPP_SHADER_MATERIAL_H

#include "material.h"

class ShaderMaterial : public Material{

public:
    using sptr = std::shared_ptr<ShaderMaterial>;

    std::unordered_map<std::string, std::vector<float>> defaultAttributeValues;

    ShaderMaterial();

    ShaderMaterial(const ShaderMaterial& source) : Material(source){
        type = "ShaderMaterial";
        name = "ShadowMaterial";
        fragmentShader = source.fragmentShader;
        vertexShader = source.vertexShader;
        isShaderMaterial = source.isShaderMaterial;
        //uniforms->copy(*source.uniforms);
        wireframe = source.wireframe;
        wireframeLinewidth = source.wireframeLinewidth;
        lights = source.lights;
        clipping = source.clipping;
        skinning = source.skinning;
        morphTargets = source.morphTargets;
        morphNormals = source.morphNormals;
        extensions = source.extensions;
        indexOfAttributeName = source.indexOfAttributeName;
        extensions = source.extensions;
    }

    ShaderMaterial& copy(const ShaderMaterial& source) {
        Material::copy(source);
        type = source.type;
        fragmentShader = source.fragmentShader;
        vertexShader = source.vertexShader;

        //uniforms->copy(*source.uniforms);
        wireframe = source.wireframe;
        wireframeLinewidth = source.wireframeLinewidth;
        lights = source.lights;
        clipping = source.clipping;
        skinning = source.skinning;
        morphTargets = source.morphTargets;
        morphNormals = source.morphNormals;
        extensions = source.extensions;
        indexOfAttributeName = source.indexOfAttributeName;
        extensions = source.extensions;
        return *this;
    }

    static sptr create() {
        return std::make_shared<ShaderMaterial>();
    }
};


#endif //THREE_CPP_SHADER_MATERIAL_H
