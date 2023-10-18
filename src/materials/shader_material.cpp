//
// Created by SeanR on 2023/10/18.
//

#include "shader_material.h"

#include "shader_chunk.h"
#include "uniform_values.h"

#include <memory>

ShaderMaterial::ShaderMaterial() : Material(){
    type = "ShaderMaterial";
    name = "ShaderMaterial";
    isShaderMaterial = true;

    uniforms = std::make_shared<UniformValues>();

    vertexShader.assign(getShaderChunk(shaderlib_name::get("default_vertex")));
    fragmentShader.assign(getShaderChunk(shaderlib_name::get("default_fragment")));

    linewidth = 1;

    wireframe = false;
    wireframeLinewidth = 1;

    fog = false;
    lights = false;
    clipping = false;

    skinning = false;
    morphTargets = false;
    morphNormals = false;

    extensions.derivatives = false;
    extensions.fragDepth = false;
    extensions.drawBuffers = false;
    extensions.shaderTextureLOD = false;

    defaultAttributeValues["color"] = { 1,1,1 };
    defaultAttributeValues["uv"] = { 0,0 };
    defaultAttributeValues["uv2"] = { 0,0 };
}