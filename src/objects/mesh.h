//
// Created by SeanR on 2023/10/17.
//

#ifndef THREE_CPP_MESH_H
#define THREE_CPP_MESH_H

#include "object_3d.h"
#include "geometry.h"
#include "material.h"

class Mesh : public Object3D {
public:

    Mesh();

    Mesh(const Geometry::sptr& geometry, const Material::sptr& material=nullptr);

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
    //void raycast();

    virtual Mesh* clone(bool recursive = true);

    /**
     *
     * @param object
     * @param recursive
     */
    virtual Mesh& copy(const Mesh& source, bool recursive = true);
};

#endif //THREE_CPP_MESH_H
