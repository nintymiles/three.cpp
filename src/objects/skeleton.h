//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_SKELETON_H
#define THREE_CPP_SKELETON_H

#include "object_3d.h"
#include "bone.h"
#include "data_texture.h"

class Skeleton : public Object3D {
public:
    using sptr = std::shared_ptr<Skeleton>;

    bool useVertexTexture = false;

    std::vector<Bone::sptr> bones;

    std::vector<unsigned char> boneMatrices;

    DataTexture::sptr boneTexture = nullptr;

    int boneTextureSize = -1;

    std::vector<Matrix4> boneInverses;

    int frame = -1;

    Skeleton() : Object3D() {}

    Skeleton(const std::vector<Bone::sptr>& bones, const std::vector<Matrix4>& boneInverses);

    static sptr create() {
        return std::make_shared<Skeleton>();
    }

    static sptr create(const std::vector<Bone::sptr>& bones, const std::vector<Matrix4>& boneInverses) {
        return std::make_shared<Skeleton>(bones, boneInverses);
    }

    void calculateInverses();

    void pose();

    void update();

    Bone* getBoneByName(const std::string& name);

    void dispose();

};


#endif //THREE_CPP_SKELETON_H
