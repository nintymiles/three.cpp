//
// Created by SeanR on 2023/10/16.
//
#include "gl_program.h"

#include "material.h"
#include "constants.h"
#include "shader_chunk.h"
#include "gl_renderer.h"
#include "gl_binding_states.h"
#include "gl_uniforms.h"

//todo:fix this
#define OPENGL_ES_3_2

unsigned GLProgram::programId = 0;

void sendDebugMessage(const std::wstring& name) {
    std::wcout << name << std::endl;
}

GLProgram::GLProgram(GLRenderer& renderer, const GLExtensions::sptr& extensions, const std::string& cacheKey, const ProgramParameters& parameters, const std::shared_ptr<GLBindingStates>& bindingStates)
        :renderer(renderer), bindingStates(bindingStates){
    id = GLProgram::programId++;

    this->cacheKey = cacheKey;

    std::unordered_map<std::string,std::string> definesString = parameters.defines;

    std::string vShader = parameters.vertexShader;

    std::string fShader = parameters.fragmentShader;

    std::string shadowMapTypeDefine = generateShadowMapTypeDefine(parameters);

    std::string envMapTypeDefine = generateEnvMapTypeDefine(parameters);

    std::string envMapModeDefine = generateEnvMapModeDefine(parameters);

    std::string envMapBlendingDefine = generateEnvMapBlendingDefine(parameters);

    float gammaFactorDefine = (renderer.gammaFactor > 0) ? renderer.gammaFactor : 1.0f;

    std::string customExtensions = parameters.isGLES3 ? "" : generateExtensions(parameters);

    std::string customDefines = generateDefines(definesString);

    //std::string customDefinesFloat = generateDefines(material.definesFloat);

    this->program = glCreateProgram();

    std::stringstream prefixVertex, prefixFragment;

    GLSLVersion glslVersion = parameters.glslVersion;


    if (parameters.isRawShaderMaterial) {
        prefixVertex << customDefines << std::endl;
        if (customDefines.size() > 0)
            prefixVertex << std::endl;

        prefixFragment << customExtensions << customDefines << std::endl;
        if (!customExtensions.empty() || !customDefines.empty())
            prefixFragment << std::endl;
    } else {
#ifdef OPENGL_ES_3_2
        prefixVertex << "#version 300 es" << std::endl;
        prefixVertex << generatePrecision(parameters) << std::endl;
#else
        prefixVertex << "#version 440" << std::endl;
        prefixVertex << generatePrecision(parameters) << std::endl;
        //prefixVertex << "#version 140" << std::endl;
#endif
        prefixVertex << "#define attribute in" << std::endl;
        prefixVertex << "#define varying out" << std::endl;
        prefixVertex << "#define texture2D texture" << std::endl;

        prefixVertex << "#define SHADER_NAME " << parameters.shaderName << std::endl;

        prefixVertex << customDefines;

        if (parameters.instancing) prefixVertex << "#define USE_INSTANCING" << std::endl;
        if(parameters.instancingColor) prefixVertex << "#define USE_INSTANCING_COLOR" << std::endl;
        if (parameters.supportsVertexTextures) prefixVertex << "#define VERTEX_TEXTURES" << std::endl;

        prefixVertex << "#define GAMMA_FACTOR " << std::to_string(gammaFactorDefine) << std::endl;

        prefixVertex << "#define MAX_BONES " << parameters.maxBones << std::endl;

        if (parameters.useFog && parameters.fog) prefixVertex << "#define USE_FOG" << std::endl;
        if (parameters.useFog && parameters.fogExp2) prefixVertex << "#define FOG_EXP2" << std::endl;

        if (parameters.map)  prefixVertex << "#define USE_MAP" << std::endl;
        if (parameters.envMap) prefixVertex << "#define USE_ENVMAP" << std::endl;
        if (parameters.envMap) prefixVertex << "#define " + envMapModeDefine << std::endl;
        if (parameters.lightMap) prefixVertex << "#define USE_LIGHTMAP" << std::endl;
        if (parameters.aoMap) prefixVertex << "#define USE_AOMAP" << std::endl;
        if (parameters.emissiveMap) prefixVertex << "#define USE_EMISSIVEMAP" << std::endl;
        if (parameters.bumpMap) prefixVertex << "#define USE_BUMPMAP" << std::endl;
        if (parameters.normalMap) prefixVertex << "#define USE_NORMALMAP" << std::endl;
        if (parameters.normalMap && parameters.objectSpaceNormalMap) prefixVertex << "#define OBJECTSPACE_NORMALMAP" << std::endl;
        if (parameters.normalMap && parameters.tangentSpaceNormalMap) prefixVertex << "#define TANGENTSPACE_NORMALMAP" << std::endl;

        if (parameters.clearcoatMap) prefixVertex << "#define USE_CLEARCOATMAP" << std::endl;
        if (parameters.clearcoatRoughnessMap) prefixVertex << "#define USE_CLEARCOAT_ROUGHNESSMAP" << std::endl;
        if (parameters.clearcoatNormalMap) prefixVertex << "#define USE_CLEARCOAT_NORMALMAP" << std::endl;
        if (parameters.displacementMap && parameters.supportsVertexTextures) prefixVertex << "#define USE_DISPLACEMENTMAP" << std::endl;
        if (parameters.specularMap) prefixVertex << "#define USE_SPECULARMAP" << std::endl;
        if (parameters.roughnessMap) prefixVertex << "#define USE_ROUGHNESSMAP" << std::endl;
        if (parameters.metalnessMap) prefixVertex << "#define USE_METALNESSMAP" << std::endl;
        if (parameters.alphaMap) prefixVertex << "#define USE_ALPHAMAP" << std::endl;
        if (parameters.transmissionMap) prefixVertex << "#define USE_TRANSMISSIONMAP" << std::endl;

        if (parameters.vertexTangents) prefixVertex << "#define USE_TANGENT" << std::endl;
        if (parameters.vertexColors) prefixVertex << "#define USE_COLOR" << std::endl;
        if (parameters.vertexUvs) prefixVertex << "#define USE_UV" << std::endl;
        if (parameters.uvsVertexOnly) prefixVertex << "#define UVS_VERTEX_ONLY" << std::endl;

        if (parameters.flatShading) prefixVertex << "#define FLAT_SHADED" << std::endl;

        if (parameters.skinning) prefixVertex << "#define USE_SKINNING" << std::endl;
        if (parameters.useVertexTexture) prefixVertex << "#define BONE_TEXTURE" << std::endl;

        if (parameters.morphTargets) prefixVertex << "#define USE_MORPHTARGETS" << std::endl;
        if (parameters.morphNormals && parameters.flatShading == false) prefixVertex << "#define USE_MORPHNORMALS" << std::endl;
        if (parameters.doubleSided) prefixVertex << "#define DOUBLE_SIDED" << std::endl;
        if (parameters.flipSided) prefixVertex << "#define FLIP_SIDED" << std::endl;

        if (parameters.shadowMapEnabled) prefixVertex << "#define USE_SHADOWMAP" << std::endl;
        if (parameters.shadowMapEnabled) prefixVertex << "#define " + shadowMapTypeDefine << std::endl;

        if (parameters.sizeAttenuation) prefixVertex << "#define USE_SIZEATTENUATION" << std::endl;

        if (parameters.logarithmicDepthBuffer) prefixVertex << "#define USE_LOGDEPTHBUF" << std::endl;
        if (parameters.logarithmicDepthBuffer && parameters.rendererExtensionFragDepth) prefixVertex << "#define USE_LOGDEPTHBUF_EXT" << std::endl;

        prefixVertex << "uniform mat4 modelMatrix;" << std::endl;
        prefixVertex << "uniform mat4 modelViewMatrix;" << std::endl;
        prefixVertex << "uniform mat4 projectionMatrix;" << std::endl;
        prefixVertex << "uniform mat4 viewMatrix;" << std::endl;
        prefixVertex << "uniform mat3 normalMatrix;" << std::endl;
        prefixVertex << "uniform vec3 cameraPosition;" << std::endl;
        prefixVertex << "uniform bool isOrthographic;" << std::endl;

        prefixVertex << "#ifdef USE_INSTANCING" << std::endl;

        prefixVertex << " attribute mat4 instanceMatrix;" << std::endl;

        prefixVertex << "#endif" << std::endl;

        prefixVertex << "#ifdef USE_INSTANCING_COLOR" << std::endl;

        prefixVertex << " attribute vec3 instanceColor;" << std::endl;

        prefixVertex << "#endif" << std::endl;


        prefixVertex << "attribute vec3 position;" << std::endl;
        prefixVertex << "attribute vec3 normal;" << std::endl;
        prefixVertex << "attribute vec2 uv;" << std::endl;

        prefixVertex << "#ifdef USE_TANGENT" << std::endl;

        prefixVertex << "	attribute vec4 tangent;" << std::endl;

        prefixVertex << "#endif" << std::endl;

        prefixVertex << "#ifdef USE_COLOR" << std::endl;

        prefixVertex << "	attribute vec3 color;" << std::endl;

        prefixVertex << "#endif" << std::endl;

        prefixVertex << "#ifdef USE_MORPHTARGETS" << std::endl;

        prefixVertex << "	attribute vec3 morphTarget0;" << std::endl;
        prefixVertex << "	attribute vec3 morphTarget1;" << std::endl;
        prefixVertex << "	attribute vec3 morphTarget2;" << std::endl;
        prefixVertex << "	attribute vec3 morphTarget3;" << std::endl;

        prefixVertex << "	#ifdef USE_MORPHNORMALS" << std::endl;

        prefixVertex << "		attribute vec3 morphNormal0;" << std::endl;
        prefixVertex << "		attribute vec3 morphNormal1;" << std::endl;
        prefixVertex << "		attribute vec3 morphNormal2;" << std::endl;
        prefixVertex << "		attribute vec3 morphNormal3;" << std::endl;

        prefixVertex << "	#else" << std::endl;

        prefixVertex << "		attribute vec3 morphTarget4;" << std::endl;
        prefixVertex << "		attribute vec3 morphTarget5;" << std::endl;
        prefixVertex << "		attribute vec3 morphTarget6;" << std::endl;
        prefixVertex << "		attribute vec3 morphTarget7;" << std::endl;

        prefixVertex << "	#endif" << std::endl;

        prefixVertex << "#endif" << std::endl;

        prefixVertex << "#ifdef USE_SKINNING" << std::endl;

        prefixVertex << "	attribute vec4 skinIndex;" << std::endl;
        prefixVertex << "	attribute vec4 skinWeight;" << std::endl;

        prefixVertex << "#endif" << std::endl;


#ifdef OPENGL_ES_3_2
        prefixFragment << "#version 300 es" << std::endl;
        prefixFragment << generatePrecision(parameters) << std::endl;

#else
        prefixFragment << "#version 440" << std::endl;
        prefixFragment << generatePrecision(parameters) << std::endl;
        //prefixFragment << "#version 140" << std::endl;
#endif
        prefixFragment << customExtensions << std::endl;
        prefixFragment << "#define varying in" << std::endl;
        prefixFragment << "out highp vec4 pc_fragColor;" << std::endl;
        prefixFragment << "#define gl_FragColor pc_fragColor" << std::endl;
        prefixFragment << "#define gl_FragDepthEXT gl_FragDepth" << std::endl;
        prefixFragment << "#define texture2D texture" << std::endl;
        prefixFragment << "#define textureCube texture" << std::endl;
        prefixFragment << "#define texture2DProj textureProj" << std::endl;
        prefixFragment << "#define texture2DLodEXT textureLod" << std::endl;
        prefixFragment << "#define texture2DProjLodEXT textureProjLod" << std::endl;
        prefixFragment << "#define textureCubeLodEXT textureLod" << std::endl;
        prefixFragment << "#define texture2DGradEXT textureGrad" << std::endl;
        prefixFragment << "#define texture2DProjGradEXT textureProjGrad" << std::endl;
        prefixFragment << "#define textureCubeGradEXT textureGrad" << std::endl;


        prefixFragment << "#define SHADER_NAME " + parameters.shaderName << std::endl;

        prefixFragment << customDefines;

        float alphaTest = parameters.alphaTest;
        if (alphaTest) {
            prefixFragment << "#define ALPHATEST " + std::to_string(alphaTest);
            if (std::fmod(alphaTest, 1.0f) > 0)
                prefixFragment << "";
            else
                prefixFragment << ".0" << std::endl; // add ".0" if integer
        }
        prefixFragment << "#define GAMMA_FACTOR " << gammaFactorDefine << std::endl;

        if (parameters.useFog && parameters.fog)  prefixFragment << "#define USE_FOG" << std::endl;
        if (parameters.useFog && parameters.fogExp2)  prefixFragment << "#define FOG_EXP2" << std::endl;

        if (parameters.map)  prefixFragment << "#define USE_MAP" << std::endl;
        if (parameters.matcap)  prefixFragment << "#define USE_MATCAP" << std::endl;
        if (parameters.envMap)  prefixFragment << "#define USE_ENVMAP" << std::endl;
        if (parameters.envMap)  prefixFragment << "#define " + envMapTypeDefine << std::endl;
        if (parameters.envMap)  prefixFragment << "#define " + envMapModeDefine << std::endl;
        if (parameters.envMap)  prefixFragment << "#define " + envMapBlendingDefine << std::endl;
        if (parameters.lightMap)  prefixFragment << "#define USE_LIGHTMAP" << std::endl;
        if (parameters.aoMap)  prefixFragment << "#define USE_AOMAP" << std::endl;
        if (parameters.emissiveMap)  prefixFragment << "#define USE_EMISSIVEMAP" << std::endl;
        if (parameters.bumpMap)  prefixFragment << "#define USE_BUMPMAP" << std::endl;
        if (parameters.normalMap)  prefixFragment << "#define USE_NORMALMAP" << std::endl;
        if (parameters.normalMap && parameters.objectSpaceNormalMap)  prefixFragment << "#define OBJECTSPACE_NORMALMAP" << std::endl;
        if (parameters.normalMap && parameters.tangentSpaceNormalMap)  prefixFragment << "#define TANGENTSPACE_NORMALMAP" << std::endl;
        if (parameters.clearcoatMap)  prefixFragment << "#define USE_CLEARCOATMAP" << std::endl;
        if (parameters.clearcoatRoughnessMap)  prefixFragment << "#define USE_CLEARCOAT_ROUGHNESSMAP" << std::endl;
        if (parameters.clearcoatNormalMap)  prefixFragment << "#define USE_CLEARCOAT_NORMALMAP" << std::endl;
        if (parameters.specularMap)  prefixFragment << "#define USE_SPECULARMAP" << std::endl;
        if (parameters.roughnessMap)  prefixFragment << "#define USE_ROUGHNESSMAP" << std::endl;
        if (parameters.metalnessMap)  prefixFragment << "#define USE_METALNESSMAP" << std::endl;
        if (parameters.alphaMap)  prefixFragment << "#define USE_ALPHAMAP" << std::endl;

        if (parameters.sheen)  prefixFragment << "#define USE_SHEEN" << std::endl;
        if (parameters.transmissionMap) prefixFragment << "#define USE_TRANSMISSIONMAP" << std::endl;

        if (parameters.vertexTangents)  prefixFragment << "#define USE_TANGENT" << std::endl;
        if (parameters.vertexColors || parameters.instancingColor)  prefixFragment << "#define USE_COLOR" << std::endl;
        if (parameters.vertexUvs)  prefixFragment << "#define USE_UV" << std::endl;
        if (parameters.uvsVertexOnly)  prefixFragment << "#define UVS_VERTEX_ONLY" << std::endl;

        if (parameters.gradientMap)  prefixFragment << "#define USE_GRADIENTMAP" << std::endl;

        if (parameters.flatShading)  prefixFragment << "#define FLAT_SHADED" << std::endl;

        if (parameters.doubleSided)  prefixFragment << "#define DOUBLE_SIDED" << std::endl;
        if (parameters.flipSided)  prefixFragment << "#define FLIP_SIDED" << std::endl;

        if (parameters.shadowMapEnabled)  prefixFragment << "#define USE_SHADOWMAP" << std::endl;
        if (parameters.shadowMapEnabled)  prefixFragment << "#define " + shadowMapTypeDefine << std::endl;

        if (parameters.premultipliedAlpha)  prefixFragment << "#define PREMULTIPLIED_ALPHA" << std::endl;

        if (parameters.physicallyCorrectLights)  prefixFragment << "#define PHYSICALLY_CORRECT_LIGHTS" << std::endl;

        if (parameters.logarithmicDepthBuffer)  prefixFragment << "#define USE_LOGDEPTHBUF" << std::endl;
        if (parameters.logarithmicDepthBuffer) {
            if (parameters.rendererExtensionFragDepth)
                prefixFragment << "#define USE_LOGDEPTHBUF_EXT" << std::endl;
        }

        bool extensionShaderTextureLOD = parameters.extensionShaderTextureLOD;


        bool rendererExtensionShaderTextureLod = parameters.renderExtensionShaderTextureLOD;

        if ((extensionShaderTextureLOD || parameters.envMap) && rendererExtensionShaderTextureLod)
            prefixFragment << "#define TEXTURE_LOD_EXT" << std::endl;

        prefixFragment << "uniform mat4 viewMatrix;" << std::endl;
        prefixFragment << "uniform vec3 cameraPosition;" << std::endl;
        prefixFragment << "uniform bool isOrthographic;" << std::endl;

        if (parameters.toneMapping != ToneMapping::NoToneMapping)  prefixFragment << "#define TONE_MAPPING" << std::endl;
        if (parameters.toneMapping != ToneMapping::NoToneMapping) prefixFragment << getShaderChunk(ShaderLibID::tonemapping_pars_fragment) << std::endl; // this code is required here because it is used by the toneMapping() function defined below
        if (parameters.toneMapping != ToneMapping::NoToneMapping) prefixFragment << getToneMappingFunction("toneMapping", parameters.toneMapping) << std::endl;

        if (parameters.dithering)  prefixFragment << "#define DITHERING" << std::endl;

        if (parameters.outputEncoding!=TextureEncoding::Unknown ||
            parameters.mapEncoding != TextureEncoding::Unknown ||
            parameters.matcapEncoding != TextureEncoding::Unknown ||
            parameters.envMapEncoding != TextureEncoding::Unknown ||
            parameters.emissiveMapEncoding != TextureEncoding::Unknown ||
            parameters.lightMapEncoding != TextureEncoding::Unknown)
            prefixFragment << getShaderChunk(ShaderLibID::encodings_pars_fragment) << std::endl; // this code is required here because it is used by the various encoding/decoding function defined below
        if (parameters.mapEncoding!=TextureEncoding::Unknown) prefixFragment << getTexelDecodingFunction("mapTexelToLinear", parameters.mapEncoding) << std::endl;
        if (parameters.matcapEncoding != TextureEncoding::Unknown) prefixFragment << getTexelDecodingFunction("matcapTexelToLinear", parameters.matcapEncoding) << std::endl;
        if (parameters.envMapEncoding != TextureEncoding::Unknown) prefixFragment << getTexelDecodingFunction("envMapTexelToLinear", parameters.envMapEncoding) << std::endl;
        if (parameters.emissiveMapEncoding != TextureEncoding::Unknown) prefixFragment << getTexelDecodingFunction("emissiveMapTexelToLinear", parameters.emissiveMapEncoding) << std::endl;
        if (parameters.lightMapEncoding != TextureEncoding::Unknown) prefixFragment << getTexelDecodingFunction("lightMapTexelToLinear", parameters.lightMapEncoding) << std::endl;
        if (parameters.outputEncoding != TextureEncoding::Unknown) prefixFragment << getTexelEncodingFunction("linearToOutputTexel", parameters.outputEncoding) << std::endl;

        if (parameters.depthPacking!=DepthPackingStrategies::None)  prefixFragment << "#define DEPTH_PACKING " + std::to_string((int)parameters.depthPacking) << std::endl;
    }

    vShader = resolveIncludes(vShader);
    vShader = replaceLightsNums(vShader, parameters);
    vShader = replaceClippingPlaneNums(vShader, parameters);

    fShader = resolveIncludes(fShader);
    fShader = replaceLightsNums(fShader, parameters);
    fShader = replaceClippingPlaneNums(fShader, parameters);

    vShader = unrollLoops(vShader);
    fShader = unrollLoops(fShader);

    if (parameters.isGLES3 && parameters.isRawShaderMaterial) {
        bool isGLSL3ShaderMaterial = true;

        // GLSL 3.0 conversion

        std::stringstream prefixVertex3;

#ifdef OPENGL_ES_3_2
        prefixVertex3 << "#version 300 es" << std::endl
//							 << generatePrecision(parameters) << std::endl
#else
        prefixVertex3 << "#version 440" << std::endl
                      <<" precision highp float;" << std::endl
                      << "#define HIGH_PRECISION" << std::endl
#endif
//                      << "#define attribute in" << std::endl
//                      << "#define varying out" << std::endl
//                      << "#define texture2D texture" << std::endl
                      << prefixVertex.str() << std::endl;
        prefixVertex.swap(prefixVertex3);


        std::stringstream prefixFragment3;
        prefixFragment3
#ifdef OPENGL_ES_3_2
                << "#version 300 es" << std::endl
//					<< generatePrecision(parameters) << std::endl;
#else
                << "#version 440" << std::endl
                <<"precision highp float;"<< std::endl
                <<"#define HIGH_PRECISION"<< std::endl
                #endif
//                << "#define varying in" << std::endl
//                << "out highp vec4 pc_fragColor;" << std::endl
//                << "#define gl_FragColor pc_fragColor" << std::endl
//                << "#define gl_FragDepthEXT gl_FragDepth" << std::endl
//                << "#define texture2D texture" << std::endl
//                << "#define textureCube texture" << std::endl
//                << "#define texture2DProj textureProj" << std::endl
//                << "#define texture2DLodEXT textureLod" << std::endl
//                << "#define texture2DProjLodEXT textureProjLod" << std::endl
//                << "#define textureCubeLodEXT textureLod" << std::endl
//                << "#define texture2DGradEXT textureGrad" << std::endl
//                << "#define texture2DProjGradEXT textureProjGrad" << std::endl
//                << "#define textureCubeGradEXT textureGrad" << std::endl;
                    << prefixFragment.str() << std::endl;
        prefixFragment.swap(prefixFragment3);
    }

    prefixVertex << vShader << std::endl;
    prefixFragment << fShader << std::endl;

    std::string vertexGlsl = prefixVertex.str();
    std::string fragmentGlsl = prefixFragment.str();

    // for glsl Debug
    std::ofstream vshaderfile;
    vshaderfile.open(parameters.shaderName+"_vshader.txt", std::ios_base::out);
    vshaderfile << vertexGlsl;
    vshaderfile.close();

    std::ofstream fshaderfile;
    fshaderfile.open(parameters.shaderName+"_fshader.txt", std::ios_base::out);
    fshaderfile << fragmentGlsl;
    fshaderfile.close();

    GLShader vertexShader = GLShader(GL_VERTEX_SHADER, vertexGlsl);
    GLShader fragmentShader = GLShader(GL_FRAGMENT_SHADER, fragmentGlsl);

    glAttachShader(this->program, vertexShader.shader);
    glAttachShader(this->program, fragmentShader.shader);

    if (!parameters.indexOfAttributeName.empty()) {
        glBindAttribLocation(this->program, 0, parameters.indexOfAttributeName.c_str());
    }
    else if (parameters.morphTargets) {
        glBindAttribLocation(program, 0, "position");
    }

    glLinkProgram(this->program);
    bool checkShaderErrors = true;// renderer.debug.count("checkShaderErrors") > 0;
    //if (checkShaderErrors)
    //	checkShaderErrors = renderer.debug["checkShaderErrors"];
    if (checkShaderErrors) {
        char programLog[1024];
        glGetProgramInfoLog(program, 1024, 0, programLog);

        char vertexLog[1024];
        glGetShaderInfoLog(vertexShader.shader, 1024, 0, vertexLog);

        char fragmentLog[1024];
        glGetShaderInfoLog(fragmentShader.shader, 1024, 0, fragmentLog);

        int linkStatus;
        bool runnable = true;
        bool haveDiagnostics = true;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == 0) {
            runnable = false;
            std::string vertexError = getShaderErrors(vertexShader, "vertex");
            if (!vertexError.empty()) {
#ifdef _DEBUG
                std::ofstream outfile;
						outfile.open("vertexError.txt", std::ios_base::out);
						outfile << vertexError;
						outfile.close();
						//sendDebugMessage(std::wstring().assign(vertexError.begin(), vertexError.end()));
#endif
            }
            std::string fragmentErrors = getShaderErrors(fragmentShader, "fragment");
            if (!fragmentErrors.empty()) {
#ifdef _DEBUG
                std::ofstream outfile;
						outfile.open("fragmentError.txt", std::ios_base::out);
						outfile << fragmentErrors;
						outfile.close();
						//sendDebugMessage(std::wstring().assign(fragmentErrors.begin(), fragmentErrors.end()));
#endif
            }
            int validateState;
            glGetProgramiv(program, GL_VALIDATE_STATUS, &validateState);

            std::stringstream errorMessage;
            errorMessage << "THREE.Renderers.gl.GLProgram:Shader error " << glGetError() << " , VALIDATE_STATUS " << validateState << ", GL.GetProgramInfoLog " << programLog << std::endl << vertexLog << std::endl << fragmentLog;
#ifdef _DEBUG
            std::ofstream errorfile;
					errorfile.open("programError.txt", std::ios_base::out);
					errorfile << errorMessage.str();
					errorfile.close();
					//sendDebugMessage(std::wstring().assign(errorMessage.str().begin(), errorMessage.str().end()));
#endif
        }
        else if (!std::string().assign(programLog).empty()) {

        }
        else if (!std::string().assign(vertexLog).empty() || !std::string().assign(fragmentLog).empty()) {
            haveDiagnostics = false;
        }
        if (haveDiagnostics) {
            diagnostics.runnable= runnable;
            //diagnostics.material= material;
            diagnostics.programLog= programLog;
            //
            //
        }
    }

    glDeleteShader(vertexShader.shader);
    glDeleteShader(fragmentShader.shader);

}

std::vector<std::string> GLProgram::getEncodingComponents(TextureEncoding encoding){
    std::vector<std::string> values;
    switch (encoding) {
        case TextureEncoding::LinearEncoding :
            values.push_back("Linear");values.push_back("( value )");
            return values;
        case TextureEncoding::sRGBEncoding :
            values.push_back("sRGB");values.push_back("( value )");
            return values;
        case TextureEncoding::RGBEEncoding:
            values.push_back("RGBE");values.push_back("( value )");
            return values;
        case TextureEncoding::RGBM7Encoding:
            values.push_back("RGBM");values.push_back("( value )");
            return values;
        case TextureEncoding::RGBM16Encoding:
            values.push_back("RGBM");values.push_back("( value )");
            return values;
        case TextureEncoding::RGBDEncoding:
            values.push_back("RGBD");values.push_back("( value )");
            return values;
        case TextureEncoding::GammaEncoding:
            values.push_back("Gamma");values.push_back("( value )");
            return values;
        case TextureEncoding::LogLuvEncoding:
            values.push_back("LogLuv");values.push_back("( value )");
            return values;
        default :
            values.push_back("");values.push_back("");
            return values;
    }
}

std::string GLProgram::getShaderErrors(const GLShader& shader, const std::string& type){
    int status;
    glGetShaderiv(shader.shader, GL_COMPILE_STATUS, &status);

    GLchar infoLog[1024];
    glGetShaderInfoLog(shader.shader, 1024, NULL, infoLog);

    std::string log(infoLog);
    if (status == 1 && log.empty()) return log; // empty string

    int length;
    GLchar* source = new GLchar[4096000];

    glGetShaderSource(shader.shader, 4096000, &length, source);

    std::stringstream ss;

    ss<< "THREE.Renderers.gl.GLProgram: glGetShaderInfoLog() "<< type<< std::endl << log << std::endl <<addLineNumbers(std::string(source));

    delete source;

    return ss.str();
}

std::string GLProgram::getTexelDecodingFunction(const std::string& functionName, TextureEncoding encoding){
    std::stringstream ss;

    std::vector<std::string> components = getEncodingComponents(encoding);

    ss << "vec4 " << functionName << " ( vec4 value ) { return " << components[0] << "ToLinear"<< components[1] << ";}";

    return ss.str();
}

std::string GLProgram::getTexelEncodingFunction(const std::string& functionName, TextureEncoding encoding){
    std::stringstream ss;

    std::vector<std::string> components = getEncodingComponents(encoding);

    ss << "vec4 " << functionName << " ( vec4 value ) { return LinearTo" << components[0] << components[1] << ";}";

    return ss.str();
}

std::string GLProgram::getToneMappingFunction(const std::string& functionName, ToneMapping toneMapping){
    std::stringstream ss;
    std::string toneMappingName;
    switch (toneMapping) {

        case ToneMapping::LinearToneMapping:
            toneMappingName = "Linear";
            break;

        case ToneMapping::ReinhardToneMapping:
            toneMappingName = "Reinhard";
            break;

        case ToneMapping::Uncharted2ToneMapping:
            toneMappingName = "Uncharted2";
            break;

        case ToneMapping::CineonToneMapping:
            toneMappingName = "OptimizedCineon";
            break;

        case ToneMapping::ACESFilmicToneMapping:
            toneMappingName = "ACESFilmic";
            break;

        default:
            toneMappingName="";

    }

    ss << "vec3 " + functionName + "( vec3 color ) { return " << toneMappingName << "ToneMapping( color ); }";

    return ss.str();

}

std::string GLProgram::generateExtensions(const ProgramParameters& parameters){
    std::stringstream chunks;

    if (parameters.extensionDerivatives || parameters.envMapCubeUV || parameters.bumpMap || parameters.tangentSpaceNormalMap || parameters.clearcoatNormalMap || parameters.flatShading || parameters.shaderID=="physical")
        chunks <<  "#extension GL_OES_standard_derivatives : enable" << std::endl;

    if ((parameters.extensionFragDepth || parameters.logarithmicDepthBuffer) && parameters.rendererExtensionFragDepth)
        chunks << "#extension GL_EXT_frag_depth : enable"<< std::endl;

    if (parameters.extensionDrawBuffers && parameters.rendererExtensionDrawBuffers)
        chunks << "#extension GL_EXT_draw_buffers : require"<< std::endl;

    if ((parameters.shaderTextureLOD || parameters.envMap) && parameters.renderExtensionShaderTextureLOD)
        chunks << "#extension GL_EXT_shader_texture_lod : enable" << std::endl;

    return chunks.str();
}

std::string GLProgram::generateDefines(const std::unordered_map<std::string, std::string>& defines){
    if (defines.size() == 0) return {};
    std::stringstream ss;
    std::string strVal;
    float floatVal;
    int intVal;

    for (auto& entry : defines) {
        ss << "#define " << entry.first << " " << entry.second << std::endl;
    }

    return ss.str();
}

void GLProgram::fetchAttributeLocation(const int program, std::unordered_map<std::string, GLint>& attribute){
    int n;

    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &n);
    for (unsigned i = 0;i < n;i++) {
        GLsizei length;
        GLint size;
        GLenum type;
        GLchar name[101];

        glGetActiveAttrib(program, i, 100, &length, &size, &type,name);

        int location = glGetAttribLocation(program, name);

        std::string attributeName(name);

        attribute[attributeName] = location;
        //morphTarget
        //morphNormal
    }
}

std::string& GLProgram::replaceLightsNums(std::string& str, const ProgramParameters& parameters){
    using string_utils::replace_all;

    str = replace_all(str, "NUM_DIR_LIGHTS", std::to_string(parameters.numDirLights));
    str = replace_all(str, "NUM_SPOT_LIGHTS", std::to_string(parameters.numSpotLights));
    str = replace_all(str, "NUM_RECT_AREA_LIGHTS", std::to_string(parameters.numRectAreaLights));
    str = replace_all(str, "NUM_POINT_LIGHTS", std::to_string(parameters.numPointLights));
    str = replace_all(str, "NUM_HEMI_LIGHTS", std::to_string(parameters.numHemiLights));
    str = replace_all(str, "NUM_DIR_LIGHT_SHADOWS", std::to_string(parameters.numDirLightShadows));
    str = replace_all(str, "NUM_SPOT_LIGHT_SHADOWS", std::to_string(parameters.numSpotLightShadows));
    str = replace_all(str, "NUM_POINT_LIGHT_SHADOWS", std::to_string(parameters.numPointLightShadows));

    return str;
}

std::string GLProgram::replaceClippingPlaneNums(std::string& str, const ProgramParameters& parameters){
    using string_utils::replace_all;

    int numClipingPlanes = parameters.numClippingPlanes;
    int numClipIntersection = parameters.numClippingPlanes;
    str = replace_all(str, "NUM_CLIPPING_PLANES", std::to_string(numClipingPlanes));
    str = replace_all(str, "UNION_CLIPPING_PLANES", std::to_string(numClipingPlanes - numClipingPlanes));

    return str;
}

static const std::string includePattern = R"([ \t]*#include +<([\w\d./]+)>)";
//"[ \\t] * #include + <([\\w\\d./]+)>";

std::string GLProgram::resolveIncludes(std::string& source){
    std::regex regx(includePattern);

    std::string localSource = source;
    auto words_begin = std::sregex_iterator(source.begin(), source.end(), regx);
    auto words_end = std::sregex_iterator();
    if (words_begin != words_end) {
        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            std::string result = includeReplacer(match);
            localSource = string_utils::replace_all(localSource, match[0], result);
        }
    }
    return localSource;
}
std::string GLProgram::includeReplacer(std::smatch& match){
    if (match.size() > 1) {
        std::stringstream localstream;
        for (unsigned ctr = 1; ctr < match.size(); ctr++) {
            std::string include = match[ctr];
            std::string source = getShaderChunk(shaderlib_name::get(include));
            if (source.empty() || source.size() == 1) continue;
            std::string result = resolveIncludes(source);
            source = string_utils::replace_all(source, match[0], result);
            localstream << source;
        }

        return localstream.str();
    }
    else {
        return std::string();
    }
}
static const std::string loopPattern = R"(#pragma unroll_loop_start[\s]+?for \( int i \= (\d+)\; i < (\d+)\; i \+\+ \) \{([\s\S]+?)(?=\})\}[\s]+?#pragma unroll_loop_end)";
//"(#pragma unroll_loop[\\s]+?for \\( int i \\= (\\d+)\\; i < (\\d+)\\; i \\+\\+ \\) \\{([\\s\\S]+?)(?=\\})\\}";

std::string GLProgram::unrollLoops(const std::string& source){
    using std::string;
    using std::smatch;
    using string_utils::replace_all;

    std::regex regx(loopPattern);

    std::string localSource = source;
    auto words_begin = std::sregex_iterator(source.begin(), source.end(), regx);
    auto words_end = std::sregex_iterator();
    if (words_begin != words_end) {
        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            smatch match = *i;
            string result = loopReplacer(match);
            localSource = replace_all(localSource, match[0], result);
        }
    }
    return localSource;
}

std::string GLProgram::resolveIncludeUX(const std::string& source){
    using std::string;
    using std::sregex_iterator;

    static const std::regex rex(includePattern);

    string result;

    sregex_iterator rex_it(source.begin(), source.end(), rex);
    sregex_iterator rex_end;
    size_t pos = 0;

    while (rex_it != rex_end) {
        std::smatch match = *rex_it;
        result.append(source, pos, match.position(0) - pos);
        pos = match.position(0) + match.length(0);

        std::ssub_match sub = match[1];
        string r = getShaderChunk(shaderlib_name::get(sub.str()));
        if (r.empty()) {
            std::stringstream ss;
            ss << "unable to resolve #include <" << sub.str() << ">";
            throw std::logic_error(ss.str());
        }
        result.append(r);
        rex_it++;
    }
    if (pos == 0) return source;
    else {
        result.append(source, pos, source.length());
        return result;
    }
}

std::string GLProgram::unrollLoopsUX(const std::string& source){
    using std::regex;
    using std::sregex_iterator;
    using std::smatch;
    using std::ssub_match;

    static const regex rex(R"(for \( int i = (\d+)\; i < (\d+); i \+\+ \) \{[\r\n]?([\s\S]+?)(?=\})\})");
    static const regex rex2(R"(\[ i \])");

    std::stringstream unroll;
    sregex_iterator rex_it(source.begin(), source.end(), rex);
    sregex_iterator rex_end;

    auto it_start = source.begin();

    while (rex_it != rex_end) {
        smatch match = *rex_it;

        for (auto it_end = source.begin() + match.position(); it_start != it_end; it_start++) unroll << *it_start;

        int start = atoi(match[1].str().c_str());
        int end = atoi(match[2].str().c_str());
        ssub_match snippet = match[3];

        for (int i = start; i < end; i++) {
            std::stringstream ss2;
            ss2 << "[ " << i << " ]";
            unroll << regex_replace(snippet.str(), rex2, ss2.str());
        }
        rex_it++;
        it_start = source.begin() + match.position() + match.length();
    }
    for (; it_start < source.end(); it_start++) unroll << *it_start;

    return unroll.str();
}

std::string GLProgram::loopReplacer(std::smatch& match){
    using std::string;
    using string_utils::replace_all;

    std::stringstream ss;

    if (match.size() > 1) {
        int start = std::stoi(match[1]);
        int end = std::stoi(match[2]);
        string snippet = match[3];
        for (int i = start;i < end;i++) {
            string snippet1 = replace_all(snippet,"[ i ]", "[ " + std::to_string(i) + " ]");
            snippet1 = replace_all(snippet1, "UNROLLED_LOOP_INDEX", std::to_string(i));

            ss << snippet1 << std::endl;
        }
    }

    return ss.str();
}

std::string GLProgram::generatePrecision(const ProgramParameters& parameter){
    std::stringstream ss;
    ss << "precision " << parameter.precision << " float; " << std::endl;
    ss << "precision " + parameter.precision + " int; "<< std::endl;

    if (parameter.precision == "highp")
    {
        ss << "#define HIGH_PRECISION" << std::endl;
    }
    else if (parameter.precision=="mediump")
    {
        ss <<"#define MEDIUM_PRECISION"<< std::endl;
    }
    else if (parameter.precision =="lowp")
    {
        ss<<"#define LOW_PRECISION"<< std::endl;
    }

    return ss.str();
}

std::string GLProgram::generateShadowMapTypeDefine(const ProgramParameters& parameters){
    if(parameters.shadowMapType==ShadowMapType::PCFShadowMap)
        return "SHADOWMAP_TYPE_PCF";
    else if(parameters.shadowMapType ==ShadowMapType::PCFSoftShadowMap)
        return "SHADOWMAP_TYPE_PCF_SOFT";
    else if(parameters.shadowMapType == ShadowMapType::VSMShadowMap)
        return "SHADOWMAP_TYPE_VSM";
    else
        return "SHADOWMAP_TYPE_BASIC";
}

std::string GLProgram::generateEnvMapTypeDefine(const ProgramParameters& parameters){
    std::string envMapTypeDefine = "ENVMAP_TYPE_CUBE";
    if (parameters.envMap)
    {
        TextureMapping envMapMode = parameters.envMapMode;
        switch (envMapMode)
        {

            case TextureMapping::CubeReflectionMapping: //Constants.CubeReflectionMapping:
            case TextureMapping::CubeRefractionMapping:
                envMapTypeDefine = "ENVMAP_TYPE_CUBE";
                break;

            case TextureMapping::CubeUVReflectionMapping:
            case TextureMapping::CubeUVRefractionMapping:
                envMapTypeDefine = "ENVMAP_TYPE_CUBE_UV";
                break;

            case TextureMapping::EquirectangularReflectionMapping:
            case TextureMapping::EquirectangularRefractionMapping:
                envMapTypeDefine = "ENVMAP_TYPE_EQUIREC";
                break;

            case TextureMapping::SphericalReflectionMapping:
                envMapTypeDefine = "ENVMAP_TYPE_SPHERE";
                break;

        }

    }

    return envMapTypeDefine;
}

std::string GLProgram::generateEnvMapModeDefine(const ProgramParameters& parameters){
    if (parameters.envMap) {
        switch (parameters.envMapMode) {

            case TextureMapping::CubeRefractionMapping:
            case TextureMapping::EquirectangularRefractionMapping:
                return "ENVMAP_MODE_REFRACTION";
            default :
                return "ENVMAP_MODE_REFLECTION";

        }

    }

    return "ENVMAP_MODE_REFLECTION";
}

std::string GLProgram::generateEnvMapBlendingDefine(const ProgramParameters& parameters){
    //var envMapBlendingDefine = "ENVMAP_BLENDING_NONE";
    if (parameters.envMap) {
        //int combine = (int)parameters["combine"];
        switch (parameters.combine) {

            case Combine::MultiplyOperation:
                return "ENVMAP_BLENDING_MULTIPLY";
                break;

            case Combine::MixOperation:
                return "ENVMAP_BLENDING_MIX";
                break;

            case Combine::AddOperation:
                return  "ENVMAP_BLENDING_ADD";
                break;
            default :
                return "ENVMAP_BLENDING_NONE";

        }

    }
    return "ENVMAP_BLENDING_NONE";
}



GLUniforms::sptr GLProgram::getUniforms(){
    if (cachedUniforms == nullptr) {
        cachedUniforms = GLUniforms::create(renderer.textures,program);
    }
    return cachedUniforms;
}

std::unordered_map<std::string, GLint>& GLProgram::getAttributes(){
    if (cachedAttributes.size() == 0) {
        fetchAttributeLocation(program, cachedAttributes);
    }
    return cachedAttributes;
}

std::string GLProgram::addLineNumbers(const std::string& code){
    std::vector<std::string> lines = string_utils::split(code, '\n');
    std::stringstream ss;
    for (unsigned i = 0;i < lines.size();i++) {

        ss << (i + 1) << ":" << lines[i]<< std::endl;
        lines[i] = ss.str();
    }
    return ss.str();
}
