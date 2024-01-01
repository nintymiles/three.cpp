//
// Created by SeanR on 2023/10/15.
//

#include "gl_geometries.h"

void GLGeometries::onGeometryDispose(Geometry* geometry){
    GeometryInfo& ginfo = geometries[geometry->id];
    BufferGeometry::sptr buffergeometry = ginfo.bufferGeometry;

    if (buffergeometry->index != nullptr) {
        attributes->remove(*buffergeometry->index);
    }
    for (auto attr = buffergeometry->attributes.begin();attr != buffergeometry->attributes.end();) {
        attributes->remove(*attr->second);
        attr++;
    }
    /*if (buffergeometry->position != nullptr) {
        attributes->remove(*buffergeometry->position);
    }
    if (buffergeometry->normal != nullptr) {
        attributes->remove(*buffergeometry->normal);
    }
    if (buffergeometry->color != nullptr) {
        attributes->remove(*buffergeometry->color);
    }
    if (buffergeometry->uv != nullptr) {
        attributes->remove(*buffergeometry->uv);
    }
    if (buffergeometry->uv2 != nullptr) {
        attributes->remove(*buffergeometry->uv2);
    }
    if (buffergeometry->tangent != nullptr) {
        attributes->remove(*buffergeometry->tangent);
    }
    if (buffergeometry->bitangent != nullptr) {
        attributes->remove(*buffergeometry->bitangent);
    }
    if (buffergeometry->lineDistance != nullptr) {
        attributes->remove(*buffergeometry->lineDistance);
    }
    if (buffergeometry->skinIndex != nullptr) {
        attributes->remove(*buffergeometry->skinIndex);
    }
    if (buffergeometry->skinWeight != nullptr) {
        attributes->remove(*buffergeometry->skinWeight);
    }*/

    geometry->onDispose.disconnect(ginfo.connectionId);

    geometries.erase(geometry->id);

    if (wireframeAttributes.find(geometry->id) != wireframeAttributes.end()) {
        auto &attribute = wireframeAttributes[buffergeometry->id];
        attributes->remove(*attribute);
        wireframeAttributes.erase(geometry->id);
    }


    info->memory.geometries--;
}

void GLGeometries::updateWireframeAttribute(BufferGeometry& geometry){
    std::vector<unsigned> index;

    auto geometryIndex = geometry.index;
    std::shared_ptr<BufferAttribute<float>> geometryPosition = geometry.getAttribute(AttributeName::position);
    unsigned version;

    if (geometryIndex != nullptr){
        auto array = geometryIndex->array;
        version = geometryIndex->version;

        //todo:refactor for loop for unsigned or size_t here
        for (int i = 0; i < (int)array.size() - 3; i += 3)
        {
            unsigned a = array[i + 0];
            unsigned b = array[i + 1];
            unsigned c = array[i + 2];

            index.push_back(a);
            index.push_back(b);
            index.push_back(b);
            index.push_back(c);
            index.push_back(c);
            index.push_back(a);
        }
    }else{
        auto array = geometryPosition->array;
        version = geometryPosition->version;

        for (int i = 0; i < (array.size() / 3 - 1); i += 3){
            auto a = i + 0;
            auto b = i + 1;
            auto c = i + 2;
            index.push_back(a);
            index.push_back(b);
            index.push_back(b);
            index.push_back(c);
            index.push_back(c);
            index.push_back(a);
        }
    }

    BufferAttribute<unsigned>::sptr attribute = BufferAttribute<unsigned>::create(index, 1);
    attribute->version = version;

    //attributes->update<unsigned>(*attribute, GL_ELEMENT_ARRAY_BUFFER);

    if (wireframeAttributes.count(geometry.id) > 0) {
        BufferAttribute<unsigned>::sptr previousAttribute = wireframeAttributes[geometry.id];
        if (previousAttribute != nullptr) attributes->remove(*previousAttribute);
    }

    wireframeAttributes.insert_or_assign(geometry.id,attribute);

}

std::shared_ptr<BufferGeometry> GLGeometries::get(Object3D& object, std::shared_ptr<Geometry>& geometry){
    GeometryInfo& ginfo = geometries[geometry->id];
    if (ginfo.bufferGeometry!=nullptr) return ginfo.bufferGeometry;

    geometry->onDispose.connect(*this, &GLGeometries::onGeometryDispose);

    BufferGeometry::sptr bufferGeometry = nullptr;

    if (!geometry->isGeometry) {
        bufferGeometry = std::dynamic_pointer_cast<BufferGeometry>(geometry);
        ginfo.bufferGeometry = bufferGeometry;
    }
    else if (geometry->isGeometry) {
        if (geometry->__bufferGeometry == nullptr) {
            geometry->__bufferGeometry = BufferGeometry::create();
            geometry->__bufferGeometry->setFromObject(object);
        }
        bufferGeometry = geometry->__bufferGeometry;
        geometry->__bufferGeometry.reset();
        ginfo.bufferGeometry = bufferGeometry;
    }

    info->memory.geometries++;

    return ginfo.bufferGeometry;
}

void GLGeometries::update(const std::shared_ptr<BufferGeometry>& geometry){
    if (geometry->index)
        attributes->update<unsigned>(*geometry->index, GL_ELEMENT_ARRAY_BUFFER);

    for (auto attr = geometry->attributes.begin();attr != geometry->attributes.end();attr++) {
        if(attr->second)
            attributes->update<float>(*attr->second,GL_ARRAY_BUFFER);
        //when attr++ puts here,you can't even break loop once by using continue
        //attr++;
    }

    auto morphAttributes = geometry->morphAttributes;
    for (auto& attribute : morphAttributes) {
        std::vector<BufferAttribute<float>::sptr> attrVector = attribute.second;
        for (unsigned i = 0;i < attrVector.size();i++) {
            attributes->update<float>(*attrVector[i], GL_ARRAY_BUFFER);
        }
    }

}


std::shared_ptr<BufferAttribute<unsigned>> GLGeometries::getWireframeAttribute(const std::shared_ptr<BufferGeometry>& geometry)
{
    auto currentAttribute = wireframeAttributes[geometry->id];

    if (currentAttribute != nullptr) {

        auto geometryIndex = geometry->index;

        if (geometryIndex != nullptr) {

            // if the attribute is obsolete, create a new one

            if (currentAttribute->version < geometryIndex->version) {
                updateWireframeAttribute(*geometry);
            }
        }
    }
    else {
        updateWireframeAttribute(*geometry);
    }

    return wireframeAttributes[geometry->id];
}
