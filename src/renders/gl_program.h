//
// Created by SeanR on 2022/11/24.
//

#ifndef THREE_CPP_SRC_RENDERS_GL_PROGRAM_H
#define THREE_CPP_SRC_RENDERS_GL_PROGRAM_H

#include <GLES3/gl3.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>
#include <string>
#include <algorithm>
#include <map>
#include <regex>
#include <iostream>
#include <tuple>
#include <cmath>

#include "constants.h"
#include "common_types.h"
#include "../utils/string_utils.h"
//#include "../utils/string_format.h"
#include "shaders/shader_chunk.h"
#include "gl_renderer.h"
#include "opengl/gl_uniforms.h"

/**
 * all kinds of shader parameters
 */
struct GLProgramParameters{
    bool isGLES3 = true; //OpenGLES3.0

    bool instancing = false;
    bool instancingColor = false;
    bool supportsVertexTextures = false;
    bool useFog=false,fog=false,fogExp2=false;
    bool map = false;
    bool matcap=false;
    bool envMap = false;
    double envMapCubeUVHeight;
    bool lightMap = false;
    bool aoMap = false;
    bool emissiveMap = false;
    bool bumpMap=false;
    bool normalMap=false,objectSpaceNormalMap=false,tangentSpaceNormalMap=false;
    bool clearcoat=false,clearcoatMap=false,clearcoatRoughnessMap=false,clearcoatNormalMap=false;

    bool iridescence=false,iridescenceMap=false,iridescenceThicknessMap=false;
    bool specularMap,specularIntensityMap,specularColorMap;
    bool roughnessMap,metalnessMap;

    bool alphaMap,alphaTest;
    bool sheen,sheenColorMap,sheenRoughnessMap;
    bool transmission,transmissionMap;
    bool thicknessMap;
    bool decodeVideoTexturev;
    bool vertexTangents,vertexColors,vertexAlphas,vertexUvs,uvsVertexOnly;
    bool gradientMap,displacementMap;
    bool flatShading;
    bool doubleSided,flipSided;
    bool shadowMapEnabled;
    bool premultipliedAlpha;
    bool physicallyCorrectLights;
    bool logarithmicDepthBuffer;
    bool rendererExtensionFragDepth;

    int toneMapping;

    bool dithering;
    bool opaque;
    bool useDepthPacking;
    bool skinning;

    bool extensionDerivatives,extensionFragDepth,extensionDrawBuffers,extensionShaderTextureLOD;
    bool rendererExtensionDrawBuffers,rendererExtensionShaderTextureLod;
    bool isRawShaderMaterial;

    std::string glslVersion;
    std::string shaderID;
    std::string precision;
    std::string vertexShader,fragmentShader;
    std::string shaderName;

    int numSpotLightShadows,numSpotLightMaps,numSpotLightShadowsWithMaps,numSpotLights;

    int numDirLights,numDirLightShadows;
    int numRectAreaLights,numHemiLights;
    int numPointLightShadows,numPointLights;

    int numClippingPlanes,numClipIntersection;

    int shadowMapType;
    int envMapMode;
    int combine;
    int morphTargetsCount,morphTextureStride;

    bool morphTargets,morphNormals,morphColors;
    bool sizeAttenuation,decodeVideoTexture;

    int outputEncoding;
    int depthPacking;


    std::map<std::string,std::string> defines;
    std::string index0AttributeName;

};

int programIdCount = 0;

//string formatter reference:
//https://stackoverflow.com/questions/63121776/simplest-syntax-for-string-interpolation-in-c
std::string handleSource(std::string str,int errorLine){
    std::vector<std::string> lines = string_utils::split(str,"\n");
    std::vector<std::string> lines2;

    int from = std::min<int>(errorLine-6,0);
    int to = std::max<int>(errorLine+6,lines.size());

    for(int i = from;i < to;i++){
        int lineNo = i + 1;
        //auto fmtLineStr = string_format::format("{0}{1}: {2}",lineNo==errorLine?">":" ",lineNo,lines[i]);
        auto fmtLineStr = fmt::format("{0}{1}: {2}",lineNo==errorLine?">":" ",lineNo,lines[i]);
        lines2.push_back(fmtLineStr);
    }
    return string_utils::join(lines2);
}

std::vector<std::string> getEncodingComponents(int encoding){
    switch (encoding) {
        case LinearEncoding:
            return std::vector<std::string>{"Linear","( value )"};
        case sRGBEncoding:
            return std::vector<std::string>{"sRGB","( value )"};
        default:
            spdlog::warn("threecpp.GLProgram:Unsupported encoding:{0}",encoding);
            return std::vector<std::string>{"Linear","( value )"};
    }
}

std::string getTexelEncodingFunction( std::string functionName, int encoding ) {
    std::vector<std::string> components = getEncodingComponents( encoding );
    return "vec4 " + functionName + "( vec4 value ) { return LinearTo" + components[ 0 ] + components[ 1 ] + "; }";
}

std::string getShaderErrors(GLint shader,std::string type){
    std::string errMsgStr,shaderSrc;
    GLint compiled;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);
    if(!compiled){
        // Retrieve the compiler messages when compilation fails
        GLint infoLen = 0;
        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
        if ( infoLen > 1 ){
            char* infoLog = (char*)malloc ( sizeof ( char ) * infoLen );
            glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
            //esLogMessage(“Error compiling shader:\n%s\n”, infoLog);
            errMsgStr = std::string(infoLog);
            free ( infoLog );
        }

        if(errMsgStr == "") return "";

        GLint len = 0;
        glGetShaderiv( shader, GL_SHADER_SOURCE_LENGTH, &len );
        std::string source( static_cast<size_t>( len + 1 ), '\0' );
        glGetShaderSource( shader, len+1, nullptr, &source[0] );
        std::regex pattern("ERROR: 0:(\\d+)");
        std::smatch errorMatches;
        if(std::regex_search(errMsgStr,errorMatches,pattern)){
            std::string errorLine = errorMatches.str();
            for (auto &c : type) // for every char in type (note: c is a reference)
                c = std::toupper(c); // c is a reference, so the assignment changes the char in type
            return type + "\n\n" + errMsgStr + "\n\n" + handleSource(source,std::stoul(errorLine));
        }
    }

    return "";
}

std::string getToneMappingFunction( std::string functionName, const int toneMapping ) {
    std::string toneMappingName;

    switch ( toneMapping ) {

        case LinearToneMapping:
            toneMappingName = "Linear";
            break;

        case ReinhardToneMapping:
            toneMappingName = "Reinhard";
            break;

        case CineonToneMapping:
            toneMappingName = "OptimizedCineon";
            break;

        case ACESFilmicToneMapping:
            toneMappingName = "ACESFilmic";
            break;

        case CustomToneMapping:
            toneMappingName = "Custom";
            break;

        default:
            spdlog::warn( "threecpp.GLProgram: Unsupported toneMapping:{}", toneMapping );
            toneMappingName = "Linear";

    }

    return "vec3 " + functionName + "( vec3 color ) { return " + toneMappingName + "ToneMapping( color ); }";

}

bool filterEmptyLine( std::string str ) {
    return str != "";
}

std::string generateExtensions( const GLProgramParameters& parameters ) {
    using std::vector;
    using std::string;
    using std::copy_if;
    vector<string> chunks = {
            (parameters.extensionDerivatives || !!parameters.envMapCubeUVHeight || parameters.bumpMap ||
             parameters.tangentSpaceNormalMap || parameters.clearcoatNormalMap || parameters.flatShading ||
             parameters.shaderID == "physical") ? "#extension GL_OES_standard_derivatives : enable" : "",
            (parameters.extensionFragDepth || parameters.logarithmicDepthBuffer) &&
            parameters.rendererExtensionFragDepth ? "#extension GL_EXT_frag_depth : enable" : "",
            (parameters.extensionDrawBuffers && parameters.rendererExtensionDrawBuffers)
            ? "#extension GL_EXT_draw_buffers : require" : "",
            (parameters.extensionShaderTextureLOD || parameters.envMap || parameters.transmission) &&
            parameters.rendererExtensionShaderTextureLod ? "#extension GL_EXT_shader_texture_lod : enable" : ""
    };

    vector<string> chunksFiltered{};
    copy_if(chunks.begin(),chunks.end(),chunksFiltered.begin(),filterEmptyLine);
    return string_utils::join(chunksFiltered,"\n");
}

std::string generateDefines( std::map<std::string,std::string> defines ) {
    using std::vector;
    using std::string;
    vector<string> chunks{};

    for ( auto& m : defines ) {
        string value = m.second;

        if ( value == "" ) continue;

        chunks.push_back( "#define " + m.first + " " + value );
    }

    return string_utils::join( chunks,"\n" );
}

//https://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade
std::shared_ptr<std::map<std::string,threecpp::GLVertexAttribute>> fetchAttributeLocations( GLint program ) {
    std::map<std::string,threecpp::GLVertexAttribute> attributes{};

    GLint maxAttribNameLength = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);
    std::vector<GLchar> nameData(maxAttribNameLength);
    GLint numActiveAttribs = 0;
    glGetProgramiv(program,GL_ACTIVE_ATTRIBUTES,&numActiveAttribs);
    for(int attrib = 0; attrib < numActiveAttribs; ++attrib){
        GLint arraySize = 0;
        GLenum type = 0;
        GLsizei actualLength = 0;
        glGetActiveAttrib(program, attrib, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);
        std::string name((char*)&nameData[0], actualLength - 1);

        GLint  attributeLocation = 0;
        glGetAttribLocation(program,(char*)&nameData[0]);
        GLint  locationSize = 1;
        switch ( type ) {
            case GL_FLOAT_MAT2:
                locationSize = 2;
                break;
            case GL_FLOAT_MAT3:
                locationSize = 3;
                break;
            case GL_FLOAT_MAT4:
                locationSize = 4;
                break;
            default:
                break;
        }

        attributes[name] = {type:type,location:attributeLocation,locationSize:locationSize};
    }

    return std::shared_ptr<std::map<std::string,threecpp::GLVertexAttribute>>(&attributes);
}

//string replace function reference:
//https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
std::string replaceLightNums( std::string string, GLProgramParameters parameters ) {
    using std::to_string;

    int numSpotLightCoords = parameters.numSpotLightShadows + parameters.numSpotLightMaps - parameters.numSpotLightShadowsWithMaps;

    string = std::regex_replace(string, std::regex("/NUM_DIR_LIGHTS/g"), to_string(parameters.numDirLights));
    string = std::regex_replace(string, std::regex("/NUM_SPOT_LIGHTS/g"), to_string(parameters.numSpotLights));
    string = std::regex_replace(string, std::regex("/NUM_SPOT_LIGHT_MAPS/g"), to_string(parameters.numSpotLightMaps));
    string = std::regex_replace(string, std::regex("/NUM_SPOT_LIGHT_COORDS/g"), to_string(numSpotLightCoords));
    string = std::regex_replace(string, std::regex("/NUM_RECT_AREA_LIGHTS/g"), to_string(parameters.numRectAreaLights));
    string = std::regex_replace(string, std::regex("/NUM_POINT_LIGHTS/g"), to_string(parameters.numPointLights));
    string = std::regex_replace(string, std::regex("/NUM_HEMI_LIGHTS/g"), to_string(parameters.numHemiLights));
    string = std::regex_replace(string, std::regex("/NUM_DIR_LIGHT_SHADOWS/g"), to_string(parameters.numDirLightShadows));
    string = std::regex_replace(string, std::regex("/NUM_SPOT_LIGHT_SHADOWS_WITH_MAPS/g"), to_string(parameters.numSpotLightShadowsWithMaps));
    string = std::regex_replace(string, std::regex("/NUM_SPOT_LIGHT_SHADOWS/g"), to_string(parameters.numSpotLightShadows));
    string = std::regex_replace(string, std::regex("/NUM_POINT_LIGHT_SHADOWS/g"), to_string(parameters.numPointLightShadows));

    return string;
}

std::string  replaceClippingPlaneNums( std::string string, GLProgramParameters parameters ) {
    using std::to_string;

    string = std::regex_replace(string, std::regex("/NUM_CLIPPING_PLANES/g"), to_string(parameters.numClippingPlanes));
    string = std::regex_replace(string, std::regex("/UNION_CLIPPING_PLANES/g"), to_string(parameters.numClippingPlanes - parameters.numClipIntersection));

    return string;
}

std::string resolveIncludes( std::string shaderStr ) {
    using std::regex;
    using std::sregex_iterator;
    using std::string;
    using std::smatch;
    using std::regex_search;

    regex pattern("^[ \\t]*#include +<([\\w\\d./]+)>");
//    smatch result_match;
//    if(std::regex_search(text,result_match,pattern))
//        std::cout << result_match.str(0) << " || " << result_match.str(1) << endl;
//    else
//        std::cout << "not found" << std::endl;
//    //std::cout << '\n' << regex_replace(text, pattern, "[$&]") << '\n';

    string replacedStr=shaderStr;
    for (sregex_iterator it(shaderStr.begin(), shaderStr.end(), pattern), end_it;
         it != end_it; ++it){
        smatch m = *it;
        if(m[1].matched){
            //std::cout << m.position() << " || " << m.str(1) << std::endl;
            spdlog::debug("{0} || {1}",m.position(),m.str(1));
            string::size_type pos{};
            pos = replacedStr.find(m.str(),0);
            if(pos!=replacedStr.npos){
                replacedStr.replace(pos,m.str().size(),ShaderChunk[m.str(1)]);
            }
        }else{
            //std::cout << "not found" << std::endl;
            spdlog::warn("not found");
        }

    }
    //std::cout << replacedStr << std::endl;
    spdlog::debug("combined shader string ===== {}",replacedStr);

    return replacedStr;
}

// Unroll Loops
std::string unrollLoops( std::string shaderStr ) {
    using std::regex;
    using std::sregex_iterator;
    using std::regex_replace;
    using std::smatch;
    using std::string;

    regex pattern("#pragma unroll_loop_start\\s+for\\s*\\(\\s*int\\s+i\\s*=\\s*(\\d+)\\s*;\\s*i\\s*<\\s*(\\d+)\\s*;\\s*i\\s*\\+\\+\\s*\\)\\s*{([\\s\\S]+?)}\\s+#pragma unroll_loop_end");
    string replacedStr = shaderStr;
    for (sregex_iterator it(shaderStr.begin(), shaderStr.end(), pattern), end_it;
         it != end_it; ++it){
        smatch m = *it;
        if(m[1].matched){
            spdlog::debug("{0} || {1}",m.position(),m.str(1));
            string::size_type pos{};
            pos = replacedStr.find(m.str(),0);
            if(pos!=replacedStr.npos){
                replacedStr.replace(pos,m.str().size(),ShaderChunk[m.str(1)]);
            }
        }else{
            spdlog::warn("not found");
        }

    }
    //return string.replace( unrollLoopPattern, loopReplacer );
    return replacedStr;
}

//function loopReplacer( match, start, end, snippet ) {
//
//    let string = '';
//
//    for ( let i = parseInt( start ); i < parseInt( end ); i ++ ) {
//
//        string += snippet
//                          .replace( /\[\s*i\s*\]/g, '[ ' + i + ' ]' )
//        .replace( /UNROLLED_LOOP_INDEX/g, i );
//    }
//
//    return string;
//}

std::string generatePrecision( const GLProgramParameters& parameters ) {
    using std::string;

    string precisionstring = "precision " + parameters.precision + " float;\\nprecision"  + parameters.precision + " int;";

    if ( parameters.precision == "highp" ) {
        precisionstring += "\\n#define HIGH_PRECISION";
    } else if ( parameters.precision == "mediump" ) {
        precisionstring += "\\n#define MEDIUM_PRECISION";

    } else if ( parameters.precision == "lowp" ) {
        precisionstring += "\\n#define LOW_PRECISION";
    }

    return precisionstring;
}

std::string generateShadowMapTypeDefine( const GLProgramParameters& parameters ) {
    using std::string;
    string shadowMapTypeDefine = "SHADOWMAP_TYPE_BASIC";

    if ( parameters.shadowMapType == PCFShadowMap ) {
        shadowMapTypeDefine = "SHADOWMAP_TYPE_PCF";
    } else if ( parameters.shadowMapType == PCFSoftShadowMap ) {
        shadowMapTypeDefine = "SHADOWMAP_TYPE_PCF_SOFT";
    } else if ( parameters.shadowMapType == VSMShadowMap ) {
        shadowMapTypeDefine = "SHADOWMAP_TYPE_VSM";
    }

    return shadowMapTypeDefine;
}

std::string generateEnvMapTypeDefine( const GLProgramParameters& parameters ) {
    using std::string;
    string envMapTypeDefine = "ENVMAP_TYPE_CUBE";

    if ( parameters.envMap ) {

        switch ( parameters.envMapMode ) {
            case CubeReflectionMapping:
            case CubeRefractionMapping:
                envMapTypeDefine = 'ENVMAP_TYPE_CUBE';
                break;
            case CubeUVReflectionMapping:
                envMapTypeDefine = 'ENVMAP_TYPE_CUBE_UV';
                break;
        }

    }

    return envMapTypeDefine;
}

std::string generateEnvMapModeDefine( const GLProgramParameters& parameters ) {
    using std::string;
    string envMapModeDefine = "ENVMAP_MODE_REFLECTION";

    if ( parameters.envMap ) {
        switch ( parameters.envMapMode ) {
            case CubeRefractionMapping:
                envMapModeDefine = "ENVMAP_MODE_REFRACTION";
                break;
        }
    }

    return envMapModeDefine;
}


std::string generateEnvMapBlendingDefine( const GLProgramParameters& parameters ) {
    using std::string;
    string envMapBlendingDefine = "ENVMAP_BLENDING_NONE";

    if ( parameters.envMap ) {

        switch ( parameters.combine ) {
            case MultiplyOperation:
                envMapBlendingDefine = "ENVMAP_BLENDING_MULTIPLY";
                break;
            case MixOperation:
                envMapBlendingDefine = "ENVMAP_BLENDING_MIX";
                break;
            case AddOperation:
                envMapBlendingDefine = "ENVMAP_BLENDING_ADD";
                break;
        }

    }

    return envMapBlendingDefine;
}

std::tuple<float,float,float> generateCubeUVSize( const GLProgramParameters& parameters ) {
    using std::tuple;
    double imageHeight = parameters.envMapCubeUVHeight;

    if ( imageHeight == 0 ) return tuple<double,double,double>{0,0,0};

    double maxMip = std::log2( imageHeight ) - 2;
    double texelHeight = 1.0 / imageHeight;
    double texelWidth = 1.0 / ( 3 * std::max<double>( std::pow( 2, maxMip ), 7 * 16 ) );

    return { texelWidth, texelHeight, maxMip };
}


class GLProgram{
public:
    GLProgram()=delete;
    GLProgram(const GLRenderer& renderer,const std::string& cacheKey,const GLProgramParameters& parameters):cacheKey(cacheKey){};
    ~GLProgram(){
        glDeleteProgram(program);
    }

private:
    void init(const GLProgramParameters& parameters){
        auto shaderSourcePair = generateShaderGLSL(parameters);
        program = glCreateProgram();
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        auto vertexShaderSrc = shaderSourcePair.first;
        if(!compileShader(vertexShader,vertexShaderSrc))
            spdlog::error("vertex shader can't be compiled, shader source: \\n {}",vertexShaderSrc);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        auto fragmentShaderSrc = shaderSourcePair.second;
        if(!compileShader(fragmentShader,fragmentShaderSrc))
            spdlog::error("fragment shader can't be compiled, shader source: \\n {}",fragmentShaderSrc);

        glAttachShader(program,vertexShader);
        glAttachShader(program,fragmentShader);

        // Force a particular attribute to index 0.
        if ( parameters.index0AttributeName != "" ) {
            glBindAttribLocation( program, 0, parameters.index0AttributeName.c_str() );
        } else if ( parameters.morphTargets == true ) {
            // programs with morphTargets displace position out of attribute 0
            glBindAttribLocation( program, 0, "position" );
        }

        glLinkProgram( program );

    }

    bool compileShader(GLuint shaderHandle,std::string shaderSource){
        return compileShader(shaderHandle,shaderSource.c_str(),shaderSource.size());
    }
    //basic shader compiling method
    //note here，debug mehtod and non-debug method，also note memory handling
    bool compileShader(GLuint shaderHandle,const GLchar *source, const int32_t iSize) {
        if (source == NULL || iSize <= 0) return false;

        glShaderSource(shaderHandle, 1, &source, &iSize);  // Not specifying 3rd parameter
        // (size) could be troublesome..

        glCompileShader(shaderHandle);

#if defined(DEBUG)
        GLint logLength;
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetShaderInfoLog(shaderHandle, logLength, &logLength, log);
            printf("Shader compile log:\n%s", log);
            free(log);
        }
#endif

        GLint status;
        glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);
        if (status == 0) {
            glDeleteShader(shaderHandle);
            return false;
        }

        return true;
    }

    std::pair<std::string,std::string> generateShaderGLSL(const GLProgramParameters& parameters){
        using std::map,std::string,std::vector;
        using std::tuple;

        map<string,string> defines = parameters.defines;

        string vertexShader = parameters.vertexShader;
        string fragmentShader = parameters.fragmentShader;

        string shadowMapTypeDefine = generateShadowMapTypeDefine( parameters );
        string envMapTypeDefine = generateEnvMapTypeDefine( parameters );
        string envMapModeDefine = generateEnvMapModeDefine( parameters );
        string envMapBlendingDefine = generateEnvMapBlendingDefine( parameters );
        tuple<double,double,double> envMapCubeUVSize = generateCubeUVSize( parameters );

        string customExtensions = parameters.isGLES3 ? "" : generateExtensions( parameters );

        string customDefines = generateDefines( defines );

        //const program = gl.createProgram();

        string prefixVertex, prefixFragment;
        string versionString = parameters.glslVersion!="" ? "#version " + parameters.glslVersion + "\\n" : "";

        if ( parameters.isRawShaderMaterial ) {

            //prefixVertex = [customDefines].filter( filterEmptyLine ).join( '\n' );
            prefixVertex = customDefines;

            if ( prefixVertex.size() > 0 ) {
                prefixVertex += "\n";
            }

//            prefixFragment = [
//
//                    customExtensions,
//                    customDefines
//
//            ].filter( filterEmptyLine ).join( '\n' );
            if(customDefines!="" && customExtensions!="")
                prefixFragment = customExtensions + "\\n" + customDefines;

            if ( prefixFragment.size() > 0 ) {
                prefixFragment += "\\n";
            }

        } else {
            vector<string> prefixVertexVec{
                        generatePrecision( parameters ),
                        "#define SHADER_NAME " + parameters.shaderName,
                        customDefines,
                        parameters.instancing ? "#define USE_INSTANCING" : "",
                        parameters.instancingColor ? "#define USE_INSTANCING_COLOR" : "",
                        parameters.supportsVertexTextures ? "#define VERTEX_TEXTURES" : "",
                        ( parameters.useFog && parameters.fog ) ? "#define USE_FOG" : "",
                        ( parameters.useFog && parameters.fogExp2 ) ? "#define FOG_EXP2" : "",
                        parameters.map ? "#define USE_MAP" : "",
                        parameters.envMap ? "#define " + envMapModeDefine : "",
                        parameters.lightMap ? "#define USE_LIGHTMAP" : "",
                        parameters.aoMap ? "#define USE_AOMAP" : "",
                        parameters.emissiveMap ? "#define USE_EMISSIVEMAP" : "",
                        parameters.bumpMap ? "#define USE_BUMPMAP" : "",
                        parameters.normalMap ? "#define USE_NORMALMAP" : "",
                        ( parameters.normalMap && parameters.objectSpaceNormalMap ) ? "#define OBJECTSPACE_NORMALMAP" : "",
                        ( parameters.normalMap && parameters.tangentSpaceNormalMap ) ? "#define TANGENTSPACE_NORMALMAP" : "",

                        parameters.clearcoatMap ? "#define USE_CLEARCOATMAP" : "",
                        parameters.clearcoatRoughnessMap ? "#define USE_CLEARCOAT_ROUGHNESSMAP" : "",
                        parameters.clearcoatNormalMap ? "#define USE_CLEARCOAT_NORMALMAP" : "",

                        parameters.iridescenceMap ? "#define USE_IRIDESCENCEMAP" : "",
                        parameters.iridescenceThicknessMap ? "#define USE_IRIDESCENCE_THICKNESSMAP" : "",

                        parameters.displacementMap && parameters.supportsVertexTextures ? "#define USE_DISPLACEMENTMAP" : "",

                        parameters.specularMap ? "#define USE_SPECULARMAP" : "",
                        parameters.specularIntensityMap ? "#define USE_SPECULARINTENSITYMAP" : "",
                        parameters.specularColorMap ? "#define USE_SPECULARCOLORMAP" : "",

                        parameters.roughnessMap ? "#define USE_ROUGHNESSMAP" : "",
                        parameters.metalnessMap ? "#define USE_METALNESSMAP" : "",
                        parameters.alphaMap ? "#define USE_ALPHAMAP" : "",

                        parameters.transmission ? "#define USE_TRANSMISSION" : "",
                        parameters.transmissionMap ? "#define USE_TRANSMISSIONMAP" : "",
                        parameters.thicknessMap ? "#define USE_THICKNESSMAP" : "",

                        parameters.sheenColorMap ? "#define USE_SHEENCOLORMAP" : "",
                        parameters.sheenRoughnessMap ? "#define USE_SHEENROUGHNESSMAP" : "",

                        parameters.vertexTangents ? "#define USE_TANGENT" : "",
                        parameters.vertexColors ? "#define USE_COLOR" : "",
                        parameters.vertexAlphas ? "#define USE_COLOR_ALPHA" : "",
                        parameters.vertexUvs ? "#define USE_UV" : "",
                        parameters.uvsVertexOnly ? "#define UVS_VERTEX_ONLY" : "",

                        parameters.flatShading ? "#define FLAT_SHADED" : "",

                        parameters.skinning ? "#define USE_SKINNING" : "",

                        parameters.morphTargets ? "#define USE_MORPHTARGETS" : "",
                        parameters.morphNormals && parameters.flatShading == false ? "#define USE_MORPHNORMALS" : "",
                        ( parameters.morphColors && parameters.isGLES3 ) ? "#define USE_MORPHCOLORS" : "",
                        ( parameters.morphTargetsCount > 0 && parameters.isGLES3 ) ? "#define MORPHTARGETS_TEXTURE" : "",
                        ( parameters.morphTargetsCount > 0 && parameters.isGLES3 ) ? "#define MORPHTARGETS_TEXTURE_STRIDE " + parameters.morphTextureStride : "",
                        ( parameters.morphTargetsCount > 0 && parameters.isGLES3 ) ? "#define MORPHTARGETS_COUNT " + parameters.morphTargetsCount : "",
                        parameters.doubleSided ? "#define DOUBLE_SIDED" : "",
                        parameters.flipSided ? "#define FLIP_SIDED" : "",

                        parameters.shadowMapEnabled ? "#define USE_SHADOWMAP" : "",
                        parameters.shadowMapEnabled ? "#define " + shadowMapTypeDefine : "",

                        parameters.sizeAttenuation ? "#define USE_SIZEATTENUATION" : "",

                        parameters.logarithmicDepthBuffer ? "#define USE_LOGDEPTHBUF" : "",
                        ( parameters.logarithmicDepthBuffer && parameters.rendererExtensionFragDepth ) ? "#define USE_LOGDEPTHBUF_EXT" : "",

                        "uniform mat4 modelMatrix;",
                        "uniform mat4 modelViewMatrix;",
                        "uniform mat4 projectionMatrix;",
                        "uniform mat4 viewMatrix;",
                        "uniform mat3 normalMatrix;",
                        "uniform vec3 cameraPosition;",
                        "uniform bool isOrthographic;",

                        "#ifdef USE_INSTANCING",

                        "	attribute mat4 instanceMatrix;",

                        "#endif",

                        "#ifdef USE_INSTANCING_COLOR",

                        "	attribute vec3 instanceColor;",

                        "#endif",

                        "attribute vec3 position;",
                        "attribute vec3 normal;",
                        "attribute vec2 uv;",

                        "#ifdef USE_TANGENT",

                        "	attribute vec4 tangent;",

                        "#endif",

                        "#if defined( USE_COLOR_ALPHA )",

                        "	attribute vec4 color;",

                        "#elif defined( USE_COLOR )",

                        "	attribute vec3 color;",

                        "#endif",

                        "#if ( defined( USE_MORPHTARGETS ) && ! defined( MORPHTARGETS_TEXTURE ) )",

                        "	attribute vec3 morphTarget0;",
                        "	attribute vec3 morphTarget1;",
                        "	attribute vec3 morphTarget2;",
                        "	attribute vec3 morphTarget3;",

                        "	#ifdef USE_MORPHNORMALS",

                        "		attribute vec3 morphNormal0;",
                        "		attribute vec3 morphNormal1;",
                        "		attribute vec3 morphNormal2;",
                        "		attribute vec3 morphNormal3;",

                        "	#else",

                        "		attribute vec3 morphTarget4;",
                        "		attribute vec3 morphTarget5;",
                        "		attribute vec3 morphTarget6;",
                        "		attribute vec3 morphTarget7;",

                        "	#endif",

                        "#endif",

                        "#ifdef USE_SKINNING",

                        "	attribute vec4 skinIndex;",
                        "	attribute vec4 skinWeight;",

                        "#endif",

                        "\\n"

                        };


            prefixVertex = string_utils::join(prefixVertexVec,"\\n");

            vector<string> prefixFragmentVec{
                    customExtensions,

                    generatePrecision( parameters ),

                    "#define SHADER_NAME " + parameters.shaderName,

                    customDefines,

                    ( parameters.useFog && parameters.fog ) ? "#define USE_FOG" : "",
                    ( parameters.useFog && parameters.fogExp2 ) ? "#define FOG_EXP2" : "",

                    parameters.map ? "#define USE_MAP" : "",
                    parameters.matcap ? "#define USE_MATCAP" : "",
                    parameters.envMap ? "#define USE_ENVMAP" : "",
                    parameters.envMap ? "#define " + envMapTypeDefine : "",
                    parameters.envMap ? "#define " + envMapModeDefine : "",
                    parameters.envMap ? "#define " + envMapBlendingDefine : "",

                    parameters.envMapCubeUVHeight ? fmt::format("#define CUBEUV_TEXEL_WIDTH {}" , std::get<0>(envMapCubeUVSize)) : "",
                    parameters.envMapCubeUVHeight ? fmt::format("#define CUBEUV_TEXEL_HEIGHT {}" , std::get<1>(envMapCubeUVSize)) : "",
                    parameters.envMapCubeUVHeight ? fmt::format("#define CUBEUV_MAX_MIP {}.0" , std::get<2>(envMapCubeUVSize)) : "",
                    parameters.lightMap ? "#define USE_LIGHTMAP" : "",
                    parameters.aoMap ? "#define USE_AOMAP" : "",
                    parameters.emissiveMap ? "#define USE_EMISSIVEMAP" : "",
                    parameters.bumpMap ? "#define USE_BUMPMAP" : "",
                    parameters.normalMap ? "#define USE_NORMALMAP" : "",
                    ( parameters.normalMap && parameters.objectSpaceNormalMap ) ? "#define OBJECTSPACE_NORMALMAP" : "",
                    ( parameters.normalMap && parameters.tangentSpaceNormalMap ) ? "#define TANGENTSPACE_NORMALMAP" : "",

                    parameters.clearcoat ? "#define USE_CLEARCOAT" : "",
                    parameters.clearcoatMap ? "#define USE_CLEARCOATMAP" : "",
                    parameters.clearcoatRoughnessMap ? "#define USE_CLEARCOAT_ROUGHNESSMAP" : "",
                    parameters.clearcoatNormalMap ? "#define USE_CLEARCOAT_NORMALMAP" : "",

                    parameters.iridescence ? "#define USE_IRIDESCENCE" : "",
                    parameters.iridescenceMap ? "#define USE_IRIDESCENCEMAP" : "",
                    parameters.iridescenceThicknessMap ? "#define USE_IRIDESCENCE_THICKNESSMAP" : "",

                    parameters.specularMap ? "#define USE_SPECULARMAP" : "",
                    parameters.specularIntensityMap ? "#define USE_SPECULARINTENSITYMAP" : "",
                    parameters.specularColorMap ? "#define USE_SPECULARCOLORMAP" : "",
                    parameters.roughnessMap ? "#define USE_ROUGHNESSMAP" : "",
                    parameters.metalnessMap ? "#define USE_METALNESSMAP" : "",

                    parameters.alphaMap ? "#define USE_ALPHAMAP" : "",
                    parameters.alphaTest ? "#define USE_ALPHATEST" : "",

                    parameters.sheen ? "#define USE_SHEEN" : "",
                    parameters.sheenColorMap ? "#define USE_SHEENCOLORMAP" : "",
                    parameters.sheenRoughnessMap ? "#define USE_SHEENROUGHNESSMAP" : "",

                    parameters.transmission ? "#define USE_TRANSMISSION" : "",
                    parameters.transmissionMap ? "#define USE_TRANSMISSIONMAP" : "",
                    parameters.thicknessMap ? "#define USE_THICKNESSMAP" : "",

                    parameters.decodeVideoTexture ? "#define DECODE_VIDEO_TEXTURE" : "",

                    parameters.vertexTangents ? "#define USE_TANGENT" : "",
                    parameters.vertexColors || parameters.instancingColor ? "#define USE_COLOR" : "",
                    parameters.vertexAlphas ? "#define USE_COLOR_ALPHA" : "",
                    parameters.vertexUvs ? "#define USE_UV" : "",
                    parameters.uvsVertexOnly ? "#define UVS_VERTEX_ONLY" : "",

                    parameters.gradientMap ? "#define USE_GRADIENTMAP" : "",

                    parameters.flatShading ? "#define FLAT_SHADED" : "",

                    parameters.doubleSided ? "#define DOUBLE_SIDED" : "",
                    parameters.flipSided ? "#define FLIP_SIDED" : "",

                    parameters.shadowMapEnabled ? "#define USE_SHADOWMAP" : "",
                    parameters.shadowMapEnabled ? "#define " + shadowMapTypeDefine : "",

                    parameters.premultipliedAlpha ? "#define PREMULTIPLIED_ALPHA" : "",

                    parameters.physicallyCorrectLights ? "#define PHYSICALLY_CORRECT_LIGHTS" : "",

                    parameters.logarithmicDepthBuffer ? "#define USE_LOGDEPTHBUF" : "",
                    ( parameters.logarithmicDepthBuffer && parameters.rendererExtensionFragDepth ) ? "#define USE_LOGDEPTHBUF_EXT" : "",

                    "uniform mat4 viewMatrix;",
                    "uniform vec3 cameraPosition;",
                    "uniform bool isOrthographic;",

                    ( parameters.toneMapping != NoToneMapping ) ? "#define TONE_MAPPING" : "",
                    ( parameters.toneMapping != NoToneMapping ) ? ShaderChunk[ "tonemapping_pars_fragment" ] : "", // this code is required here because it is used by the toneMapping() function defined below
                    ( parameters.toneMapping != NoToneMapping ) ? getToneMappingFunction( "toneMapping", parameters.toneMapping ) : "",

                    parameters.dithering ? "#define DITHERING" : "",
                    parameters.opaque ? "#define OPAQUE" : "",

                    ShaderChunk[ "encodings_pars_fragment" ], // this code is required here because it is used by the various encoding/decoding function defined below
                    getTexelEncodingFunction( "linearToOutputTexel", parameters.outputEncoding ),

                    parameters.useDepthPacking ? "#define DEPTH_PACKING " + parameters.depthPacking : "",

                    "\\n"
            };
            prefixFragment = string_utils::join(prefixFragmentVec,"\\n");

        }

        vertexShader = resolveIncludes( vertexShader );
        vertexShader = replaceLightNums( vertexShader, parameters );
        vertexShader = replaceClippingPlaneNums( vertexShader, parameters );

        fragmentShader = resolveIncludes( fragmentShader );
        fragmentShader = replaceLightNums( fragmentShader, parameters );
        fragmentShader = replaceClippingPlaneNums( fragmentShader, parameters );

        vertexShader = unrollLoops( vertexShader );
        fragmentShader = unrollLoops( fragmentShader );

        if ( parameters.isGLES3 && parameters.isRawShaderMaterial != true ) {
            // GLSL 3.0 conversion for built-in materials and ShaderMaterial
            versionString = "#version 300 es\\n";

            vector<string> prefixVertexVecHere{
                    "precision mediump sampler2DArray;",
                    "#define attribute in",
                    "#define varying out",
                    "#define texture2D texture"
            };
            prefixVertex = string_utils::join(prefixVertexVecHere,"\n") + "\\n" + prefixVertex;

            vector<string> prefixFragmentVecHere{
                    "#define varying in",
                    (parameters.glslVersion == GLSL3) ? "" : "layout(location = 0) out highp vec4 pc_fragColor;",
                    (parameters.glslVersion == GLSL3) ? "" : "#define gl_FragColor pc_fragColor",
                    "#define gl_FragDepthEXT gl_FragDepth",
                    "#define texture2D texture",
                    "#define textureCube texture",
                    "#define texture2DProj textureProj",
                    "#define texture2DLodEXT textureLod",
                    "#define texture2DProjLodEXT textureProjLod",
                    "#define textureCubeLodEXT textureLod",
                    "#define texture2DGradEXT textureGrad",
                    "#define texture2DProjGradEXT textureProjGrad",
                    "#define textureCubeGradEXT textureGrad"
            };

            prefixFragment = string_utils::join(prefixFragmentVecHere,"\\n") + "\\n" + prefixFragment;
        }

        std::string vertexGlsl = versionString + prefixVertex + vertexShader;
        std::string fragmentGlsl = versionString + prefixFragment + fragmentShader;

        return std::make_pair(vertexGlsl,fragmentGlsl);
    }

    std::shared_ptr<GLUniforms> getUniforms(){
        if(cachedUniforms== nullptr){
            cachedUniforms = std::make_shared<GLUniforms>(program);
        }
        return cachedUniforms;
    }

    std::shared_ptr<std::map<std::string,threecpp::GLVertexAttribute>> getAttributes(){
        if(cachedAttributes== nullptr){
            cachedAttributes = fetchAttributeLocations(program);
        }
        return cachedAttributes;
    }


private:
    std::string name;
    int id = programIdCount ++;
    int usedTimes = 1;
    std::string cacheKey;
    GLint program;
    GLint vertexShader;
    GLint fragmentShader;
    std::shared_ptr<GLUniforms> cachedUniforms;
    std::shared_ptr<std::map<std::string,threecpp::GLVertexAttribute>> cachedAttributes;
};






#endif //THREE_CPP_SRC_RENDERS_GL_PROGRAM_H
