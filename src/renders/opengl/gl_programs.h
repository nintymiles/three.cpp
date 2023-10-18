//
// Created by SeanR on 2023/10/15.
//

#ifndef THREE_CPP_GL_PROGRAMS_H
#define THREE_CPP_GL_PROGRAMS_H

#include <string>
#include <vector>

#include "gl_extensions.h"
#include "gl_capabilities.h"
#include "gl_binding_states.h"
#include "gl_clipping.h"
#include "gl_program.h"

class Material;
class Light;
class GLLights;
class Scene;
class Object3D;
class SkinnedMesh;
class GLRenderer;

class GLCubeMap;

class GLPrograms {
private:
    int getMaxBones(const SkinnedMesh& skinnedMesh);
protected:


    bool isGLES3 = false;

    bool logarithmicDepthBuffer = false;

    bool floatVertexTextures = false;

    std::string precision;

    int maxVertexUniforms = 0;

    bool vertexTextures = false;


    /*std::unordered_map<size_t, std::string> shaderIDs = {
        {getStringHashcode("MeshDepthMaterial"), "depth"},
        {getStringHashcode("MeshDistanceMaterial"), "distanceRGBA"},
        {getStringHashcode("MeshNormalMaterial"), "normal"},
        {getStringHashcode("MeshBasicMaterial"), "basic"},
        {getStringHashcode("MeshLambertMaterial"), "lambert"},
        {getStringHashcode("MeshPhongMaterial"), "phong"},
        {getStringHashcode("MeshToonMaterial"), "toon"},
        {getStringHashcode("MeshStandardMaterial"), "physical"},
        {getStringHashcode("MeshPhysicalMaterial"), "physical"},
        {getStringHashcode("MeshMatcapMaterial"), "matcap"},
        {getStringHashcode("LineBasicMaterial"), "basic"},
        {getStringHashcode("LineDashedMaterial"), "dashed"},
        {getStringHashcode("PointsMaterial"), "points"},
        {getStringHashcode("ShadowMaterial"), "shadow"},
        {getStringHashcode("SpriteMaterial"), "sprite"}
    };*/

    std::vector<std::string> parameterNames = {
            "precision","isGLES3","supportsVertexTextures",	"outputEncoding","instancing","instancingColor",
            "map","mapEncoding","matcap","matcapEncoding","envMap","envMapMode",	"envMapEncoding","envMapCubeUV",
            "lightMap","lightMapEncoding","aoMap","emissiveMap","emissiveMapEncoding","bumpMap","normalMap","objectSpaceNormalMap","tangentSpaceNormalMap","clearcoatMap","clearcoatNormalMap","clearcoatNormalMap","displacementMap","specularMap",
            "roughnessMap","metalnessMap","gradientMap",
            "alphaMap","combine","vertexColors",	"vertexTangents","vertexUvs","uvsVertexOnly","fog","useFog","fogExp2",
            "flatShading","sizeAttenuation","logarithmicDepthBuffer","skinning",
            "maxBones","useVertexTexture","morphTargets",	"morphNormals",
            "maxMorphTargets",	"maxMorphNormals",	"premultipliedAlpha",
            "numDirLights","numPointLights","numSpotLights","numHemiLights","numRectAreaLights","numDirLightShadows",	"numPointLightShadows","numSpotLightShadows",
            "shadowMapEnabled","shadowMapType","toneMapping","physicallyCorrectLights",
            "alphaTest","doubleSided",	"flipSided","numClippingPlanes","numClipIntersection","depthPacking","dithering",
            "sheen","transmissionMap"
    };

    GLExtensions::sptr extensions;

    GLCapabilities::sptr capabilities;

    GLCubeMap& cubeMaps;

    GLBindingStates::sptr bindingStates;

    GLClipping& clipping;

public :
    using sptr = std::shared_ptr<GLPrograms>;

    std::vector<GLProgram::sptr> programs;

    GLPrograms(GLCubeMap& cubeMaps,const GLExtensions::sptr& extensions,const GLCapabilities::sptr& capabilities,const GLBindingStates::sptr& bindingStates,GLClipping& clipping);

    virtual ~GLPrograms() = default;

    std::shared_ptr<ProgramParameters> getParameters(GLRenderer& renderer,const Material::sptr& material, GLLights& lights, const std::vector<Light::sptr>& shadows,const Scene* scene, Object3D& object);

    std::string getProgramCacheKey(const Material& material, const ProgramParameters& parameters);

    GLProgram::sptr acquireProgram(GLRenderer& renderer,const ProgramParameters& parameters, const std::string& code);

    void releaseProgram(GLProgram::sptr& program);

    UniformValues getUniforms(const Material::sptr& material);

};

#endif //THREE_CPP_GL_PROGRAMS_H
