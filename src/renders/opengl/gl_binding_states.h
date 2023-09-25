//
// Created by SeanR on 2023/9/24.
//

#ifndef THREE_CPP_GL_BINDING_STATES_H
#define THREE_CPP_GL_BINDING_STATES_H

#include <GLES3/gl3.h>
#include "buffer_attribute.h"
#include "gl_capabilities.h"

//?主要用来管理vertex array的binding状态
class GLBindingStates{
public:
    //named data members
    std::shared_ptr<BufferGeometry<int,double>> geometry;
    std::shared_ptr<GLProgram> program;
    bool wireframe = false;
    std::shared_ptr<BufferAttribute<int,double>> newAttributes;
    std::shared_ptr<BufferAttribute<int,double>> enabledAttributes;


    int maxVertexAttributes;
    bool vaoAvailable = true;





    GLBindingStates(std::shared_ptr<BufferAttribute<int>> attribute, std::shared_ptr<GLCapabilities> capbilities){
        glGetIntegerv( GL_MAX_VERTEX_ATTRIBS,&maxVertexAttributes );
    }

    void setup( Gluint vaoObject, material, program, geometry, index ) {

    void bindVertexArrayObject( GLuint vao ) {
        glBindVertexArray( vao );
    }

    void deleteVertexArrayObject( GLuint vao ) {
        //glDeleteVertexArray( vao );
        glDeleteVertexArrays(1,&vao);
    }

};


#endif //THREE_CPP_GL_BINDING_STATES_H
