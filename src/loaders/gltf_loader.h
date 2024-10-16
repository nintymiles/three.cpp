//
// Created by ultraman on 1/4/2024.
//

#ifndef THREE_CPP_GLTF_LOADER_H
#define THREE_CPP_GLTF_LOADER_H

//#ifndef TINYGLTF_IMPLEMENTATION
//#define TINYGLTF_IMPLEMENTATION
//#endif

//#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
//////#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#endif



//若要不出现无法解析的符号，或者符号重复问题，可以在main函数所在的cpp文件中
//只定义一次宏TINYGLTF_IMPLEMENTATION，同时在定义时也要引入tiny_gltf.h头文件
//初次加载时加载实现，后续引用时只需要单独的header文件即可
//#define TINYGLTF_IMPLEMENTATION
////#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include <tiny_gltf.h>
#include <tiny_gltf.h>
#include <regex>

#include "group.h"

class Texture;
class Material;
class AnimationClip;
class Geometry;
template<class T> class BufferAttribute;

struct GLTFModel{
    Group::sptr scene;
    std::vector<std::shared_ptr<AnimationClip>> animations;
};

class GLTFLoader {
    std::vector<std::shared_ptr<Texture>> pTextures;
    std::vector<std::shared_ptr<BufferAttribute<float>>> pBuffers;
    std::vector<std::shared_ptr<Material>> pMaterials;
    std::vector<std::shared_ptr<AnimationClip>> pAnimations;

    void buildImages(const tinygltf::Model &model);

    void buildTextures(const tinygltf::Model &model);

    void buildMaterials(const tinygltf::Model &model);

    void buildAnimations(const tinygltf::Model &model,Object3D::sptr rootObj);

    std::vector<float> loadBufferWithAccessorId(const tinygltf::Model &model,int accessorId);

    Object3D::sptr getNodeByIndex(int nodeIndex,Object3D::sptr rootObj);

public:

    GLTFLoader() {}

    ~GLTFLoader() = default;

//    void parseState(ObjectState& state);
//
//    void setMaterials(const MTLLoader::MaterialCreator& materials);

    Group::sptr load(const std::string& path);

    GLTFModel loadModel(const std::string& path);

    Object3D::sptr parseNode(tinygltf::Model &model, size_t nodeIndex);

    void parseMesh(tinygltf::Model &model, const tinygltf::Mesh &mesh, Object3D::sptr nodeObj);

    static std::string sanitizeNodeName( std::string name ){
        const std::string _RESERVED_CHARS_RE = R"(\[\]\.:\/)";
        const std::regex _reservedRe{'[' + _RESERVED_CHARS_RE + ']'};

        std::regex regExpression{R"(\s)"};
        return regex_replace(std::regex_replace(name, regExpression, "_",std::regex_constants::match_any),_reservedRe,"");
    }

    static size_t ComponentTypeByteSize(int type) {
        switch (type) {
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
            case TINYGLTF_COMPONENT_TYPE_BYTE:
                return sizeof(char);
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            case TINYGLTF_COMPONENT_TYPE_SHORT:
                return sizeof(short);
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
            case TINYGLTF_COMPONENT_TYPE_INT:
                return sizeof(int);
            case TINYGLTF_COMPONENT_TYPE_FLOAT:
                return sizeof(float);
            case TINYGLTF_COMPONENT_TYPE_DOUBLE:
                return sizeof(double);
            default:
                return 0;
        }
    }



};


#endif //THREE_CPP_GLTF_LOADER_H
