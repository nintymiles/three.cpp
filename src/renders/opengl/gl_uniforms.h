//
// Created by SeanR on 2023/6/22.
//

#ifndef THREE_CPP_GL_UNIFORMS_H
#define THREE_CPP_GL_UNIFORMS_H
#include "common_types.h"
#include "gl_headers.h"

#include "gl_textures.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3.h"
#include "matrix4.h"
#include "color.h"

#include <vector>
#include <map>
#include <string>

/*namespace uniformname {
    static inline bool isRegistered(UniformName name) {
        return name >= UniformName::registered_name_1 && name <= UniformName::registered_name_16;
    }
    static inline UniformName registered(unsigned count) {
        unsigned regname = (unsigned)UniformName::registered_name_1 + count;
        if (regname > (unsigned)UniformName::registered_name_16)
            throw std::out_of_range("too many user-defined uniforms");
        return (UniformName)regname;
    }
    UniformName get(const std::string& name);
}*/
class GLTextures;
class GLUniformContainer;
class GLUniforms;
class UniformValues;
class UniformValue;

class GLUniform{
public:
    using GLTexturesPtr = std::shared_ptr<GLTextures>;
private:
    std::vector<GLuint> allocTextUnits(GLuint n);

protected:
    GLTexturesPtr textures;

public:
    using sptr = std::shared_ptr<GLUniform>;

    UniformClassType uniformClassType = UniformClassType::SingleUniform;
    std::string id;

    GLint addr;
    UniformType type;

    GLUniform() : id(""), type(UniformType::None), addr(-1) {}

    GLUniform(const GLTexturesPtr& textures,std::string& id, UniformType type, const GLint addr) : textures(textures),id(id), type(type), addr(addr) {}


    static sptr create() {
        return std::make_shared<GLUniform>();
    }

    static sptr create(const GLTexturesPtr& textures,std::string& id, UniformType type, const GLint addr) {
        return std::make_shared<GLUniform>(textures,id, type, addr);
    }
    virtual sptr clone(std::string& id) {
        return GLUniform::create(this->textures,id, this->type, this->addr);
    }

    virtual ~GLUniform() = default;

    void setValue(bool b);

    void setValue(const std::vector<unsigned char>& b);

    void setValue(GLfloat f);

    void setValue(GLint i);

    void setValue(GLuint u);

    void setValue(const Color& c);

    void setValue(const Vector2& v);


    void setValue(const Vector3& v);

    void setValue(const Vector4f& v);

    void setValue(const Matrix3& v);

    void setValue(const Matrix4& v);

    void setValue(const std::vector<float>& v);

    void setValue(const std::vector<GLuint>& v);

    void setValue(const Texture::sptr& t);

    void setValue(const std::vector<Vector2>& v);

    void setValue(const std::vector<Vector3>& v);

    void setValue(const std::vector<Vector4f>& v);

    void setValue(const std::vector<Matrix3>& v);

    void setValue(const std::vector<Matrix4>& v);

    void setValue(const std::vector<Texture::sptr>& t);


    virtual GLUniform* asGLUniform() {
        return this;
    }
    virtual GLUniformContainer* asGLContainer() {
        return nullptr;
    }

};
class PureArrayUniform : public GLUniform{
public:
    GLint index;
    using sptr = std::shared_ptr<PureArrayUniform>;

    PureArrayUniform(const GLUniform::GLTexturesPtr& textures, std::string& id, UniformType type, const GLint addr) : GLUniform(textures,id, type, addr), index(0) {
        uniformClassType = UniformClassType::PureArrayUniform;
    }

    static sptr create(const GLUniform::GLTexturesPtr& textures, std::string& id, UniformType type, const GLint addr)
    {
        return std::make_shared<PureArrayUniform>(textures,id, type, addr);
    }

    virtual GLUniform::sptr clone(std::string& id) override {
        return PureArrayUniform::create(this->textures,id, this->type, this->addr);
    }
};

class GLUniformContainer {
    friend class GLUniforms;
protected:


    //std::unordered_map<std::string, std::string> nameRegistry;
public :
    std::unordered_map<std::string, GLUniform::sptr,threecpp::StringHash> seq;
    virtual ~GLUniformContainer() = default;

    /*UniformName registered(std::string name) {
        auto found = nameRegistry.find(name);
        if (found != nameRegistry.end()) return found->second;
        auto uname = uniformname::registered(nameRegistry.size());
        nameRegistry[name] = uname;
        return uname;
    }*/

    void add(GLUniform::sptr uniform) {
        seq[uniform->id] = uniform;
    }
    template <typename T>
    void setUniformValue(std::string& name, const T& value) {
        if (seq.count(name) > 0) {
            seq[name]->setValue(value);
        }
    }

    GLUniform::sptr get(const std::string& name) {
        return seq.count(name) ? seq[name] : nullptr;
    }
    /*GLUniform::ptr get(const size_t index) {
        switch (index) {
        case 0: return get(UniformName::registered_name_1);
        case 1: return get(UniformName::registered_name_2);
        case 2: return get(UniformName::registered_name_3);
        case 3: return get(UniformName::registered_name_4);
        case 4: return get(UniformName::registered_name_5);
        case 5: return get(UniformName::registered_name_6);
        case 6: return get(UniformName::registered_name_7);
        case 7: return get(UniformName::registered_name_8);
        case 8: return get(UniformName::registered_name_9);
        case 9: return get(UniformName::registered_name_10);
        case 10: return get(UniformName::registered_name_11);
        case 11: return get(UniformName::registered_name_12);
        case 12: return get(UniformName::registered_name_13);
        case 13: return get(UniformName::registered_name_14);
        case 14: return get(UniformName::registered_name_15);
        case 15: return get(UniformName::registered_name_16);
        default:
            throw std::logic_error("unsupported index");
        }
    }
    int getIndex(UniformName uniformName) {
        switch (uniformName) {
        case UniformName::registered_name_1: return 0;
        case UniformName::registered_name_2: return 1;
        case UniformName::registered_name_3: return 2;
        case UniformName::registered_name_4: return 3;
        case UniformName::registered_name_5: return 4;
        case UniformName::registered_name_6: return 5;
        case UniformName::registered_name_7: return 6;
        case UniformName::registered_name_8: return 7;
        case UniformName::registered_name_9: return 8;
        case UniformName::registered_name_10: return 9;
        case UniformName::registered_name_11: return 10;
        case UniformName::registered_name_12: return 11;
        case UniformName::registered_name_13: return 12;
        case UniformName::registered_name_14: return 13;
        case UniformName::registered_name_15: return 14;
        case UniformName::registered_name_16: return 15;
        default:
            return -1;
        }
    }*/
};

class StructuredUniform : public GLUniform, public GLUniformContainer{
public:
    using sptr = std::shared_ptr<StructuredUniform>;

    StructuredUniform(const GLUniform::GLTexturesPtr& textures, std::string& id, UniformType type, const GLint addr) : GLUniform(textures,id, type, addr) {
        uniformClassType = UniformClassType::StructuredUniform;
    }

    static sptr create(const GLUniform::GLTexturesPtr& textures, std::string& id, UniformType type, const GLint addr) {
        return std::make_shared<StructuredUniform>(textures,id, type, addr);
    }

    virtual GLUniform::sptr clone(std::string& id) override {
        auto cloned = create(this->textures,this->id, this->type, this->addr);
        cloned->seq = this->seq;
        return cloned;
    }
    virtual GLUniform* asGLUniform() override {
        return nullptr;
    }

    virtual GLUniformContainer* asGLContainer() override {
        return this;
    }
    void setValue(UniformValue& v,int arrayIndex=0);
};

class GLUniforms : public GLUniformContainer {
public:
    using GLTexturesPtr = std::shared_ptr<GLTextures>;

protected:

    GLTexturesPtr textures;

    GLuint program = 0;

    void initGLUniforms(const GLuint program);

public:
    using sptr = std::shared_ptr<GLUniforms>;

    const std::string id = "GLUniforms";

    GLUniforms(const GLTexturesPtr& textures,const GLuint program) :textures(textures) {
        this->program = program;
        initGLUniforms(program);
    }

    static sptr create(const GLTexturesPtr& textures,const GLuint program) {
        return std::make_shared<GLUniforms>(textures,program);
    }

    template <typename T>
    void setUniformValue(const std::string& name, const T& value) {
        if (seq.count(name)) seq[name]->setValue(value);
    }

    void parseUniform(const std::string& name, UniformType activeInfo, const GLint addr, GLUniformContainer* container);

    void setProjectionMatrix(const Matrix4& projMatrix);

    bool contains(const std::string& name) {
        return this->seq.count(name) > 0;
    }

    template<typename T>
    void setOptional(std::unordered_map<std::string,T>& objects, std::string name,GLTextures* textures=nullptr)
    {
        if (objects.count(name)) {
            T value = objects[name];
            setValue(name, value,textures);
        }
    }
    static void upload(const std::vector<GLUniform::sptr>& seq, UniformValues& values);

    std::vector<GLUniform::sptr> seqWithValue(UniformValues& values);
};

//template<typename T>
//inline std::shared_ptr<GLUniforms> cloneUniforms(const GLUniforms& source) {
//	GLUniforms::ptr target = GLUniforms::create();

//	for (auto& entry : source) {
//		target->insert({ entry.first,entry.second });
//	}
//	//target->copy(source);
//	return  target;
//}


//class SingleUniform{
//public:
//    SingleUniform()=default;
//    SingleUniform(std::string id,int addr,threecpp::GLActiveInfo activeInfo):id(id),addr(addr),activeInfo(activeInfo){}
//
//    SingleUniform& setValue(std::vector<threecpp::UniformValue> v){
//        switch (activeInfo.type) {
//            case 0x1406:  // FLOAT
//                setValueV1f(v);
//                break;
//            case 0x8b50:  // _VEC2
//                setValueV2f(v);
//                break;
//            case 0x8b51:  // _VEC3
//                setValueV3f(v);
//                break;
//            case 0x8b52:  // _VEC4
//                setValueV4f(v);
//                break;
//
////            case 0x8b5a: return setValueM2; // _MAT2
////            case 0x8b5b: return setValueM3; // _MAT3
////            case 0x8b5c: return setValueM4; // _MAT4
////
////            case 0x1404: case 0x8b56: return setValueV1i; // INT, BOOL
////            case 0x8b53: case 0x8b57: return setValueV2i; // _VEC2
////            case 0x8b54: case 0x8b58: return setValueV3i; // _VEC3
////            case 0x8b55: case 0x8b59: return setValueV4i; // _VEC4
////
////            case 0x1405: return setValueV1ui; // UINT
////            case 0x8dc6: return setValueV2ui; // _VEC2
////            case 0x8dc7: return setValueV3ui; // _VEC3
////            case 0x8dc8: return setValueV4ui; // _VEC4
////
////            case 0x8b5e: // SAMPLER_2D
////            case 0x8d66: // SAMPLER_EXTERNAL_OES
////            case 0x8dca: // INT_SAMPLER_2D
////            case 0x8dd2: // UNSIGNED_INT_SAMPLER_2D
////            case 0x8b62: // SAMPLER_2D_SHADOW
////                return setValueT1;
////
////            case 0x8b5f: // SAMPLER_3D
////            case 0x8dcb: // INT_SAMPLER_3D
////            case 0x8dd3: // UNSIGNED_INT_SAMPLER_3D
////                return setValueT3D1;
////
////            case 0x8b60: // SAMPLER_CUBE
////            case 0x8dcc: // INT_SAMPLER_CUBE
////            case 0x8dd4: // UNSIGNED_INT_SAMPLER_CUBE
////            case 0x8dc5: // SAMPLER_CUBE_SHADOW
////                return setValueT6;
////
////            case 0x8dc1: // SAMPLER_2D_ARRAY
////            case 0x8dcf: // INT_SAMPLER_2D_ARRAY
////            case 0x8dd7: // UNSIGNED_INT_SAMPLER_2D_ARRAY
////            case 0x8dc4: // SAMPLER_2D_ARRAY_SHADOW
////                return setValueT2DArray1;
//
//        }
//    }
//
//private:
//    // Note: Defining these methods externally, because they come in a bunch
//    // and this way their names minify.
//
//    // Single scalar
//    void setValueV1f( std::vector<threecpp::UniformValue> v ) {
//        if ( cache.size()>0 && cache[0].d == v[0].d) return;
//
//        glUniform1f( addr, v[0].d);
//        cache[0] = v[0];
//    }
//
//    void setValueV2f( std::vector<threecpp::UniformValue> v ) {
//        if ( v.size()>=2 ) {
//            if ( cache[ 0 ].d != v[0].d || cache[ 1 ].d != v[1].d ) {
//                glUniform2f( addr, v[0].d, v[1].d );
//
//                cache[ 0 ] = v[0];
//                cache[ 1 ] = v[1];
//            }
//        }
////        else {
////
////            if ( arraysEqual( cache, v ) ) return;
////
////            gl.uniform2fv( this.addr, v );
////
////            copyArray( cache, v );
////        }
//
//    }
//
//    void setValueV3f( std::vector<threecpp::UniformValue> v ) {
//
//        if ( v.size()>=3 ) {
//            if ( cache[ 0 ].d != v[0].d || cache[ 1 ].d != v[1].d || cache[ 2 ].d != v[2].d ) {
//
//                glUniform3f( addr, v[0].d, v[1].d, v[2].d );
//
//                cache[ 0 ] = v[0];
//                cache[ 1 ] = v[1];
//                cache[ 2 ] = v[2];
//            }
//        }
////        else if ( v.r !== undefined ) {
////
////            if ( cache[ 0 ] !== v.r || cache[ 1 ] !== v.g || cache[ 2 ] !== v.b ) {
////
////                gl.uniform3f( this.addr, v.r, v.g, v.b );
////
////                cache[ 0 ] = v.r;
////                cache[ 1 ] = v.g;
////                cache[ 2 ] = v.b;
////
////            }
////
////        } else {
////
////            if ( arraysEqual( cache, v ) ) return;
////
////            gl.uniform3fv( this.addr, v );
////
////            copyArray( cache, v );
////
////        }
//
//    }
//
//    void setValueV4f( std::vector<threecpp::UniformValue> v ) {
//        if ( v.size()>=4 ) {
//            if ( cache[ 0 ].d != v[0].d || cache[ 1 ].d != v[1].d || cache[ 2 ].d != v[2].d || cache[ 3 ].d != v[3].d ) {
//                glUniform4f( addr, v[0].d, v[1].d, v[2].d, v[3].d );
//
//                cache[ 0 ] = v[0];
//                cache[ 1 ] = v[1];
//                cache[ 2 ] = v[2];
//                cache[ 3 ] = v[3];
//            }
//        }
////        else {
////
////            if ( arraysEqual( cache, v ) ) return;
////
////            gl.uniform4fv( this.addr, v );
////
////            copyArray( cache, v );
////
////        }
//
//    }
//
//
//public:
//    std::string id;
//    int addr;
//    threecpp::GLActiveInfo activeInfo;
//
//private:
//    std::vector<threecpp::UniformValue> cache;
//
//};
//
//class GLUniforms{
//public:
//    GLUniforms(GLint program):programHandle(program){
//        init();
//    }
//
//private:
//    void init(){
//        GLint program = programHandle;
//        GLint maxUniformLen;
//        GLint numUniforms;
//        char *uniformName;
//        GLint index;
//        glGetProgramiv ( program, GL_ACTIVE_UNIFORMS, &numUniforms );
//        glGetProgramiv ( program, GL_ACTIVE_UNIFORM_MAX_LENGTH,
//                         &maxUniformLen );
//        uniformName = (char *)malloc ( sizeof ( char ) * maxUniformLen );
//        for ( index = 0; index < numUniforms; index++ ) {
//            GLint size;
//            GLenum type;
//            GLint location;
//            // Get the uniform info
//            glGetActiveUniform(program, index, maxUniformLen, NULL,
//                               &size, &type, uniformName);
//            // Get the uniform location
//            location = glGetUniformLocation(program, uniformName);
//
//            threecpp::GLActiveInfo activeInfo{name:uniformName,size:size,type:type};
//
//            SingleUniform uniformObj(uniformName,location,activeInfo);
//            seq.push_back(uniformObj);
//            map[uniformObj.id] = uniformObj;
//        }
//
//    }
//private:
//    std::vector<SingleUniform> seq;
//    std::map<std::string,SingleUniform> map;
//    GLint programHandle;
//};

#endif //THREE_CPP_GL_UNIFORMS_H
