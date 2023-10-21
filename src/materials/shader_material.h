//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_SHADER_MATERIAL_H
#define THREE_CPP_SHADER_MATERIAL_H

#include "material.h"

#include <memory>

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

//class ShaderMaterial : public Material{
//public:
//    std::string type = "ShaderMaterial";
//
//    std::string vertexShader = default_vertex;
//    std::string fragmentShader = default_fragment;
//
//    double lineWidth = 1;
//    bool wireFrame = false;
//    double wireLineWidth = 1;
//
//    bool fog = false; // set to use scene fog
//    bool lights = false; // set to use scene lights
//    bool clipping = false; // set to use user-defined clipping planes
//
//    std::string glslVersion = "";
//
//    std::map<std::string,int> defines;
//    std::map<std::string,threecpp::UniformValue> uniforms;
//
//    std::map<std::string,bool> extensions = {{"derivatives",false},{"fragDepth",false},{"drawBuffers",false},{"shaderTextureLOD",false}};
//
//    // When rendered geometry doesn't include these attributes but the material does,
//    // use these default values in WebGL. This avoids errors when buffer data is missing.
//    std::map<std::string,std::vector<int>> defaultAttributeValues = {
//            {"color", { 1, 1, 1 }},
//            {"uv", { 0, 0 }},
//            {"uv2", { 0, 0 }}
//    };
//
//    std::string index0AttributeName;
//    bool uniformsNeedUpdate = false;
//};


#endif //THREE_CPP_SHADER_MATERIAL_H
