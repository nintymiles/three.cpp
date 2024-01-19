//
// Created by SeanR on 2023/10/17.
//
#include "mesh.h"
#include "buffer_geometry.h"
#include "mesh_basic_material.h"

#include "vector2.h"

#include "ray.h"
#include "ray_caster.h"
#include "triangle.h"

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

    Ray _ray;
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

Vector3& Mesh::getVertexPosition(size_t index, Vector3& target) {

    auto geometry = this->geometry;
    BufferGeometry::sptr bufferGeometry = std::dynamic_pointer_cast<BufferGeometry>(geometry);
    auto position = bufferGeometry->getAttribute(AttributeName::position);
    auto morphPosition = bufferGeometry->morphAttributes["position"];
    auto morphTargetsRelative = bufferGeometry->morphTargetsRelative;

    target.fromBufferAttribute( *position, index );

    auto morphInfluences = this->morphTargetInfluences;

    if ( !morphPosition.empty() && !morphInfluences.empty() ) {

        _morphA.set( 0, 0, 0 );

        for ( size_t i = 0, il = morphPosition.size(); i < il; i ++ ) {

            auto influence = morphInfluences[ i ];
            auto morphAttribute = morphPosition[ i ];

            if ( influence == 0 ) continue;

            _tempA.fromBufferAttribute( *morphAttribute, index );

            if ( morphTargetsRelative ) {
                _morphA.addScaledVector( _tempA, influence );
            } else {
                _morphA.addScaledVector( _tempA.sub( target ), influence );
            }

        }

        target.add( _morphA );

    }

//    if ( this->isSkinnedMesh ) {
//        this->boneTransform( index, target );
//    }

    return target;

}

Object3D& Mesh::raycast(Raycaster& raycaster, std::vector<std::shared_ptr<IntersectionData>>& intersects) {

    auto geometry = this->geometry;
    auto material = this->material;
    auto& matrixWorld = this->matrixWorld;

    if ( !material ) return *this;

    // Checking boundingSphere distance to ray
    if ( !(geometry->boundingSphere.isEmpty()) ) geometry->computeBoundingSphere();

    _sphere.copy( geometry->boundingSphere );
    _sphere.applyMatrix4( matrixWorld );

    if ( raycaster.ray.intersectsSphere( _sphere ) == false ) return *this;

    //
    _inverseMatrix.copy( matrixWorld ).invert();
    _ray.copy( raycaster.ray ).applyMatrix4( _inverseMatrix );

    // Check boundingBox before continuing

    if ( !geometry->boundingBox.isEmpty() ) {
        if ( _ray.intersectsBox( geometry->boundingBox ) == false ) return *this;
    }

    std::shared_ptr<IntersectionData> intersection;

    BufferGeometry::sptr bufferGeometry = std::dynamic_pointer_cast<BufferGeometry>(geometry);
    auto index = bufferGeometry->index;
    auto position = bufferGeometry->getAttribute(AttributeName::position);
    auto uv = bufferGeometry->getAttribute(AttributeName::uv);
    auto uv2 = bufferGeometry->getAttribute(AttributeName::uv2);
    auto groups = bufferGeometry->groups;
    auto drawRange = bufferGeometry->drawRange;

    if ( index ) {

        // indexed buffer geometry
        if ( this->materials.size()>1 ) {

            for ( size_t i = 0, il = groups.size(); i < il; i ++ ) {

                auto group = groups[ i ];
                auto groupMaterial = this->materials[ group.materialIndex ];

                auto start = math::max( group.start, drawRange.start );
                auto end = math::min( index->count, math::min( ( group.start + group.count ), ( drawRange.start + drawRange.count ) ) );

                for ( size_t j = start, jl = end; j < jl; j += 3 ) {

                    auto a = index->getX( j );
                    auto b = index->getX( j + 1 );
                    auto c = index->getX( j + 2 );

                    intersection = checkBufferGeometryIntersection( shared_from_this(), groupMaterial, raycaster, _ray, uv, uv2, a, b, c );

                    if ( intersection ) {

                        intersection->faceIndex = math::floor( j / 3 ); // triangle number in indexed buffer semantics
                        intersection->face.materialIndex = group.materialIndex;
                        intersects.push_back( intersection );

                    }

                }

            }

        } else {

            auto start = math::max( (unsigned)0, drawRange.start );
            auto end = drawRange.count > 0 ? math::min( index->count, ( drawRange.start + drawRange.count ) ) : index->count;

            for ( size_t i = start, il = end; i < il; i += 3 ) {

                auto a = index->getX( i );
                auto b = index->getX( i + 1 );
                auto c = index->getX( i + 2 );

                intersection = checkBufferGeometryIntersection( shared_from_this(), material, raycaster, _ray, uv, uv2, a, b, c );

                if ( intersection ) {

                    intersection->faceIndex = math::floor( i / 3 ); // triangle number in indexed buffer semantics
                    intersects.push_back( intersection );
                }

            }

        }

    } else if ( position ) {

        // non-indexed buffer geometry
        if ( materials.size()>0 ) {

            for ( size_t i = 0, il = groups.size(); i < il; i ++ ) {

                auto group = groups[ i ];
                auto groupMaterial = materials[ group.materialIndex ];

                auto start = math::max( group.start, drawRange.start );
                auto end = math::min( position->count, math::min( ( group.start + group.count ), ( drawRange.start + drawRange.count ) ) );

                for ( size_t j = start, jl = end; j < jl; j += 3 ) {

                    auto a = j;
                    auto b = j + 1;
                    auto c = j + 2;

                    intersection = checkBufferGeometryIntersection( shared_from_this(), groupMaterial, raycaster, _ray, uv, uv2, a, b, c );

                    if ( intersection ) {

                        intersection->faceIndex = math::floor( j / 3 ); // triangle number in non-indexed buffer semantics
                        intersection->face.materialIndex = group.materialIndex;
                        intersects.push_back( intersection );
                    }
                }
            }

        } else {

            auto start = math::max( (unsigned)0, drawRange.start );
            auto end = math::min( position->count, ( drawRange.start + drawRange.count ) );

            for ( size_t i = start, il = end; i < il; i += 3 ) {

                auto a = i;
                auto b = i + 1;
                auto c = i + 2;

                intersection = checkBufferGeometryIntersection( shared_from_this(), material, raycaster, _ray, uv, uv2, a, b, c );

                if ( intersection ) {

                    intersection->faceIndex = math::floor( i / 3 ); // triangle number in non-indexed buffer semantics
                    intersects.push_back( intersection );
                }
            }
        }
    }

    return *this;
}

std::shared_ptr<IntersectionData> checkIntersection( Mesh::sptr object, Material::sptr material, Raycaster& raycaster, Ray& ray,
                                                     Vector3& pA, Vector3& pB, Vector3& pC, Vector3& targetPoint ) {

    bool intersected;

    if ( material->side == Side::BackSide ) {

        intersected = ray.intersectTriangle( pC, pB, pA, true, targetPoint );
    } else {

        intersected = ray.intersectTriangle( pA, pB, pC, ( material->side == Side::FrontSide ), targetPoint );
    }

    if ( !intersected ) return nullptr;

    _intersectionPointWorld.copy( targetPoint );
    _intersectionPointWorld.applyMatrix4( object->matrixWorld );

    auto distance = raycaster.ray.origin.distanceTo( _intersectionPointWorld );

    if ( distance < raycaster.pNear || distance > raycaster.pFar ) return nullptr;

    std::shared_ptr<IntersectionData> intersect = std::make_shared<IntersectionData>();

    intersect->distance = distance;
    intersect->point = _intersectionPointWorld.clone();
    intersect->object = object;

    return intersect;
}

std::shared_ptr<IntersectionData> checkBufferGeometryIntersection( Mesh::sptr object, Material::sptr material, Raycaster& raycaster,
                                                                   Ray& ray, BufferAttribute<float>::sptr uv, BufferAttribute<float>::sptr uv2, size_t a, size_t b, size_t c ) {

    object->getVertexPosition( a, _vA );
    object->getVertexPosition( b, _vB );
    object->getVertexPosition( c, _vC );

    auto intersection = checkIntersection( object, material, raycaster, ray, _vA, _vB, _vC, _intersectionPoint );

    if ( intersection ) {

        if ( uv ) {
            _uvA.set(uv->getX(a),uv->getY(a));
            _uvB.set(uv->getX(b),uv->getY(b));
            _uvC.set(uv->getX(c),uv->getY(c));
//            _uvA.fromBufferAttribute<float>( uv, a );
//            _uvB.fromBufferAttribute<float>( uv, b );
//            _uvC.fromBufferAttribute<float>( uv, c );
//            fillVector2FromBufferAttribute<float>(_uvA,uv,a);
//            fillVector2FromBufferAttribute<float>(_uvB,uv,b);
//            fillVector2FromBufferAttribute<float>(_uvC,uv,c);

            Vector2 targetUV{};
            Triangle::getUV( _intersectionPoint, _vA, _vB, _vC, _uvA, _uvB, _uvC, targetUV );
            intersection->uv = targetUV;
        }

        if ( uv2 ) {
            _uvA.set(uv2->getX(a),uv->getY(a));
            _uvB.set(uv2->getX(b),uv->getY(b));
            _uvC.set(uv2->getX(c),uv->getY(c));
//            _uvA.fromBufferAttribute<float>( uv2, a );
//            _uvB.fromBufferAttribute<float>( uv2, b );
//            _uvC.fromBufferAttribute<float>( uv2, c );

            Vector2 targetUV2{};
            Triangle::getUV( _intersectionPoint, _vA, _vB, _vC, _uvA, _uvB, _uvC, targetUV2 );
            intersection->uv2 = targetUV2;

        }

        IntersectionData::FaceData face = { a,b,c,Vector3(),0};

        Triangle::getNormal( _vA, _vB, _vC, face.normal );

        intersection->face = face;

    }

    return intersection;

}


