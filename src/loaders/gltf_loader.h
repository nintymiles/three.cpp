//
// Created by ultraman on 1/4/2024.
//

#ifndef THREE_CPP_GLTF_LOADER_H
#define THREE_CPP_GLTF_LOADER_H

//#define TINYGLTF_IMPLEMENTATION
////#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

#include "group.h"

class Texture;
class Material;
template<class T> class BufferAttribute;
class GLTFLoader {
    std::vector<std::shared_ptr<Texture>> pTextures;
    std::vector<std::shared_ptr<BufferAttribute<float>>> pBuffers;
    std::vector<std::shared_ptr<Material>> pMaterials;

    void buildImages(const tinygltf::Model &model);

    void buildMaterials(const tinygltf::Model &model);

public:

    GLTFLoader() {}

    ~GLTFLoader() = default;

//    void parseState(ObjectState& state);
//
//    void setMaterials(const MTLLoader::MaterialCreator& materials);

    Group::sptr load(const std::string& path);

    Group::sptr parseNode(tinygltf::Model &model, const tinygltf::Node &node);

    void parseMesh(tinygltf::Model &model, const tinygltf::Mesh &mesh, Group::sptr nodeGroup);

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
