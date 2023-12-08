//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_GL_PROGRAM_PARAMETERS_H
#define THREE_CPP_GL_PROGRAM_PARAMETERS_H

#include "constants.h"
#include "uniform_values.h"

#include <unordered_map>

class UniformValues;
struct ProgramParameters {
    using sptr = std::shared_ptr<ProgramParameters>;
    std::string name = "";
    bool isGLES3 = false;
    std::string shaderID;
    std::string shaderName="";
    std::string precision="highp";
    std::string fragmentShader="";
    std::string vertexShader="";
    std::unordered_map<std::string, std::string> defines;
    std::unordered_map<std::string, float> definesFloat;
    bool isRawShaderMaterial = false;
    bool instancingColor = false;
    bool supportsVertexTextures = false;
    TextureEncoding outputEncoding = TextureEncoding::Unknown;
    bool instancing = false;
    bool map = false;
    TextureEncoding mapEncoding = TextureEncoding::Unknown;
    bool matcap = false;
    TextureEncoding matcapEncoding = TextureEncoding::Unknown;
    bool envMap = false;
    TextureMapping envMapMode = TextureMapping::Unknown;
    TextureEncoding envMapEncoding = TextureEncoding::Unknown;
    bool envMapCubeUV = false;
    int envMapCubeUVHeight = 0;
    bool lightMap = false;
    TextureEncoding lightMapEncoding = TextureEncoding::Unknown;
    bool aoMap = false;
    bool emissiveMap = false;
    TextureEncoding emissiveMapEncoding = TextureEncoding::Unknown;
    bool bumpMap = false;
    bool normalMap = false;
    //todo:fix this
    bool clearcoat = false;
    bool clearcoatMap = false;
    bool clearcoatRoughnessMap = false;
    bool objectSpaceNormalMap = false;
    bool tangentSpaceNormalMap = false;

    bool decodeVideoTexture = false;

    bool clearcoatNormalMap = false;
    bool displacementMap = false;
    bool specularMap = false;
    bool roughnessMap = false;
    bool metalnessMap = false;
    bool gradientMap = false;
    bool alphaMap = false;

    //todo:new para
    bool opaque = false;

    Combine combine = Combine::None;
    bool vertexColors = false;
    bool vertexTangents = false;
    bool vertexUvs = false;
    bool uvsVertexOnly = false;
    bool fog = false;
    bool useFog = false;
    bool fogExp2 = false;
    bool flatShading = false;
    bool sizeAttenuation = false;
    bool logarithmicDepthBuffer = false;
    bool skinning = false;
    int maxBones = 0;
    bool useVertexTexture = false;
    bool morphTargets = false;
    bool morphNormals = false;
    int maxMorphTargets =0;
    int maxMorphNormals = 0;;
    bool premultipliedAlpha = false;
    int numDirLights = 0;
    int numPointLights = 0;
    int numSpotLights = 0;
    int numSpotLightMaps = 0;
    int numHemiLights = 0;
    int numRectAreaLights = 0;
    int numDirLightShadows = 0;
    int numPointLightShadows =0;
    int numSpotLightShadowsWithMaps =0;
    int numSpotLightShadows =0;
    bool shadowMapEnabled = false;
    ShadowMapType shadowMapType = ShadowMapType::NoneShadowMap;
    ToneMapping toneMapping = ToneMapping::NoToneMapping;
    bool physicallyCorrectLights = false;
    float alphaTest = false;
    Side side = Side::None;
    bool doubleSided = false;
    bool flipSided = false;
    int numClippingPlanes = 0;
    int numClipIntersection = 0;
    DepthPackingStrategies depthPacking = DepthPackingStrategies::None;
    bool dithering = false;
    bool sheen = false;
    bool transmissionMap = false;
    bool extensionDerivatives = false;
    bool extensionFragDepth = false;
    bool extensionDrawBuffers = false;
    bool rendererExtensionDrawBuffers = false;
    bool rendererExtensionFragDepth = false;
    bool shaderTextureLOD = false;
    bool extensionShaderTextureLOD = false;
    bool renderExtensionShaderTextureLOD = false;
    GLSLVersion glslVersion;
    std::string indexOfAttributeName;
    std::string customProgramCacheKey;

    //UniformValues uniforms;
    std::shared_ptr<UniformValues> uniforms;
};

struct EnvMapCubeUVSize{
    float texelWidth;
    float texelHeight;
    float maxMip;

    using sptr = std::shared_ptr<EnvMapCubeUVSize>;
};


#endif //THREE_CPP_GL_PROGRAM_PARAMETERS_H
