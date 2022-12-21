//
// Created by SeanR on 2022/12/16.
//

#ifndef THREE_CPP_SRC_RENDERS_OPENGL_OPENGL_CAPABILITIES_H
#define THREE_CPP_SRC_RENDERS_OPENGL_OPENGL_CAPABILITIES_H

#include <string>
#include <map>
#include <GLES3/gl3.h>

class OpenGLCapabilities{
public:
  int maxAnisotropy;
  bool isGLES3 = true;
  bool logarithmicDepthBuffer;

  int maxTextures,maxVertexTextures,maxTextureSize,maxCubemapSize,
        maxAttributes,maxVertexUniforms,maxVaryings,maxFragmentUniforms,maxSamples;


  std::string getMaxPrecision( std::string precision ) {

        if ( precision == "highp" ) {

//            if ( glGetShaderPrecisionFormat( GL_VERTEX_SHADER, GL_FLOAT ).precision > 0 &&
//                glGetShaderPrecisionFormat( GL_FRAGMENT_SHADER, GL_FLOAT ).precision > 0 ) {

                return "highp";

//            }

            precision = "mediump";

        }

        if ( precision == "mediump" ) {

//            if ( gl.getShaderPrecisionFormat( gl.VERTEX_SHADER, gl.MEDIUM_FLOAT ).precision > 0 &&
//                gl.getShaderPrecisionFormat( gl.FRAGMENT_SHADER, gl.MEDIUM_FLOAT ).precision > 0 ) {

                return "mediump";

//            }

        }
        return "lowp";
    }

    OpenGLCapabilities(std::map<std::string,std::string> parameters){

        std::string precision = parameters.find("precision")!=end(parameters)?parameters.at("precesion"):"highp";

        //const drawBuffers = isWebGL2 || extensions.has( 'WEBGL_draw_buffers' );
        //const logarithmicDepthBuffer = parameters.logarithmicDepthBuffer == true;
        logarithmicDepthBuffer = parameters["logarithmicDepthBuffer"] == "true";

        glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS,&maxTextures );
        glGetIntegerv( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&maxVertexTextures );
        glGetIntegerv( GL_MAX_TEXTURE_SIZE,&maxTextureSize );
        glGetIntegerv( GL_MAX_CUBE_MAP_TEXTURE_SIZE,&maxCubemapSize );

        glGetIntegerv( GL_MAX_VERTEX_ATTRIBS,&maxAttributes );
        glGetIntegerv( GL_MAX_VERTEX_UNIFORM_VECTORS,&maxVertexUniforms );
        glGetIntegerv( GL_MAX_VARYING_VECTORS,&maxVaryings );
        glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_VECTORS,&maxFragmentUniforms );

//    const vertexTextures = maxVertexTextures > 0;
//    const floatFragmentTextures = isWebGL2 || extensions.has( 'OES_texture_float' );
//    const floatVertexTextures = vertexTextures && floatFragmentTextures;

        glGetIntegerv( GL_MAX_SAMPLES,&maxSamples );

    }

    //    function getMaxAnisotropy() {
//
//        if ( maxAnisotropy !== undefined ) return maxAnisotropy;
//
//        if ( extensions.has( 'EXT_texture_filter_anisotropic' ) === true ) {
//
//            const extension = extensions.get( 'EXT_texture_filter_anisotropic' );
//
//            maxAnisotropy = gl.getParameter( extension.MAX_TEXTURE_MAX_ANISOTROPY_EXT );
//
//        } else {
//
//            maxAnisotropy = 0;
//
//        }
//
//        return maxAnisotropy;
//
//    }


};

//static const OpenGLCapabilities instance{{}};



#endif //THREE_CPP_SRC_RENDERS_OPENGL_OPENGL_CAPABILITIES_H
