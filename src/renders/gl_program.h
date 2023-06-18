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

#include "constants.h"
#include "common_types.h"
#include "../utils/string_utils.h"
//#include "../utils/string_format.h"
#include "shaders/shader_chunk.h"

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
    bool envMap = false,envMapCubeUVHeight=false;
    bool lightMap = false;
    bool aoMap = false;
    bool emissivMap = false;
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
    bool gradientMap;
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

    bool extensionDerivatives,extensionFragDepth,extensionDrawBuffers,extensionShaderTextureLOD;
    bool rendererExtensionDrawBuffers,rendererExtensionShaderTextureLod;

    std::string glslVersion;
    std::string shaderID;


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
std::map<std::string,threecpp::GLVertexAttribute> fetchAttributeLocations( GLint program ) {
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

    return attributes;
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





#endif //THREE_CPP_SRC_RENDERS_GL_PROGRAM_H
