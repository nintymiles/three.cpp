//
// Created by SeanR on 2023/10/15.
//

#ifndef THREE_CPP_GL_OBJECTS_H
#define THREE_CPP_GL_OBJECTS_H

#include "buffer_geometry.h"
#include "gl_info.h"
#include "gl_attributes.h"
#include "gl_geometries.h"

#include <memory>

class GLObjects {
public:
    using ptr = std::shared_ptr<GLObjects>;
    std::unordered_map<unsigned, unsigned> updateList;

    GLInfo::sptr info;

    GLAttributes::sptr attributes;

    GLGeometries::sptr geometries;

    GLObjects(GLGeometries::sptr& geometries,GLAttributes::sptr& attributes, GLInfo::sptr& info) : geometries(geometries),attributes(attributes),info(info) {}

    virtual ~GLObjects() = default;

    GLObjects& copy(const GLObjects& source) {
        updateList = source.updateList;
        attributes = source.attributes;
        geometries = source.geometries;
        info = source.info;

        return *this;
    }
    GLObjects& operator = (const GLObjects& source) {
        return copy(source);
    }

    std::shared_ptr<BufferGeometry> update(Object3D& object);
};

#endif //THREE_CPP_GL_OBJECTS_H
