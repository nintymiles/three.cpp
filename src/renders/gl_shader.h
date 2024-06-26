//
// Created by SeanR on 2022/11/24.
//

#ifndef THREE_CPP_SRC_RENDERS_GL_SHADER_H
#define THREE_CPP_SRC_RENDERS_GL_SHADER_H

#include "gl_headers.h"
#include "uniform_values.h"

#include <string>
#include <regex>
#include <cmath>
#include <sstream>
#include <map>
#include <memory>

namespace threecpp{
    namespace shader_util{
        //GLuint glShader( GLenum type, std::string shaderSrc) {
        //    GLuint shader = glCreateShader( type );
        //    const char * shaderSource = shaderSrc.c_str();
        //    glShaderSource( shader, 1, &shaderSource,NULL);
        //    glCompileShader( shader );
        //
        //    return shader;
        //}

        inline std::string getShaderErrors( GLuint shader, GLenum type ) {
            GLint infoLen;
            glGetShaderiv( shader, GL_COMPILE_STATUS,&infoLen );
            GLchar *info = (GLchar *)malloc ( sizeof ( GLchar ) * infoLen );
            glGetShaderInfoLog( shader,infoLen,NULL,info);

            std::string error = std::string(info);
            if(infoLen && (error == "")) return error;

            const std::regex err_regex("ERROR: 0:(\\d+)");
            std::cmatch err_smatch;

            std::string errorLineStr;
            if(std::regex_search(error.c_str(),err_smatch,err_regex)){
                if(err_smatch.size()>0){
                    errorLineStr = err_smatch.str();
                }
            }

            int errorLine;
            if(errorLineStr.size()>0){
                errorLine = std::stoi(errorLineStr);
            }

            //const errorLine = parseInt( /ERROR: 0:(\d+)/.exec( errors )[ 1 ] );

            // --enable-privileged-webgl-extension
            // console.log( '**' + type + '**', gl.getExtension( 'WEBGL_debug_shaders' ).getTranslatedShaderSource( shader ) );

            //return type.toUpperCase() + '\n\n' + errors + '\n\n' + handleSource( gl.getShaderSource( shader ), errorLine );
            return error;
        }

        inline void stringsplit(const std::string& str, const char split, std::vector<std::string>& res){
            if (str == "") return;

            std::string strs = str + split;
            size_t pos = strs.find(split);

            while (pos != strs.npos){
                std::string temp = strs.substr(0, pos);
                res.push_back(temp);

                strs = strs.substr(pos + 1, strs.size());
                pos = strs.find(split);
            }
        }

        inline std::string handleSource( std::string error, int errorLine ) {
            std::vector<std::string> lines_transformed;
            std::vector<std::string> lines;
            stringsplit( error,'\n',lines );

            const int from = std::max<int>( errorLine - 6, 0 );
            const int to = std::min<int>( errorLine + 6, lines.size() );

            std::stringstream ss;
            for ( int i = from; i < to; i ++ ) {
                //lines_transformed.push_back( ("" + ( i + 1 )) + ": " + lines[ i ] );
                ss << i+1 << ": " << lines[i] ;
                //lines_transformed.push_back(ss.str());
                if(i<(to-1))
                    ss << "\n";
            }

            //return lines_transformed.join( '\n' );
            return ss.str();

        }

        enum EncodingComponents{
            LinearEncoding,
            sRGBEncoding
        };

        inline std::pair<std::string,std::string> getEncodingComponents( EncodingComponents encoding ) {
            switch( encoding ){
                case LinearEncoding:
                    return { "Linear", "( value )" };
                case sRGBEncoding:
                    return { "sRGB", "( value )" };
                default:
                    //console.warn( 'THREE.WebGLProgram: Unsupported encoding:', encoding );
                    return { "Linear", "( value )" };
            }
        }

    }
}



class LibUniformValues;
class GLShader {
public:
    using sptr = std::shared_ptr<GLShader>;

    unsigned type;

    std::string name="";
    std::string code ="";

    const char* vertexShader;
    const char* fragmentShader;

    unsigned shader = -1;

    LibUniformValues uniforms;

    GLShader() :type(GL_VERTEX_SHADER){}

    GLShader(const char* vertexShader, const char* fragmentShader, const LibUniformValues& uniforms)
            : type(GL_VERTEX_SHADER), vertexShader(vertexShader), fragmentShader(fragmentShader), uniforms(uniforms) {}

    GLShader(unsigned type, const std::string& code) {
        this->type = type;
        this->code = code;

        shader = glCreateShader(type);

        const char* shaderCode = code.c_str();
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);
    }

    static sptr create() {
        return std::make_shared<GLShader>();
    }

    LibUniformValues& getUniforms() {
        return uniforms;
    }

};




#endif //THREE_CPP_SRC_RENDERS_GL_SHADER_H
