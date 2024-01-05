#ifndef SRC_COMMON_UTILS_H_
#define SRC_COMMON_UTILS_H_

#include <exception>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

//#include <gles3/gl3.h>
#include "gl_headers.h"

#ifdef _MSC_VER
#include<io.h>
#define access _access_s
#else
#include <unistd.h>
#endif

namespace threecpp{

//单独的函数声明和定义一定要分开，否则被多次include后，就会出现duplicate symbols
//function decalaration and defintion must be seperated，or else 
//will occur duplicate symbols after included more than once
std::runtime_error genOutOfRangeError(int index);

inline void checkError() {
#ifdef _DEBUG
    std::string errorCodeMessage;
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {

        switch (err) {
            case GL_INVALID_ENUM:
                errorCodeMessage = R"(GL_INVALID_ENUM
                        An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.
                    )";
                break;
            case GL_INVALID_VALUE:
                errorCodeMessage = R"(GL_INVALID_VALUE
                        A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.
                    )";
                break;
            case GL_INVALID_OPERATION:
                errorCodeMessage = R"(GL_INVALID_OPERATION
                        The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.
                    )";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errorCodeMessage = R"(GL_INVALID_FRAMEBUFFER_OPERATION
                        The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.
                    )";
                break;
            case GL_OUT_OF_MEMORY:
                errorCodeMessage = R"(GL_OUT_OF_MEMORY
                        There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.
                    )";
                break;
//            case  GL_STACK_UNDERFLOW:
//                errorCodeMessage = R"(GL_STACK_UNDERFLOW
//                        An attempt has been made to perform an operation that would cause an internal stack to underflow.
//                    )";
//                break;
//            case  GL_STACK_OVERFLOW:
//                errorCodeMessage = R"(GL_STACK_OVERFLOW
//                        An attempt has been made to perform an operation that would cause an internal stack to overflow.
//                    )";
//                break;
        }
        std::cout << " Render Error :" << std::endl << errorCodeMessage << std::endl;
    }
#endif
}


    size_t getSystemTimeInMillis();

    inline std::string getFileSeparator(){
    #ifdef _WIN32
        return "\\";
    #else
        return "/";
    #endif
    }

    bool FileExists(const std::string& fileName);

    std::string getProgramPath();

    //todo:this path maybe not good for release
    std::string getProjectPath();

    inline std::string getFileDir(const std::string& filepath) {
        if (filepath.find_last_of(getFileSeparator()) != std::string::npos)
            return filepath.substr(0, filepath.find_last_of(getFileSeparator()));
        return "";
    }

    inline std::string getFilePathExtension(const std::string &fileName) {
        if (fileName.find_last_of(".") != std::string::npos)
            return fileName.substr(fileName.find_last_of(".") + 1);
        return "";
    }


} // namespace threecpp

#endif /* SRC_COMMON_UTILS_H_ */
