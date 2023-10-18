//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_INSTANCED_MESH_H
#define THREE_CPP_INSTANCED_MESH_H

#include "mesh.h"

class InstancedMesh : public Mesh {
public:
    BufferAttribute<float> instanceMatrix;

    int count = 0;

    using ptr = std::shared_ptr<InstancedMesh>;

    InstancedMesh() : Mesh() {
        isInstancedMesh = true;
    }

    InstancedMesh(const Geometry::sptr& geometry, const Material::sptr& material, int count) : Mesh(geometry, material), count(count) {
        isInstancedMesh = true;
    }

    InstancedMesh(const Geometry::sptr& geometry, const std::initializer_list<Material::sptr>& materials, int count) :Mesh(geometry, materials), count(count) {
        isInstancedMesh = true;
    }

    InstancedMesh(const InstancedMesh& mesh) : Mesh(mesh) {
        isInstancedMesh = true;
    }
};

#endif //THREE_CPP_INSTANCED_MESH_H
