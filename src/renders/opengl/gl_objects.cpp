//
// Created by SeanR on 2023/10/22.
//
#include "gl_objects.h"

#include "instanced_mesh.h"

std::shared_ptr<BufferGeometry> GLObjects::update(Object3D& object){
    auto frame = info->render.frame;
    auto geometry = object.geometry;
    auto bufferGeometry = geometries->get(object, geometry);

    if (updateList.count(bufferGeometry->id) == 0 || updateList[bufferGeometry->id] != frame) {
        if (geometry->isGeometry) {
            bufferGeometry->updateFromObject(object);
        }
        geometries->update(bufferGeometry);

        if (updateList.count(bufferGeometry->id) == 0) {
            updateList.insert({bufferGeometry->id, frame});
        } else {
            updateList[bufferGeometry->id] = frame;
        }
    }

    if (object.isInstancedMesh) {
        InstancedMesh* instanceMesh = static_cast<InstancedMesh*>(&object);
        attributes->update<float>(instanceMesh->instanceMatrix, GL_ARRAY_BUFFER);
    }

    return bufferGeometry;
}
