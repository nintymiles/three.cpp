//
// Created by SeanR on 2023/10/18.
//
#include "gl_programs.h"

#include "skinned_mesh.h"
#include "instanced_mesh.h"
#include "mesh_standard_material.h"
#include "shader_material.h"
#include "shader_chunk.h"
#include "shader_lib.h"
#include "scene.h"
#include "gl_renderer.h"
#include "timer.h"
#include "gl_program.h"
#include "gl_clipping.h"
#include "gl_cube_maps.h"

using namespace std;
TextureEncoding getTextureEncodingFromMap(const Texture::sptr& map){
    TextureEncoding encoding = map ? map->encoding : TextureEncoding::LinearEncoding;
    //int encoding = (int)TextureEncoding::LinearEncoding;

    return encoding;
}

int GLPrograms::getMaxBones(const SkinnedMesh& skinnedMesh)
{
    auto skeleton = skinnedMesh.skeleton;
    auto bones = skeleton->bones;

    if (capabilities->floatVertexTextures)
    {
        return 2014;
    }
    else
    {
        // default for when object is not specified
        // ( for example when prebuilding shader to be used with multiple objects )
        //
        //  - leave some extra space for other uniforms
        //  - limit here is ANGLE's 254 max uniform vectors
        //    (up to 54 should be safe)

        auto nVertexUniforms = maxVertexUniforms;
        auto nVertexMatrices = (int)floor((float)((nVertexUniforms - 20) / 4));

        auto maxBones = std::min(nVertexMatrices,(int)bones.size());

        if (maxBones < bones.size())
        {
            //Trace.TraceWarning("THREE.Renderers.GLRenderer: Skeleton has" + bones.Length + " bones. This GPU supports " + maxBones + ".");
            return 0;
        }

        return maxBones;
    }
}

GLPrograms::GLPrograms(GLCubeMaps* cubeMaps,GLCubeUVMaps* cubeUVMaps, const GLExtensions::sptr& extensions, const GLCapabilities::sptr& capabilities, const GLBindingStates::sptr& bindingStates, std::shared_ptr<GLClipping>& clipping)
        :cubeMaps(cubeMaps),cubeUVMaps(cubeUVMaps),extensions(extensions),capabilities(capabilities),bindingStates(bindingStates),clipping(clipping){
    isGLES3 = capabilities->isGLES3;
    logarithmicDepthBuffer = capabilities->logarithmicDepthBuffer;
    floatVertexTextures = capabilities->floatVertexTextures;
    precision = capabilities->precision;
    maxVertexUniforms = capabilities->maxVertexUniforms;
    vertexTextures = capabilities->vertexTextures;
}

std::shared_ptr<ProgramParameters> GLPrograms::getParameters(GLRenderer& renderer,const Material::sptr& material, GLLights& lights, const vector<Light::sptr>& shadows, const Scene::sptr scene, Object3D& object){

    std::shared_ptr<ProgramParameters> parameters = std::make_shared<ProgramParameters>();

    Texture::sptr environment = instanceOf<MeshStandardMaterial>(material.get()) ? scene->environment : nullptr;

    Texture::sptr envMap = cubeMaps->get(material->envMap != nullptr ? material->envMap : nullptr);

    std::string vertexShader = "";
    std::string fragmentShader = "";

    if (!material->shaderId.empty()) {
        GLShader shader = getShader(material->shaderId);
        vertexShader = shader.vertexShader;
        fragmentShader = shader.fragmentShader;
    }
    else {
        ShaderMaterial::sptr shaderMaterial = std::dynamic_pointer_cast<ShaderMaterial>(material);
        vertexShader = shaderMaterial->vertexShader;
        fragmentShader = shaderMaterial->fragmentShader;
    }

    int maxBones = object.type=="SkinnedMesh" ? getMaxBones(*reinterpret_cast<SkinnedMesh *>(&object)) : 0;


    if (!material->precision.empty()) {
        precision = capabilities->getMaxPrecision(material->precision);
        if (precision != material->precision) {

        }
    }

    //material->onBeforeCompile.emitSignal(*shaderobject, renderer);

    GLRenderTarget::sptr currentRenderTarget = renderer._currentRenderTarget;

    //int numMultiviewViews = currentRenderTarget != nullptr && currentRenderTarget->isGLMultiviewRenderTarget ? currentRenderTarget->numViews : 0;
    auto useAlphaTest = material->alphaTest > 0;
    auto useClearcoat = material->clearcoat > 0;
    auto useIridescence = material->iridescence > 0;

    parameters->isGLES3 = true;// only GLES3;
    parameters->shaderID =material->shaderId;
    parameters->shaderName =material->type;

    //parameters->uniforms =shaderobject->uniforms);
    parameters->vertexShader = vertexShader;
    parameters->fragmentShader = fragmentShader;

    parameters->defines = material->definesString;
    parameters->definesFloat = material->definesFloat;
    parameters->isRawShaderMaterial = material->isRawShaderMaterial;
    parameters->glslVersion = material->glslVersion;

    parameters->precision = precision;

    parameters->instancing = object.type=="InstancedMesh";
    parameters->instancingColor = (instanceOf<InstancedMesh>(&object) && object.instanceColor) ? true:false;

    parameters->supportsVertexTextures = vertexTextures;
    parameters->outputEncoding = currentRenderTarget!=nullptr ? getTextureEncodingFromMap(currentRenderTarget->texture) : renderer.outputEncoding;
    parameters->map = material->map!=nullptr;
    parameters->mapEncoding = getTextureEncodingFromMap(material->map);

    parameters->matcap =material->matcap!=nullptr;
    parameters->matcapEncoding =getTextureEncodingFromMap(material->matcap);
    parameters->envMap =material->envMap!=nullptr;
    parameters->envMapMode =material->envMap!=nullptr ? material->envMap->mapping : TextureMapping::Unknown;
    parameters->envMapEncoding =getTextureEncodingFromMap(material->envMap);
    parameters->envMapCubeUV =(material->envMap!=nullptr) && ((material->envMap->mapping == TextureMapping::CubeUVReflectionMapping) || (material->envMap->mapping == TextureMapping::CubeUVRefractionMapping));
    parameters->lightMap =material->lightMap!=nullptr;
    parameters->lightMapEncoding =getTextureEncodingFromMap(material->lightMap);
    parameters->aoMap =material->aoMap!=nullptr;
    parameters->emissiveMap =material->emissiveMap!=nullptr;
    parameters->emissiveMapEncoding =getTextureEncodingFromMap(material->emissiveMap);
    parameters->bumpMap =material->bumpMap!=nullptr;
    parameters->normalMap =material->normalMap!=nullptr;
    parameters->objectSpaceNormalMap =material->normalMapType == NormalMapTypes::ObjectSpaceNormalMap;
    parameters->tangentSpaceNormalMap =material->normalMapType == NormalMapTypes::TangentSpaceNormalMap;

    parameters->decodeVideoTexture = material->map && ( material->map->isVideoTexture == true ) && ( material->map->encoding == TextureEncoding::sRGBEncoding );

    parameters->clearcoat = useClearcoat;
    parameters->clearcoatMap =material->clearcoatMap!=nullptr;
    parameters->clearcoatRoughnessMap =material->clearcoatRoughnessMap!=nullptr;
    parameters->clearcoatNormalMap =material->clearcoatNormalMap!=nullptr;

//    iridescence: useIridescence,
//    iridescenceMap: useIridescence && !! material.iridescenceMap,
//    iridescenceThicknessMap: useIridescence && !! material.iridescenceThicknessMap,

    parameters->displacementMap = material->displacementMap!=nullptr;
    parameters->roughnessMap =material->roughnessMap!=nullptr;
    parameters->metalnessMap =material->metalnessMap!=nullptr;
    parameters->specularMap = material->specularMap != nullptr;

//    specularIntensityMap: !! material.specularIntensityMap,
//    specularColorMap: !! material.specularColorMap,

    parameters-> opaque = material->transparent == false && material->blending == Blending::NormalBlending;

    parameters->alphaMap =material->alphaMap!=nullptr;
    //todo:fix this
    //parameters->alphaTest = useAlphaTest;
    parameters->alphaTest =material->alphaTest;

    parameters->gradientMap =material->gradientMap!=nullptr;

    bool isSheen = material->sheen.isNull();
    parameters->sheen = !isSheen;
//    sheenColorMap: !! material.sheenColorMap,
//    sheenRoughnessMap: !! material.sheenRoughnessMap,

//    transmission: material.transmission > 0,
//    thicknessMap: !! material.thicknessMap,
    parameters->transmissionMap = material->transmissionMap != nullptr;

    parameters->combine =material->combine;

    parameters->vertexTangents =material->normalMap!=nullptr && material->vertexTangents;
    parameters->vertexColors =material->vertexColors;
    parameters->vertexUvs =material->map!=nullptr || material->bumpMap!=nullptr || material->normalMap!=nullptr || material->specularMap!=nullptr || material->alphaMap!=nullptr || material->emissiveMap!=nullptr || material->roughnessMap!=nullptr || material->metalnessMap!=nullptr || material->clearcoatMap!=nullptr || material->clearcoatRoughnessMap!=nullptr || material->clearcoatNormalMap!=nullptr || material->displacementMap!=nullptr;
    parameters->uvsVertexOnly =!(material->map!=nullptr || material->bumpMap!=nullptr || material->normalMap!=nullptr || material->specularMap!=nullptr || material->alphaMap || material->emissiveMap!=nullptr || material->roughnessMap!=nullptr || material->metalnessMap!=nullptr|| material->clearcoatNormalMap!=nullptr) && material->displacementMap!=nullptr;

    parameters->fog =scene->fog!=nullptr;
    parameters->useFog =material->fog;
    parameters->fogExp2 =scene->fog!=nullptr && scene->fog->type=="FogExp2";

    parameters->flatShading =material->flatShading;

    parameters->sizeAttenuation =material->sizeAttenuation;
    parameters->logarithmicDepthBuffer =logarithmicDepthBuffer;

    parameters->skinning =material->skinning && maxBones > 0;
    parameters->maxBones =maxBones;
    parameters->useVertexTexture =floatVertexTextures;

    parameters->morphTargets =material->morphTargets;
    parameters->morphNormals =material->morphNormals;
    parameters->maxMorphTargets =renderer.maxMorphTargets;
    parameters->maxMorphNormals =renderer.maxMorphNormals;

    parameters->numDirLights =lights.state.directional.size();
    parameters->numPointLights =lights.state.point.size();
    parameters->numSpotLights =lights.state.spot.size();
    parameters->numSpotLightMaps =lights.state.spotLightMap.size();
    parameters->numRectAreaLights =lights.state.rectArea.size();
    parameters->numHemiLights =lights.state.hemi.size();

    parameters->numDirLightShadows =lights.state.directionalShadowMap.size();
    parameters->numPointLightShadows =lights.state.pointShadowMap.size();
    parameters->numSpotLightShadows =lights.state.spotShadowMap.size();
    parameters->numSpotLightShadowsWithMaps = lights.state.numSpotLightShadowsWithMaps;

    parameters->numClippingPlanes = clipping->numPlanes;
    parameters->numClipIntersection = clipping->numIntersection;

    parameters->dithering =material->dithering;

    parameters->shadowMapEnabled =renderer.shadowMap->enabled && shadows.size() > 0;
    parameters->shadowMapType =renderer.shadowMap->type;

    parameters->toneMapping =material->toneMapped ? renderer.toneMapping :ToneMapping::NoToneMapping;
    parameters->physicallyCorrectLights =renderer.physicallyCorrectLights;

    parameters->premultipliedAlpha =material->premultipliedAlpha;

    parameters->doubleSided =material->side == Side::DoubleSide;
    parameters->flipSided = material->side == Side::BackSide;

//    useDepthPacking: !! material.depthPacking,
    parameters->depthPacking =material->depthPacking;

    //todo:weired name
//    index0AttributeName: material.index0AttributeName,
    parameters->indexOfAttributeName = material->indexOfAttributeName;

    parameters->extensionDerivatives =material->extensions.derivatives;
    parameters->extensionFragDepth =material->extensions.fragDepth;
    parameters->extensionDrawBuffers =material->extensions.drawBuffers;
    parameters->extensionShaderTextureLOD =material->extensions.shaderTextureLOD;

    bool rendererExtensionFragDepth = isGLES3 || extensions->get(Extension::EXT_frag_depth);
    parameters->rendererExtensionFragDepth = rendererExtensionFragDepth;

    bool rendererExtensionDrawBuffers = isGLES3 || extensions->get(Extension::EXT_draw_buffers);
    parameters->rendererExtensionDrawBuffers = rendererExtensionDrawBuffers;

    bool rendererExtensionShaderTextureLod = isGLES3 || extensions->get(Extension::EXT_shader_texture_lod);
    parameters->renderExtensionShaderTextureLOD = rendererExtensionShaderTextureLod;

    parameters->customProgramCacheKey = material->customProgramCacheKey;


    //cout << "getParameter:"<< duration << endl;

    return parameters;

}

std::string GLPrograms::getProgramCacheKey(const Material& material, const ProgramParameters& parameters){
    Timer sw;
    std::vector<std::string> array;

    if (!parameters.shaderID.empty()) {
        array.push_back(parameters.shaderID);
    }
    else {
        array.push_back(parameters.fragmentShader);
        array.push_back(parameters.vertexShader);
    }

    if (parameters.defines.size() > 0) {
        for (auto& it : parameters.defines) {
            array.push_back(it.first);
            array.push_back(it.second);

        }
    }

    if (!parameters.isRawShaderMaterial) {
        array.push_back(parameters.precision);
        array.push_back(std::to_string(parameters.isGLES3));
        array.push_back(std::to_string(parameters.supportsVertexTextures));
        array.push_back(std::to_string((int)parameters.outputEncoding));
        array.push_back(std::to_string(parameters.instancing));
        array.push_back(std::to_string(parameters.map));
        array.push_back(std::to_string((int)parameters.mapEncoding));
        array.push_back(std::to_string(parameters.matcap));
        array.push_back(std::to_string((int)parameters.matcapEncoding));
        array.push_back(std::to_string(parameters.envMap));
        array.push_back(std::to_string((int)parameters.envMapMode));
        array.push_back(std::to_string((int)parameters.envMapEncoding));
        array.push_back(std::to_string(parameters.envMapCubeUV));
        array.push_back(std::to_string(parameters.lightMap));
        array.push_back(std::to_string(parameters.aoMap));
        array.push_back(std::to_string(parameters.emissiveMap));
        array.push_back(std::to_string((int)parameters.emissiveMapEncoding));
        array.push_back(std::to_string(parameters.bumpMap));
        array.push_back(std::to_string(parameters.normalMap));
        array.push_back(std::to_string(parameters.objectSpaceNormalMap));
        array.push_back(std::to_string(parameters.tangentSpaceNormalMap));
        array.push_back(std::to_string(parameters.clearcoatNormalMap));
        array.push_back(std::to_string(parameters.displacementMap));
        array.push_back(std::to_string(parameters.specularMap));
        array.push_back(std::to_string(parameters.roughnessMap));
        array.push_back(std::to_string(parameters.metalnessMap));
        array.push_back(std::to_string(parameters.gradientMap));
        array.push_back(std::to_string(parameters.alphaMap));
        array.push_back(std::to_string((int)parameters.combine));
        array.push_back(std::to_string(parameters.vertexColors));
        array.push_back(std::to_string(parameters.vertexTangents));
        array.push_back(std::to_string(parameters.vertexUvs));
        array.push_back(std::to_string(parameters.fog));
        array.push_back(std::to_string(parameters.useFog));
        array.push_back(std::to_string(parameters.fogExp2));
        array.push_back(std::to_string(parameters.flatShading));
        array.push_back(std::to_string(parameters.sizeAttenuation));
        array.push_back(std::to_string(parameters.logarithmicDepthBuffer));
        array.push_back(std::to_string(parameters.skinning));
        array.push_back(std::to_string(parameters.maxBones));
        array.push_back(std::to_string(parameters.useVertexTexture));
        array.push_back(std::to_string(parameters.morphTargets));
        array.push_back(std::to_string(parameters.morphNormals));
        array.push_back(std::to_string(parameters.maxMorphTargets));
        array.push_back(std::to_string(parameters.maxMorphNormals));
        array.push_back(std::to_string(parameters.premultipliedAlpha));
        array.push_back(std::to_string(parameters.numDirLights));
        array.push_back(std::to_string(parameters.numPointLights));
        array.push_back(std::to_string(parameters.numSpotLights));
        array.push_back(std::to_string(parameters.numSpotLightMaps));
        array.push_back(std::to_string(parameters.numHemiLights));
        array.push_back(std::to_string(parameters.numRectAreaLights));
        array.push_back(std::to_string(parameters.numDirLightShadows));
        array.push_back(std::to_string(parameters.numPointLightShadows));
        array.push_back(std::to_string(parameters.numSpotLightShadows));
        array.push_back(std::to_string(parameters.numSpotLightShadowsWithMaps));
        array.push_back(std::to_string(parameters.shadowMapEnabled));
        array.push_back(std::to_string((int)parameters.shadowMapType));
        array.push_back(std::to_string((int)parameters.toneMapping));
        array.push_back(std::to_string(parameters.physicallyCorrectLights));
        array.push_back(std::to_string(parameters.alphaTest));
        array.push_back(std::to_string(parameters.doubleSided));
        array.push_back(std::to_string(parameters.flipSided));
        array.push_back(std::to_string(parameters.numClippingPlanes));
        array.push_back(std::to_string(parameters.numClipIntersection));
        array.push_back(std::to_string((int)parameters.depthPacking));
        array.push_back(std::to_string(parameters.dithering));
        array.push_back(std::to_string(parameters.sheen));
    }

    stringstream ss;
    for (unsigned i = 0; i < array.size(); i++)
        if (i != (array.size() - 1))
            ss << array[i] << ",";
        else
            ss << array[i];

    //cout<< "getProgramCache :"<< sw.get_elapsed_time()<<endl;

    return ss.str();

}

GLProgram::sptr GLPrograms::acquireProgram(GLRenderer& renderer,const ProgramParameters& parameters, const string& code){
    GLProgram::sptr program = nullptr;

    for (auto p = 0;p < programs.size();p++) {
        auto programInfo = programs[p];

        if (programInfo->cacheKey == code) {
            program = programInfo;
            ++(program->usedTimes);
            break;
        }
    }
    if (program == nullptr) {
        //cant create a local object,then assigned it to a sharedptr and worked
        //GLProgram glProgram(renderer, extensions, code, parameters,bindingStates);

        program = std::make_shared<GLProgram>(renderer, extensions, code, parameters,bindingStates);
        programs.push_back(program);
    }
    return program;
}

void GLPrograms::releaseProgram(GLProgram::sptr& program)
{
    if (--(program->usedTimes) == 0) {
        auto it = std::find(programs.begin(), programs.end(), program);
        if (it != programs.end()) {
            int index = std::distance(programs.begin(), it);
            programs.erase(programs.begin() + index);

            program.reset();
        }
    }
}

std::shared_ptr<UniformValues> GLPrograms::getUniforms(const Material::sptr& material){
    if (!material->shaderId.empty()) {
        GLShader shader = getShader(material->shaderId);
        return std::make_shared<UniformValues>(shader.getUniforms());//UniformValues::sptr();//todo:fix this std::make_shared<UniformValues>(shader.getUniforms());
    } else {
        return (material->uniforms);
    }
}
