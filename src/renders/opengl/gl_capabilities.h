//
// Created by SeanR on 2022/12/16.
//

#ifndef THREE_CPP_SRC_RENDERS_OPENGL_OPENGL_CAPABILITIES_H
#define THREE_CPP_SRC_RENDERS_OPENGL_OPENGL_CAPABILITIES_H

#include <string>
#include <map>
#include <GLES3/gl3.h>

#include "gl_extensions.h"
#include "common_utils.h"

class GLCapabilities{
public:
  using sptr = std::shared_ptr<GLCapabilities>;

  struct GLCapabilitiesParameters {
      bool logarithmicDepthBuffer = false;
      std::string precision;
  };

    std::string precision;

//  int maxAnisotropy;
//  bool isGLES3 = true;
//  bool logarithmicDepthBuffer;
//  bool drawBuffers = true;
//
//  int maxTextures,maxVertexTextures,maxTextureSize,maxCubemapSize,
//        maxAttributes,maxVertexUniforms,maxVaryings,maxFragmentUniforms,maxSamples;

  bool logarithmicDepthBuffer = false;
  GLint maxTextures = 0;
  GLint maxVertexTextures = 0;
  GLsizei maxTextureSize = 0;
  GLsizei maxCubemapSize = 0;
  GLint maxAttributes = 0;
  GLint maxVertexUniforms = 0;
  GLint maxVaryings = 0;
  GLint maxFragmentUniforms = 0;

  bool vertexTextures = false;
  bool floatFragmentTextures =false;
  bool floatVertexTextures = false;
  bool isGLES3 = false;

  GLint maxAnisotropy = -1;
  GLint maxSamples = 0;
  GLExtensions::sptr extensions;
  GLCapabilitiesParameters parameters;


//    GLCapabilities(std::map<std::string,std::string> parameters){
//
//        std::string precision = parameters.find("precision")!=end(parameters)?parameters.at("precesion"):"highp";
//
//        //const drawBuffers = isWebGL2 || extensions.has( 'WEBGL_draw_buffers' );
//        //const logarithmicDepthBuffer = parameters.logarithmicDepthBuffer == true;
//        logarithmicDepthBuffer = parameters["logarithmicDepthBuffer"] == "true";
//
//        glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS,&maxTextures );
//        glGetIntegerv( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&maxVertexTextures );
//        glGetIntegerv( GL_MAX_TEXTURE_SIZE,&maxTextureSize );
//        glGetIntegerv( GL_MAX_CUBE_MAP_TEXTURE_SIZE,&maxCubemapSize );
//
//        glGetIntegerv( GL_MAX_VERTEX_ATTRIBS,&maxAttributes );
//        glGetIntegerv( GL_MAX_VERTEX_UNIFORM_VECTORS,&maxVertexUniforms );
//        glGetIntegerv( GL_MAX_VARYING_VECTORS,&maxVaryings );
//        glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_VECTORS,&maxFragmentUniforms );
//
////    const vertexTextures = maxVertexTextures > 0;
////    const floatFragmentTextures = isWebGL2 || extensions.has( 'OES_texture_float' );
////    const floatVertexTextures = vertexTextures && floatFragmentTextures;
//
//        glGetIntegerv( GL_MAX_SAMPLES,&maxSamples );
//
//    }

    GLCapabilities(GLExtensions::sptr& extensions, const GLCapabilitiesParameters& parameters) : extensions(extensions), parameters(parameters){
        std::string maxPrecision = getMaxPrecision(precision);

        if (maxPrecision != this->precision) {
            this->precision = maxPrecision;
        }

        isGLES3 = true;// extensions->get(Extension::ARB_ES3_compatibility);

        this->logarithmicDepthBuffer = parameters.logarithmicDepthBuffer == true;

        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);
        glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextures);
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &maxCubemapSize);
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxVertexUniforms);
        glGetIntegerv(GL_MAX_VARYING_VECTORS, &maxVaryings);
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxFragmentUniforms);

        vertexTextures = maxVertexTextures > 0;
        floatFragmentTextures = isGLES3 || !extensions->get(Extension::OES_texture_float);
        floatVertexTextures = vertexTextures && floatFragmentTextures;

        glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);

        maxSamples = isGLES3 ? maxSamples : 0;

        threecpp::checkError();
    }

    virtual ~GLCapabilities() = default;

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
    float getMaxAnisotropy() {
        if (maxAnisotropy < 0) return maxAnisotropy;
        /*if (extensions.get(Extension::EXT_texture_filter_anisotropic))
            glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        else*/
        maxAnisotropy = 0;

        return maxAnisotropy;
    }

    std::string getMaxPrecision(std::string& precision) {
        GLint r[2], p;
        if (precision == "highp") {
            glGetShaderPrecisionFormat(GL_VERTEX_SHADER, GL_HIGH_FLOAT, r, &p);
            if (p > 0) {
                glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER, GL_HIGH_FLOAT, r, &p);
                if (p > 0)
                    return "highp";
            }
            precision = "miduump";
        }
        if (precision == "mediump") {
            glGetShaderPrecisionFormat(GL_VERTEX_SHADER, GL_MEDIUM_FLOAT, r, &p);
            if (p > 0)
                return "mediump";
        }
        return "highp";
        //return "lowp";
    }


};

//static const GLCapabilities instance{{}};



#endif //THREE_CPP_SRC_RENDERS_OPENGL_OPENGL_CAPABILITIES_H
