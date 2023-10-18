//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_SKINNED_MESH_H
#define THREE_CPP_SKINNED_MESH_H
#include "mesh.h"
#include "skeleton.h"

class SkinnedMesh : public Mesh {
public:
    using sptr = std::shared_ptr<SkinnedMesh>;

    std::string bindMode;

    Matrix4 bindMatrix;

    Matrix4 bindMatrixInverse;

    Skeleton::sptr skeleton = nullptr;

    SkinnedMesh();

    SkinnedMesh(const Geometry::sptr& geometry, const Material::sptr& material);

    SkinnedMesh(const Geometry::sptr& geometry, const std::initializer_list<Material::sptr>& materials);

    SkinnedMesh(const SkinnedMesh& mesh);
};

#endif //THREE_CPP_SKINNED_MESH_H
