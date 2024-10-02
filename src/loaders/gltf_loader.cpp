//
// Created by SeanR on 1/4/2024.
//

#include "gltf_loader.h"

#include "buffer_geometry.h"
#include "mesh.h"
#include "common_utils.h"

#include "texture.h"
#include "material.h"
#include "mesh_standard_material.h"
#include "mesh_phong_material.h"

#include <algorithm>



#define BUFFER_OFFSET(i) ((float *)NULL + (i))


namespace gltf_loaders{
//    tinygltf::Model model;
//    tinygltf::TinyGLTF loader;
//    std::string err;
//    std::string warn;
}

using namespace gltf_loaders;
Group::sptr GLTFLoader::load(const std::string& path){
    Group::sptr group = Group::create();

    std::string ext = threecpp::getFilePathExtension(path);

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = false;
    if (ext.compare("glb") == 0) {
        // assume binary glTF.
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, path.c_str());
    } else {
        // assume ascii glTF.
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, path.c_str());
    }

    if (!warn.empty()) {
        printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("ERR: %s\n", err.c_str());
    }
    if (!ret) {
        printf("Failed to load .glTF : %s\n", path.c_str());
        exit(-1);
    }

    buildTextures(model);
    buildMaterials(model);

    // If the glTF asset has at least one scene, and doesn't define a default one
    // just show the first one we can find
    assert(model.scenes.size() > 0);
    auto defaultScene = model.defaultScene > -1 ? model.defaultScene : 0;
    auto gScene = model.scenes[defaultScene];
    group->name = gScene.name;


//    int scene_to_display = model.defaultScene > -1 ? model.defaultScene : 0;
//    const tinygltf::Scene &scene = model.scenes[scene_to_display];
    for (size_t i = 0; i < gScene.nodes.size(); i++) {
        group->add(parseNode(model, model.nodes[gScene.nodes[i]]));
    }

    return group;
}

Group::sptr GLTFLoader::parseNode(tinygltf::Model &model, const tinygltf::Node &node) {

    // Apply xform

//    glPushMatrix();
//    if (node.matrix.size() == 16) {
//        // Use `matrix' attribute
//        glMultMatrixd(node.matrix.data());
//    } else {
//        // Assume Trans x Rotate x Scale order
//        if (node.translation.size() == 3) {
//            glTranslated(node.translation[0], node.translation[1],
//                         node.translation[2]);
//        }
//
//        if (node.rotation.size() == 4) {
//            double angleDegrees;
//            double axis[3];
//
//            QuatToAngleAxis(node.rotation, angleDegrees, axis);
//
//            glRotated(angleDegrees, axis[0], axis[1], axis[2]);
//        }
//
//        if (node.scale.size() == 3) {
//            glScaled(node.scale[0], node.scale[1], node.scale[2]);
//        }
//    }

     std::cout << "node " << node.name << ", Meshes " << node.mesh << std::endl;

     Group::sptr nodeGrp = Group::create();
     nodeGrp->name = node.name;

     if(!node.translation.empty() && node.translation.size() >= 3)
        nodeGrp->position.set(node.translation[0],node.translation[1],node.translation[2]);
     if(!node.rotation.empty() && node.rotation.size() >=4)
        nodeGrp->quaternion.set(node.rotation[0],node.rotation[1],node.rotation[2],node.rotation[3]);
     if(!node.scale.empty() && node.scale.size() >=3 )
        nodeGrp->scale.set(node.scale[0],node.scale[1],node.scale[2]);

    // mesh is geometry and other part data of current obj/node
    if (node.mesh > -1) {
        assert(node.mesh < model.meshes.size());
        parseMesh(model, model.meshes[node.mesh],nodeGrp);
    }

    // Parse child nodes.
    for (size_t i = 0; i < node.children.size(); i++) {
        assert(node.children[i] < model.nodes.size());
        nodeGrp->add(parseNode(model, model.nodes[node.children[i]]));
    }

//    glPopMatrix();
    return nodeGrp;

}

void GLTFLoader::parseMesh(tinygltf::Model &model, const tinygltf::Mesh &mesh, Group::sptr nodeGroup) {

    //// Skip curves primitive.
    // if (gCurvesMesh.find(mesh.name) != gCurvesMesh.end()) {
    //  return;
    //}

    // if (gGLProgramState.uniforms["diffuseTex"] >= 0) {
    //  glUniform1i(gGLProgramState.uniforms["diffuseTex"], 0);  // TEXTURE0
    //}

//    if (gGLProgramState.uniforms["isCurvesLoc"] >= 0) {
//        glUniform1i(gGLProgramState.uniforms["isCurvesLoc"], 0);
//    }




//    const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
//    GLBufferState state;
//    glGenBuffers(1, &state.vb);
//    glBindBuffer(bufferView.target, state.vb);
//
//
//    if (sparse_accessor < 0)
//        glBufferData(bufferView.target, bufferView.byteLength,
//                     &buffer.data.at(0) + bufferView.byteOffset,
//                     GL_STATIC_DRAW);
//    else {
//        const auto accessor = model.accessors[sparse_accessor];
//        // copy the buffer to a temporary one for sparse patching
//        unsigned char *tmp_buffer = new unsigned char[bufferView.byteLength];
//        memcpy(tmp_buffer, buffer.data.data() + bufferView.byteOffset,
//               bufferView.byteLength);
//
//
//        const size_t size_of_sparse_indices =
//                ComponentTypeByteSize(accessor.sparse.indices.componentType);
//
//        const auto &indices_buffer_view =
//                model.bufferViews[accessor.sparse.indices.bufferView];
//        const auto &indices_buffer = model.buffers[indices_buffer_view.buffer];
//
//        const auto &values_buffer_view =
//                model.bufferViews[accessor.sparse.values.bufferView];
//        const auto &values_buffer = model.buffers[values_buffer_view.buffer];
//
//        for (size_t sparse_index = 0; sparse_index < accessor.sparse.count;
//             ++sparse_index) {
//            int index = 0;
//            // std::cout << "accessor.sparse.indices.componentType = " <<
//            // accessor.sparse.indices.componentType << std::endl;
//            switch (accessor.sparse.indices.componentType) {
//                case TINYGLTF_COMPONENT_TYPE_BYTE:
//                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
//                    index = (int)*(
//                            unsigned char *)(indices_buffer.data.data() +
//                                             indices_buffer_view.byteOffset +
//                                             accessor.sparse.indices.byteOffset +
//                                             (sparse_index * size_of_sparse_indices));
//                    break;
//                case TINYGLTF_COMPONENT_TYPE_SHORT:
//                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
//                    index = (int)*(
//                            unsigned short *)(indices_buffer.data.data() +
//                                              indices_buffer_view.byteOffset +
//                                              accessor.sparse.indices.byteOffset +
//                                              (sparse_index * size_of_sparse_indices));
//                    break;
//                case TINYGLTF_COMPONENT_TYPE_INT:
//                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
//                    index = (int)*(
//                            unsigned int *)(indices_buffer.data.data() +
//                                            indices_buffer_view.byteOffset +
//                                            accessor.sparse.indices.byteOffset +
//                                            (sparse_index * size_of_sparse_indices));
//                    break;
//            }
//            std::cout << "updating sparse data at index  : " << index
//                      << std::endl;
//            // index is now the target of the sparse index to patch in
//            const unsigned char *read_from =
//                    values_buffer.data.data() +
//                    (values_buffer_view.byteOffset +
//                     accessor.sparse.values.byteOffset) +
//                    (sparse_index * (size_of_object_in_buffer * accessor.type));
//
//            /*
//            std::cout << ((float*)read_from)[0] << "\n";
//            std::cout << ((float*)read_from)[1] << "\n";
//            std::cout << ((float*)read_from)[2] << "\n";
//            */
//
//            unsigned char *write_to =
//                    tmp_buffer + index * (size_of_object_in_buffer * accessor.type);
//
//            memcpy(write_to, read_from, size_of_object_in_buffer * accessor.type);
//        }
//
//        // debug:
//        /*for(size_t p = 0; p < bufferView.byteLength/sizeof(float); p++)
//        {
//          float* b = (float*)tmp_buffer;
//          std::cout << "modified_buffer [" << p << "] = " << b[p] << '\n';
//        }*/
//
//        glBufferData(bufferView.target, bufferView.byteLength, tmp_buffer,
//                     GL_STATIC_DRAW);
//        delete[] tmp_buffer;
//    }

    for (size_t i = 0; i < mesh.primitives.size(); i++) {
        const tinygltf::Primitive &primitive = mesh.primitives[i];

//        if (primitive.indices < 0) return;

        std::map<std::string, int>::const_iterator it(primitive.attributes.begin());
        std::map<std::string, int>::const_iterator itEnd(
                primitive.attributes.end());

        BufferGeometry::sptr geometry = BufferGeometry::create();

        int sparse_accessor = -1;
        for (; it != itEnd; it++) {
            assert(it->second >= 0);

            const tinygltf::Accessor &accessor = model.accessors[it->second];
            if (accessor.sparse.isSparse)
                sparse_accessor = it->second;
            else
                sparse_accessor = -1;

            if (accessor.bufferView > -1) {
                //bufferView是针对gpu buffer data中的对应数据记录，本地读取使用accessor中的数据即可
                const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
                if (bufferView.target == 0) {
                    std::cout << "WARN: bufferView.target is zero" << std::endl;
                    continue;  // Unsupported bufferView.
                }

                const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
                std::cout << "Attribute name= " << it->first << ", buffer.size= " << buffer.data.size()
                          << ", byteOffset = " << bufferView.byteOffset << std::endl;

                // copy the buffer to a temporary one for sparse patching
                unsigned char *tmp_buffer = new unsigned char[bufferView.byteLength];
                memcpy(tmp_buffer, buffer.data.data() + bufferView.byteOffset + accessor.byteOffset,
                       bufferView.byteLength);
//            std::vector<unsigned char> temp_buffer(bufferView.byteLength);
//            std::copy(buffer.data.begin()+bufferView.byteOffset,buffer.data.begin()+bufferView.byteOffset+bufferView.byteLength,temp_buffer.begin());
                const size_t size_of_object_in_buffer = ComponentTypeByteSize(accessor.componentType);

                if (sparse_accessor > 0) {

                    const size_t size_of_sparse_indices = ComponentTypeByteSize(accessor.sparse.indices.componentType);

                    const auto &indices_buffer_view =
                            model.bufferViews[accessor.sparse.indices.bufferView];
                    const auto &indices_buffer = model.buffers[indices_buffer_view.buffer];

                    const auto &values_buffer_view =
                            model.bufferViews[accessor.sparse.values.bufferView];
                    const auto &values_buffer = model.buffers[values_buffer_view.buffer];

                    for (size_t sparse_index = 0; sparse_index < accessor.sparse.count;
                         ++sparse_index) {
                        int index = 0;
                        // std::cout << "accessor.sparse.indices.componentType = " <<
                        // accessor.sparse.indices.componentType << std::endl;
                        switch (accessor.sparse.indices.componentType) {
                            case TINYGLTF_COMPONENT_TYPE_BYTE:
                            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                                index = (int) *(
                                        unsigned char *) (indices_buffer.data.data() +
                                                          indices_buffer_view.byteOffset +
                                                          accessor.sparse.indices.byteOffset +
                                                          (sparse_index * size_of_sparse_indices));
                                break;
                            case TINYGLTF_COMPONENT_TYPE_SHORT:
                            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                                index = (int) *(
                                        unsigned short *) (indices_buffer.data.data() +
                                                           indices_buffer_view.byteOffset +
                                                           accessor.sparse.indices.byteOffset +
                                                           (sparse_index * size_of_sparse_indices));
                                break;
                            case TINYGLTF_COMPONENT_TYPE_INT:
                            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                                index = (int) *(
                                        unsigned int *) (indices_buffer.data.data() +
                                                         indices_buffer_view.byteOffset +
                                                         accessor.sparse.indices.byteOffset +
                                                         (sparse_index * size_of_sparse_indices));
                                break;
                        }
                        std::cout << "updating sparse data at index  : " << index
                                  << std::endl;
                        // index is now the target of the sparse index to patch in
                        const unsigned char *read_from =
                                values_buffer.data.data() +
                                (values_buffer_view.byteOffset +
                                 accessor.sparse.values.byteOffset) +
                                (sparse_index * (size_of_object_in_buffer * accessor.type));

                        /*
                        std::cout << ((float*)read_from)[0] << "\n";
                        std::cout << ((float*)read_from)[1] << "\n";
                        std::cout << ((float*)read_from)[2] << "\n";
                        */

//                    unsigned char *write_to =
//                            tmp_buffer + index * (size_of_object_in_buffer * accessor.type);
//
//                    memcpy(write_to, read_from, size_of_object_in_buffer * accessor.type);
                    }
                }

//            glBindBuffer(GL_ARRAY_BUFFER, gBufferState[accessor.bufferView].vb);
//            CheckErrors("bind buffer");
                int size = 1;
                if (accessor.type == TINYGLTF_TYPE_SCALAR) {
                    size = 1;
                } else if (accessor.type == TINYGLTF_TYPE_VEC2) {
                    size = 2;
                } else if (accessor.type == TINYGLTF_TYPE_VEC3) {
                    size = 3;
                } else if (accessor.type == TINYGLTF_TYPE_VEC4) {
                    size = 4;
                } else {
                    assert(0);
                }

                // debug:
                for (size_t p = 0; p < accessor.count * size; p++) {
                    float *b = (float *) tmp_buffer;
                    std::cout << "modified_buffer [" << p << "] = " << b[p] << '\n';
                }

                BufferAttribute<float>::sptr bufferAttr = BufferAttribute<float>::create((float *) tmp_buffer,
                                                                                         accessor.count * size, size);
//            bufferAttr->copyArray((float*)tmp_buffer,accessor.count * size);
//            bufferAttr->itemSize = size;

                if ((it->first.compare("POSITION") == 0)) {
                    geometry->setAttribute(AttributeName::position, bufferAttr);
                }
                if ((it->first.compare("NORMAL") == 0)) {
                    geometry->setAttribute(AttributeName::normal, bufferAttr);
                }
                if ((it->first.compare("TEXCOORD_0") == 0)) {
                    geometry->setAttribute(AttributeName::uv, bufferAttr);
                }

                delete[] tmp_buffer;

//            int byteStride = accessor.ByteStride(bufferView);

//
//            BufferAttribute<float>::sptr bufferAttr = BufferAttribute<float>::create();
//            bufferAttr->copyArrayAt(BUFFER_OFFSET(accessor.byteOffset),byteStride,size);
//            bufferAttr->size = size;
//
//
//
//            if((it->first.compare("POSITION") == 0)) {
//                geometry->setAttribute(AttributeName::position,bufferAttr);
//            }
//            if((it->first.compare("NORMAL") == 0)) {
//                geometry->setAttribute(AttributeName::normal,bufferAttr);
//            }
//            if((it->first.compare("TEXCOORD_0") == 0)) {
//                geometry->setAttribute(AttributeName::uv,bufferAttr);
//            }

            }
        }

        if (primitive.indices > -1) {
            const tinygltf::Accessor &indexAccessor = model.accessors[primitive.indices];

            const size_t size_of_indices = ComponentTypeByteSize(indexAccessor.componentType);

            if (indexAccessor.bufferView > -1) {
                const tinygltf::BufferView &indicesBufferView = model.bufferViews[indexAccessor.bufferView];
                if (indicesBufferView.target == 0) {
                    std::cout << "WARN: IndexBufferView.target is zero" << std::endl;
                    continue;  // Unsupported bufferView.
                }

                const tinygltf::Buffer &indices_buffer = model.buffers[indicesBufferView.buffer];
                std::cout << "indices_buffer.size= " << indices_buffer.data.size()
                          << ", byteOffset = " << indicesBufferView.byteOffset << std::endl;

                // copy the buffer to a temporary one for sparse patching
                unsigned char *tmp_buffer = new unsigned char[indicesBufferView.byteLength];
                memcpy(tmp_buffer, indices_buffer.data.data() + indicesBufferView.byteOffset + indexAccessor.byteOffset,
                       indicesBufferView.byteLength);

//        std::vector<unsigned> indices{};
//        BufferAttribute<unsigned>::sptr indexAttr = BufferAttribute<unsigned>::create();
                std::vector<unsigned> indices_{};
                for (int i = 0; i < indexAccessor.count; i++) {
                    unsigned short *indexArr = (unsigned short *) tmp_buffer;
                    indices_.push_back((unsigned) indexArr[i]);
                }
                BufferAttribute<unsigned>::sptr indexAttr = BufferAttribute<unsigned>::create(indices_, 1);

//        BufferAttribute<unsigned>::sptr indexAttr = BufferAttribute<unsigned>::create(indices,1);
//        indexAttr->copyArray((unsigned short *)tmp_buffer,indicesBufferView.byteLength/size_of_indices);
                indexAttr->itemSize = 1;
                geometry->setIndex(indexAttr);
                delete[] tmp_buffer;
            }
        }

        // Material
//        tinygltf::Material &mat = model.materials[primitive.material];
//        printf("material.name = %s\n", mat.name.c_str());
//        auto diffuseTexId = mat.pbrMetallicRoughness.baseColorTexture.index;
//        if (mat.values.find("baseColorTexture") != mat.values.end()) {
//            std::string diffuseTexName = mat.values["baseColorTexture"].string_value;
////            if (model.textures.find(diffuseTexName) != model.textures.end()) {
////                tinygltf::Texture &tex = model.textures[diffuseTexName];
////                if (model.images.find(tex.source) != model.images.end()) {
////                    tinygltf::Image &image = model.images[tex.source];
////                }
////            }
//        }

        // Assume TEXTURE_2D target for the texture object.
        // glBindTexture(GL_TEXTURE_2D, gMeshState[mesh.name].diffuseTex[i]);
        Material::sptr material;
        if(primitive.material > -1)
            material = pMaterials[primitive.material];
//        else
//            material->color = Color(0xFF0000);

        nodeGroup->geometry = geometry;
        nodeGroup->material = material;
        //Mesh::sptr primMesh = Mesh::create(geometry,material);
        //nodeGroup->add(primMesh);
    }


}

void GLTFLoader::buildImages(const tinygltf::Model &model) {
    for (auto& gltfImage : model.images) {
        Texture::sptr pDstTexture = Texture::create();
        {
            pDstTexture->image->width = gltfImage.width;
            pDstTexture->image->height = gltfImage.height;
            pDstTexture->image->channels = gltfImage.component;
            pDstTexture->image->imageData = gltfImage.image;
            switch (gltfImage.component) {
                case 4:
                    pDstTexture->format = PixelFormat::RGBAFormat;
                    break;
                case 3:
                    pDstTexture->format = PixelFormat::RGBFormat;
                    break;
                case 2:
                    pDstTexture->format = PixelFormat::RGFormat;
                    break;
                case 1:
                    pDstTexture->format = PixelFormat::RedFormat;
                    break;
                default:
                    break;

            };

            pDstTexture->type = TextureDataType::UnsignedByteType;

//            pDstTexture->wrapS = Wrapping::ClampToEdgeWrapping;
//            pDstTexture->wrapT = Wrapping::ClampToEdgeWrapping;
//            pDstTexture->wrapR = Wrapping::ClampToEdgeWrapping;
            pDstTexture->setNeedsUpdate(true);

            pTextures.push_back(pDstTexture);
        }
    }
}

void GLTFLoader::buildTextures(const tinygltf::Model &model) {
    for (auto& gltfTexture : model.textures) {
        Texture::sptr pDstTexture = Texture::create();
        {
            auto& gltfImage = model.images[gltfTexture.source];
            pDstTexture->image->width = gltfImage.width;
            pDstTexture->image->height = gltfImage.height;
            pDstTexture->image->channels = gltfImage.component;
            pDstTexture->image->imageData = gltfImage.image;
            switch (gltfImage.component) {
                case 4:
                    pDstTexture->format = PixelFormat::RGBAFormat;
                    break;
                case 3:
                    pDstTexture->format = PixelFormat::RGBFormat;
                    break;
                case 2:
                    pDstTexture->format = PixelFormat::RGFormat;
                    break;
                case 1:
                    pDstTexture->format = PixelFormat::RedFormat;
                    break;
                default:
                    break;

            };

            pDstTexture->type = TextureDataType::UnsignedByteType;

            auto& gltfSampler = model.samplers[gltfTexture.sampler];
            switch (gltfSampler.minFilter) {
                case TINYGLTF_TEXTURE_FILTER_NEAREST:
                    if (gltfSampler.magFilter == TINYGLTF_TEXTURE_FILTER_NEAREST) {
                        pDstTexture->magFilter = TextureFilter::NearestFilter;
                        pDstTexture->minFilter = TextureFilter::NearestFilter;
                    }else {
                        pDstTexture->magFilter = TextureFilter::LinearFilter;
                        pDstTexture->minFilter = TextureFilter::LinearFilter;
                    }
                    break;
                case TINYGLTF_TEXTURE_FILTER_LINEAR:
                    pDstTexture->magFilter = TextureFilter::LinearFilter;
                    pDstTexture->minFilter = TextureFilter::LinearFilter;
                    break;
                case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
                    //samplerDesc.Filter = D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
                    pDstTexture->magFilter = TextureFilter::NearestMipmapNearestFilter;
                    pDstTexture->minFilter = TextureFilter::NearestMipmapNearestFilter;
                    break;
                case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:
                    pDstTexture->magFilter = TextureFilter::LinearMipmapNearestFilter;
                    pDstTexture->minFilter = TextureFilter::LinearMipmapNearestFilter;
                    break;
                case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
                    pDstTexture->magFilter = TextureFilter::NearestMipmapLinearFilter;
                    pDstTexture->minFilter = TextureFilter::NearestMipmapLinearFilter;
                    break;
                case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
                    pDstTexture->magFilter = TextureFilter::LinearMipmapLinearFilter;
                    pDstTexture->minFilter = TextureFilter::LinearMipmapLinearFilter;
                    break;
                default:
                    pDstTexture->magFilter = TextureFilter::LinearFilter;
                    pDstTexture->minFilter = TextureFilter::LinearFilter;
                    break;
            }

            auto toTextureAddressMode = [](int wrap) {
                switch (wrap) {
                    case TINYGLTF_TEXTURE_WRAP_REPEAT:
                        return Wrapping::RepeatWrapping;
                    case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
                        return Wrapping::ClampToEdgeWrapping;
                    case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
                        return Wrapping::MirroredRepeatWrapping;
                    default:
                        assert(false);
                        return Wrapping::RepeatWrapping;
                }
            };
            pDstTexture->wrapS = toTextureAddressMode(gltfSampler.wrapS);
            pDstTexture->wrapT = toTextureAddressMode(gltfSampler.wrapT);
//            pDstTexture->wrapR = Wrapping::ClampToEdgeWrapping;
            pDstTexture->setNeedsUpdate(true);

            pTextures.push_back(pDstTexture);
        }
    }
}


void GLTFLoader::buildMaterials(const tinygltf::Model &model) {
    for (auto& gltfMaterial : model.materials) {
        Material::sptr material = MeshStandardMaterial::create();
        material->name = gltfMaterial.name;
        {
            auto& glTFPBRMetallicRoughness = gltfMaterial.pbrMetallicRoughness;
            auto& baseColorFactor = glTFPBRMetallicRoughness.baseColorFactor;
            if(!baseColorFactor.empty() && baseColorFactor.size()>3)
                material->color = Color(baseColorFactor[0],baseColorFactor[1],baseColorFactor[2]);
            material->metalness = glTFPBRMetallicRoughness.metallicFactor;
            material->roughness = glTFPBRMetallicRoughness.roughnessFactor;

            auto& glTFBaseColorTexture = glTFPBRMetallicRoughness.baseColorTexture;
            if(glTFBaseColorTexture.index > -1)
                material->map = pTextures[glTFBaseColorTexture.index];
            auto& glTFMetallicRoughnessTexture = glTFPBRMetallicRoughness.metallicRoughnessTexture;
            if(glTFMetallicRoughnessTexture.index > -1) {
                material->roughnessMap = pTextures[glTFMetallicRoughnessTexture.index];
                material->metalnessMap = pTextures[glTFMetallicRoughnessTexture.index];
            }

            if (gltfMaterial.doubleSided) {
                material->side = Side::DoubleSide;
            } else {
                //material->side = Side::FrontSide;
            }
//            if ( materialDef.doubleSided === true ) {
//
//                materialParams.side = THREE.DoubleSide;
//
//            }

            if (gltfMaterial.alphaMode == "BLEND") {
                material->transparent = true;
                material->depthWrite = false;
            } else if (gltfMaterial.alphaMode == "MASK") {
                //assert(false);
                material->alphaTest = gltfMaterial.alphaCutoff;
            } else {
                material->transparent = false;
            }
//            const alphaMode = materialDef.alphaMode || ALPHA_MODES.OPAQUE;
//
//            if ( alphaMode === ALPHA_MODES.BLEND ) {
//
//                materialParams.transparent = true; // See: https://github.com/mrdoob/three.js/issues/17706
//
//                materialParams.depthWrite = false;
//
//            } else {
//
//                materialParams.transparent = false;
//
//                if ( alphaMode === ALPHA_MODES.MASK ) {
//
//                    materialParams.alphaTest = materialDef.alphaCutoff !== undefined ? materialDef.alphaCutoff : 0.5;
//
//                }
//
//            }

            if(gltfMaterial.normalTexture.index > -1){
                material->normalMap = pTextures[gltfMaterial.normalTexture.index];
                material->normalScale = Vector2(gltfMaterial.normalTexture.scale,gltfMaterial.normalTexture.scale);
            }
//            if ( materialDef.normalTexture !== undefined && materialType !== THREE.MeshBasicMaterial ) {
//
//                pending.push( parser.assignTexture( materialParams, 'normalMap', materialDef.normalTexture ) );
//                materialParams.normalScale = new THREE.Vector2( 1, 1 );
//
//                if ( materialDef.normalTexture.scale !== undefined ) {
//
//                    const scale = materialDef.normalTexture.scale;
//                    materialParams.normalScale.set( scale, scale );
//
//                }
//
//            }

            if(gltfMaterial.occlusionTexture.index > -1){
                material->aoMap = pTextures[gltfMaterial.occlusionTexture.index];
                material->aoMapIntensity = gltfMaterial.occlusionTexture.strength;
            }
//            if ( materialDef.occlusionTexture !== undefined && materialType !== THREE.MeshBasicMaterial ) {
//
//                pending.push( parser.assignTexture( materialParams, 'aoMap', materialDef.occlusionTexture ) );
//
//                if ( materialDef.occlusionTexture.strength !== undefined ) {
//
//                    materialParams.aoMapIntensity = materialDef.occlusionTexture.strength;
//
//                }
//
//            }

            auto emissiveFactor = gltfMaterial.emissiveFactor;
            material->emissive = Color(emissiveFactor[0],emissiveFactor[1],emissiveFactor[2]);
//            if ( materialDef.emissiveFactor !== undefined && materialType !== THREE.MeshBasicMaterial ) {
//                materialParams.emissive = new THREE.Color().fromArray( materialDef.emissiveFactor );
//            }

            if(gltfMaterial.emissiveTexture.index > -1){
                material->emissiveMap = pTextures[gltfMaterial.emissiveTexture.index];
            }
//            if ( materialDef.emissiveTexture !== undefined && materialType !== THREE.MeshBasicMaterial ) {
//                pending.push( parser.assignTexture( materialParams, 'emissiveMap', materialDef.emissiveTexture, THREE.sRGBEncoding ) );
//            }
        };

        pMaterials.push_back(material);
    }

}