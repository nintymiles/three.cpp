//
// Created by SeanR on 2023/9/24.
//

#ifndef THREE_CPP_GL_BINDING_STATES_H
#define THREE_CPP_GL_BINDING_STATES_H

#include <GLES3/gl3.h>
#include "buffer_attribute.h"
#include "gl_capabilities.h"

class GLBindingStates{
public:
    int maxVertexAttributes;
    bool vaoAvailable = true;


    GLBindingStates(std::shared_ptr<BufferAttribute<int>> attribute, std::shared_ptr<GLCapabilities> capbilities){
        glGetIntegerv( GL_MAX_VERTEX_ATTRIBS,&maxVertexAttributes );
    }
};


#endif //THREE_CPP_GL_BINDING_STATES_H
