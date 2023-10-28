#ifndef SRC_COMMON_UTILS_H_
#define SRC_COMMON_UTILS_H_

#include <exception>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

#include <GLES3/gl3.h>

namespace threecpp{

//单独的函数声明和定义一定要分开，否则被多次include后，就会出现duplicate symbols
//function decalaration and defintion must be seperated，or else 
//will occur duplicate symbols after included more than once
std::runtime_error genOutOfRangeError(int index);

inline void toUpperCase(std::string& str){
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

inline void toLowerCase(std::string& str){
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

inline void checkError() {
#ifdef _DEBUG
    std::string errorCodeMessage;
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {

        switch (err) {
            case  GL_INVALID_ENUM:
                errorCodeMessage = R"(GL_INVALID_ENUM
                        An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.
                    )";
                break;
            case  GL_INVALID_VALUE:
                errorCodeMessage = R"(GL_INVALID_VALUE
                        A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.
                    )";
                break;
            case  GL_INVALID_OPERATION:
                errorCodeMessage = R"(GL_INVALID_OPERATION
                        The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.
                    )";
                break;
            case  GL_INVALID_FRAMEBUFFER_OPERATION:
                errorCodeMessage = R"(GL_INVALID_FRAMEBUFFER_OPERATION
                        The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.
                    )";
                break;
            case  GL_OUT_OF_MEMORY:
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

} // namespace threecpp


#endif /* SRC_COMMON_UTILS_H_ */
