//
// Created by SeanR on 2023/9/24.
//

#ifndef THREE_CPP_GL_BINDING_STATES_H
#define THREE_CPP_GL_BINDING_STATES_H

#include "gl_headers.h"

#include "buffer_attribute.h"
#include "interleaved_buffer.h"
#include "geometry.h"
#include "buffer_geometry.h"
#include "gl_capabilities.h"
#include "gl_attributes.h"
#include "material.h"

#include <memory>
#include <unordered_map>

namespace threecpp {
struct CacheData {
std::shared_ptr<BufferAttribute<float>> attribute;
std::shared_ptr<InterleavedBuffer<float>> data;
};

struct BindingStateStruct {
    sole::uuid uuid;
    size_t geometry;
    size_t program;
    bool wireframe;
    vector<GLuint> newAttributes;
    vector<GLuint> enabledAttributes;
    vector<GLuint> attributeDivisors;
    GLuint vao;
    std::unordered_map<AttributeNameKey, CacheData> attributes;
    BufferAttribute<unsigned>::sptr index;

    bool equals(const BindingStateStruct &other) {
        return uuid == other.uuid;
    }
};

}

using StateStruct = std::unordered_map<size_t, threecpp::BindingStateStruct>;
using ProgramStruct = std::unordered_map<size_t, StateStruct>;

class GLProgram;

class GLBindingStates {
private:
    GLExtensions::sptr extensions;
    GLCapabilities::sptr capabilities;
    GLAttributes::sptr attributes;
    bool vaoAvailable = false;
    bool isGLES3 = false;
    GLint maxVertexAttributes;

    void createVertexArrayObject(GLuint* vao);

    void bindVertexArrayObject(GLuint vao);

    void deleteVertexArrayObject(GLuint* vao);

    threecpp::BindingStateStruct& getBindingState(const Geometry::sptr& geometry, const std::shared_ptr<GLProgram>& program, const Material::sptr& material);

    threecpp::BindingStateStruct createBindingState(GLuint vao);

    bool needsUpdate(const BufferGeometry::sptr& bufferGeometry, const BufferAttribute<unsigned>::sptr index);

    void saveCache(const BufferGeometry::sptr& bufferGeometry, const BufferAttribute<unsigned>::sptr& index);

    void enableAttributeAndDivisor(GLuint attribute, GLuint meshPerAttribute);

    void vertexAttribPointer(GLuint index, GLint size, GLenum type, bool normalized, GLuint stride, GLuint offset);

    void setupVertexAttributes(const Object3D::sptr& object, const Material::sptr& material, const std::shared_ptr<GLProgram>& program, const BufferGeometry::sptr& bufferGeometry);

public :
    using sptr = std::shared_ptr<GLBindingStates>;

    std::unordered_map<size_t, ProgramStruct> bindingStates;
    threecpp::BindingStateStruct defaultState;
    threecpp::BindingStateStruct currentState;

    GLBindingStates(const GLExtensions::sptr& extensions, const GLAttributes::sptr& attributes, const GLCapabilities::sptr& capabilities);

    static sptr create(const GLExtensions::sptr& extensions, const GLAttributes::sptr& attributes, const GLCapabilities::sptr& capabilities) {
        return std::make_shared<GLBindingStates>(extensions, attributes, capabilities);
    }
    virtual ~GLBindingStates();

    void initAttributes();

    void setUp(const Object3D::sptr& object, const Material::sptr& material, const std::shared_ptr<GLProgram>& program, const BufferGeometry::sptr& geometry, const BufferAttribute<unsigned>::sptr& index);

    void reset();

    void resetDefaultState();

    void releaseStatesOfGeometry(const Geometry::sptr& geometry);

    void releaseStatesOfProgram(const std::shared_ptr<GLProgram>& program);

    void enableAttribute(GLuint attribute);

    void disableUnusedAttributes();

};

////?主要用来管理vertex array的binding状态
//class GLBindingStates{
//public:
//    //named data members
//    std::shared_ptr<BufferGeometry<int,double>> geometry;
//    std::shared_ptr<GLProgram> program;
//    bool wireframe = false;
//    std::shared_ptr<BufferAttribute<int,double>> newAttributes;
//    std::shared_ptr<BufferAttribute<int,double>> enabledAttributes;
//
//
//    int maxVertexAttributes;
//    bool vaoAvailable = true;
//
//
//
//
//
//    GLBindingStates(std::shared_ptr<BufferAttribute<int>> attribute, std::shared_ptr<GLCapabilities> capbilities){
//        glGetIntegerv( GL_MAX_VERTEX_ATTRIBS,&maxVertexAttributes );
//    }
//
//    void setup( Gluint vaoObject, material, program, geometry, index ) {
//
//    void bindVertexArrayObject( GLuint vao ) {
//        glBindVertexArray( vao );
//    }
//
//    void deleteVertexArrayObject( GLuint vao ) {
//        //glDeleteVertexArray( vao );
//        glDeleteVertexArrays(1,&vao);
//    }
//
//};


#endif //THREE_CPP_GL_BINDING_STATES_H
