//
// Created by SeanR on 2023/10/15.
//
#ifndef THREE_CPP_GL_GEOMETRIES_H
#define THREE_CPP_GL_GEOMETRIES_H

#include "buffer_geometry.h"
#include "buffer_attribute.h"
#include "gl_attributes.h"
#include "gl_info.h"

#include <unordered_map>

class GLGeometries {
private:
    struct GeometryInfo {
        BufferGeometry::sptr bufferGeometry;
        BufferGeometry::OnDispose::ConnectionId connectionId;
    };

    std::unordered_map<unsigned,GeometryInfo> geometries;

    std::unordered_map<unsigned,BufferAttribute<unsigned>::sptr>wireframeAttributes;

    GLAttributes::sptr attributes;

    GLInfo::sptr info;

    void onGeometryDispose(Geometry* geometry);

    void updateWireframeAttribute(BufferGeometry& geometry);

public:
    using sptr = std::shared_ptr<GLGeometries>;


    GLGeometries(GLAttributes::sptr& attributes, GLInfo::sptr& info) : attributes(attributes), info(info) {}

    std::shared_ptr<BufferGeometry> get(Object3D& object, std::shared_ptr<Geometry>& geometry);

    void update(const std::shared_ptr<BufferGeometry>& geometry);

    std::shared_ptr<BufferAttribute<unsigned>> getWireframeAttribute(const std::shared_ptr<BufferGeometry>& geometry);


    GLGeometries& copy(const GLGeometries& source) {
        attributes = source.attributes;
        info = source.info;
        geometries = source.geometries;
        wireframeAttributes = source.wireframeAttributes;
        return *this;
    }
    GLGeometries& operator = (const GLGeometries& source) {
        return copy(source);
    }
};

#endif //THREE_CPP_GL_GEOMETRIES_H
