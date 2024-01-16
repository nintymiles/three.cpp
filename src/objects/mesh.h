//
// Created by SeanR on 2023/10/17.
//

#ifndef THREE_CPP_MESH_H
#define THREE_CPP_MESH_H

#include "object_3d.h"
#include "geometry.h"
#include "material.h"

#include <memory>

class Ray;
class Mesh : public Object3D,public std::enable_shared_from_this<Mesh> {
public:

    Mesh();

    Mesh(const Geometry::sptr geometry, const Material::sptr material=nullptr);

    Mesh(const Geometry::sptr& geometry, const std::vector<Material::sptr>& materials);

    Mesh(const Geometry::sptr& geometry, const std::initializer_list<Material::sptr>& materials);

    Mesh(const Mesh& mesh);


    using sptr = std::shared_ptr<Mesh>;
    unsigned drawMode;

    static sptr create() {
        return std::make_shared<Mesh>(Mesh());
    }
    static sptr create(const Geometry::sptr& geometry, const Material::sptr& material=nullptr) {
        return std::make_shared<Mesh>(geometry, material);
    }

    static sptr create(const Geometry::sptr& geometry, const std::initializer_list<Material::sptr>& materials) {
        return std::make_shared<Mesh>(geometry, materials);
    }
    static sptr create(const Geometry::sptr& geometry, const std::vector<Material::sptr>& materials) {
        return std::make_shared<Mesh>(geometry, materials);
    }
    void set(const Geometry::sptr& geometry, const Material::sptr& material);
    void updateMorphTargets();

    Vector3& getVertexPosition(size_t index, Vector3& target);

    virtual Mesh* clone(bool recursive = true);

    virtual Mesh& copy(const Mesh& source, bool recursive = true);

    Object3D& raycast(Raycaster& raycaster, std::vector<std::shared_ptr<IntersectionData>> &intersects) override;
};

std::shared_ptr<IntersectionData> checkIntersection( Mesh::sptr object, Material::sptr material, Raycaster& raycaster, Ray& ray,
                                                     Vector3& pA, Vector3& pB, Vector3& pC, Vector3& targetPoint );

std::shared_ptr<IntersectionData> checkBufferGeometryIntersection( Mesh::sptr object, Material::sptr material, Raycaster& raycaster,
                                                                   Ray& ray, BufferAttribute<float>::sptr uv, BufferAttribute<float>::sptr uv2, size_t a, size_t b, size_t c );

#endif //THREE_CPP_MESH_H
