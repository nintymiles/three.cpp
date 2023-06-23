//
// Created by SeanR on 2023/6/22.
//

#ifndef THREE_CPP_GL_UNIFORMS_H
#define THREE_CPP_GL_UNIFORMS_H
#include "common_types.h"

#include <vector>
#include <map>
#include <string>
#include <GLES3/gl3.h>

class SingleUniform{
public:
    SingleUniform()=default;
    SingleUniform(std::string id,int addr,threecpp::GLActiveInfo activeInfo):id(id),addr(addr),activeInfo(activeInfo){}

    SingleUniform& setValue(std::vector<threecpp::UniformValue> v){
        switch (activeInfo.type) {
            case 0x1406:  // FLOAT
                setValueV1f(v);
                break;
            case 0x8b50:  // _VEC2
                setValueV2f(v);
                break;
            case 0x8b51:  // _VEC3
                setValueV3f(v);
                break;
            case 0x8b52:  // _VEC4
                setValueV4f(v);
                break;

//            case 0x8b5a: return setValueM2; // _MAT2
//            case 0x8b5b: return setValueM3; // _MAT3
//            case 0x8b5c: return setValueM4; // _MAT4
//
//            case 0x1404: case 0x8b56: return setValueV1i; // INT, BOOL
//            case 0x8b53: case 0x8b57: return setValueV2i; // _VEC2
//            case 0x8b54: case 0x8b58: return setValueV3i; // _VEC3
//            case 0x8b55: case 0x8b59: return setValueV4i; // _VEC4
//
//            case 0x1405: return setValueV1ui; // UINT
//            case 0x8dc6: return setValueV2ui; // _VEC2
//            case 0x8dc7: return setValueV3ui; // _VEC3
//            case 0x8dc8: return setValueV4ui; // _VEC4
//
//            case 0x8b5e: // SAMPLER_2D
//            case 0x8d66: // SAMPLER_EXTERNAL_OES
//            case 0x8dca: // INT_SAMPLER_2D
//            case 0x8dd2: // UNSIGNED_INT_SAMPLER_2D
//            case 0x8b62: // SAMPLER_2D_SHADOW
//                return setValueT1;
//
//            case 0x8b5f: // SAMPLER_3D
//            case 0x8dcb: // INT_SAMPLER_3D
//            case 0x8dd3: // UNSIGNED_INT_SAMPLER_3D
//                return setValueT3D1;
//
//            case 0x8b60: // SAMPLER_CUBE
//            case 0x8dcc: // INT_SAMPLER_CUBE
//            case 0x8dd4: // UNSIGNED_INT_SAMPLER_CUBE
//            case 0x8dc5: // SAMPLER_CUBE_SHADOW
//                return setValueT6;
//
//            case 0x8dc1: // SAMPLER_2D_ARRAY
//            case 0x8dcf: // INT_SAMPLER_2D_ARRAY
//            case 0x8dd7: // UNSIGNED_INT_SAMPLER_2D_ARRAY
//            case 0x8dc4: // SAMPLER_2D_ARRAY_SHADOW
//                return setValueT2DArray1;

        }
    }

private:
    // Note: Defining these methods externally, because they come in a bunch
    // and this way their names minify.

    // Single scalar
    void setValueV1f( std::vector<threecpp::UniformValue> v ) {
        if ( cache.size()>0 && cache[0].d == v[0].d) return;

        glUniform1f( addr, v[0].d);
        cache[0] = v[0];
    }

    void setValueV2f( std::vector<threecpp::UniformValue> v ) {
        if ( v.size()>=2 ) {
            if ( cache[ 0 ].d != v[0].d || cache[ 1 ].d != v[1].d ) {
                glUniform2f( addr, v[0].d, v[1].d );

                cache[ 0 ] = v[0];
                cache[ 1 ] = v[1];
            }
        }
//        else {
//
//            if ( arraysEqual( cache, v ) ) return;
//
//            gl.uniform2fv( this.addr, v );
//
//            copyArray( cache, v );
//        }

    }

    void setValueV3f( std::vector<threecpp::UniformValue> v ) {

        if ( v.size()>=3 ) {
            if ( cache[ 0 ].d != v[0].d || cache[ 1 ].d != v[1].d || cache[ 2 ].d != v[2].d ) {

                glUniform3f( addr, v[0].d, v[1].d, v[2].d );

                cache[ 0 ] = v[0];
                cache[ 1 ] = v[1];
                cache[ 2 ] = v[2];
            }
        }
//        else if ( v.r !== undefined ) {
//
//            if ( cache[ 0 ] !== v.r || cache[ 1 ] !== v.g || cache[ 2 ] !== v.b ) {
//
//                gl.uniform3f( this.addr, v.r, v.g, v.b );
//
//                cache[ 0 ] = v.r;
//                cache[ 1 ] = v.g;
//                cache[ 2 ] = v.b;
//
//            }
//
//        } else {
//
//            if ( arraysEqual( cache, v ) ) return;
//
//            gl.uniform3fv( this.addr, v );
//
//            copyArray( cache, v );
//
//        }

    }

    void setValueV4f( std::vector<threecpp::UniformValue> v ) {
        if ( v.size()>=4 ) {
            if ( cache[ 0 ].d != v[0].d || cache[ 1 ].d != v[1].d || cache[ 2 ].d != v[2].d || cache[ 3 ].d != v[3].d ) {
                glUniform4f( addr, v[0].d, v[1].d, v[2].d, v[3].d );

                cache[ 0 ] = v[0];
                cache[ 1 ] = v[1];
                cache[ 2 ] = v[2];
                cache[ 3 ] = v[3];
            }
        }
//        else {
//
//            if ( arraysEqual( cache, v ) ) return;
//
//            gl.uniform4fv( this.addr, v );
//
//            copyArray( cache, v );
//
//        }

    }


public:
    std::string id;
    int addr;
    threecpp::GLActiveInfo activeInfo;

private:
    std::vector<threecpp::UniformValue> cache;

};

class GLUniforms{
public:
    GLUniforms(GLint program):programHandle(program){
        init();
    }

private:
    void init(){
        GLint program = programHandle;
        GLint maxUniformLen;
        GLint numUniforms;
        char *uniformName;
        GLint index;
        glGetProgramiv ( program, GL_ACTIVE_UNIFORMS, &numUniforms );
        glGetProgramiv ( program, GL_ACTIVE_UNIFORM_MAX_LENGTH,
                         &maxUniformLen );
        uniformName = (char *)malloc ( sizeof ( char ) * maxUniformLen );
        for ( index = 0; index < numUniforms; index++ ) {
            GLint size;
            GLenum type;
            GLint location;
            // Get the uniform info
            glGetActiveUniform(program, index, maxUniformLen, NULL,
                               &size, &type, uniformName);
            // Get the uniform location
            location = glGetUniformLocation(program, uniformName);

            threecpp::GLActiveInfo activeInfo{name:uniformName,size:size,type:type};

            SingleUniform uniformObj(uniformName,location,activeInfo);
            seq.push_back(uniformObj);
            map[uniformObj.id] = uniformObj;
        }

    }
private:
    std::vector<SingleUniform> seq;
    std::map<std::string,SingleUniform> map;
    GLint programHandle;
};

#endif //THREE_CPP_GL_UNIFORMS_H
