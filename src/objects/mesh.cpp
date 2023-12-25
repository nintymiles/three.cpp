//
// Created by SeanR on 2023/10/17.
//
#include "mesh.h"
#include "buffer_geometry.h"
#include "mesh_basic_material.h"

namespace mesh {
    Matrix4 _inverseMatrix;
    //var _ray = new Ray();
    Sphere _sphere;

    Vector3 _vA ;
    Vector3 _vB ;
    Vector3 _vC ;

    Vector3 _tempA ;
    Vector3 _tempB ;
    Vector3 _tempC ;

    Vector3 _morphA ;
    Vector3 _morphB ;
    Vector3 _morphC ;

    Vector2 _uvA ;
    Vector2 _uvB ;
    Vector2 _uvC ;

    Vector3 _intersectionPoint ;
    Vector3 _intersectionPointWorld ;
}
using namespace mesh;

Mesh::Mesh() : Object3D(), drawMode(0){
    /*this->geometry = BufferGeometry::create();
    this->material = MeshBasicMaterial::create();
    this->material->color.setHex(0xffffff);
    this->materials.push_back(material);

    this->updateMorphTargets();*/

    this->isMesh = true;
}

Mesh::Mesh(const Geometry::sptr geometry, const Material::sptr material) :Object3D(geometry, material), drawMode(0){
    if (this->geometry == nullptr)
        this->geometry = BufferGeometry::create();
    else
        this->geometry = geometry;

    if (this->material == nullptr) {
        this->material = MeshBasicMaterial::create();
        //this->material->color->setHex(0xffffff);
        //this->materials.push_back(material);
        this->material = material;
    }
    else {
        this->material = material;
    }

//    this->materials.push_back(this->material);
    this->isMesh = true;
}

Mesh::Mesh(const Geometry::sptr& geometry, const std::initializer_list<Material::sptr>& materials) :Object3D(geometry, materials), drawMode(0){
    this->isMesh = true;
}

Mesh::Mesh(const Geometry::sptr& geometry, const std::vector<Material::sptr>& materials) : Object3D(geometry, materials), drawMode(0){
    this->isMesh = true;
}

Mesh::Mesh(const Mesh& mesh) : Object3D(mesh), drawMode(0) {
    /*if(this->geometry==nullptr)
        this->geometry = BufferGeometry::create();

    if (this->material == nullptr) {
        this->material = MeshBasicMaterial::create();
        this->material->color->setHex(0xffffff);
        this->materials.push_back(material);
    }*/

    this->updateMorphTargets();

    this->isMesh = true;
}

void Mesh::set(const Geometry::sptr& geometry, const Material::sptr& material) {
    if (geometry != nullptr)
        this->geometry = geometry;

    if (material != nullptr) {
        this->material = material;

    }
}

void Mesh::updateMorphTargets(){
    auto geometryPtr = reinterpret_cast<BufferGeometry*>(this->geometry.get());
    if (geometryPtr != nullptr && !geometryPtr->isGeometry)
    {
        auto morphAttributes = geometryPtr->morphAttributes;
        this->morphTargetInfluences.clear();
        this->morphTargetDictionary.clear();
        int i = 0;
        for (auto& it : morphAttributes) {
            std::string key = it.first;
            std::vector<BufferAttribute<float>::sptr> bufferAttributePtr = it.second;

            this->morphTargetInfluences.push_back(0);
            this->morphTargetDictionary[key] = i++;
        }
    }
}

Mesh* Mesh::clone(bool recursive){
    return new Mesh(*this);
}

Mesh& Mesh::copy(const Mesh& source, bool recursive){
    Object3D::copy(source,recursive);

    drawMode = source.drawMode;

    return *this;
}
