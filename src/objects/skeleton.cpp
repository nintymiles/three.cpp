//
// Created by SeanR on 2023/10/18.
//

#include "skeleton.h"

namespace skeleton_cpp{
    auto _offsetMatrix = Matrix4();
    auto _identityMatrix = Matrix4();
}

using namespace skeleton_cpp;

Skeleton::Skeleton(const std::vector<Bone::sptr>& bones, const std::vector<Matrix4>& boneInverses){
    //std::copy(bones.begin(), bones.end(), this->bones.begin());
    this->bones=bones;
    this->boneMatrices = std::vector<float>(bones.size() * 16);

    if (boneInverses.size() == 0) {
        calculateInverses();
    }
    else {
        if (this->bones.size() == boneInverses.size()) {
            this->boneInverses = boneInverses;
        }
        else {
            for (int i = 0;i < this->bones.size();i++)
                this->boneInverses.push_back(Matrix4());
        }
    }
}

void Skeleton::calculateInverses(){
    if (this->boneInverses.size() > 0) {
        boneInverses.clear();
        //std::vector<Matrix4>().swap(this->boneInverses);
    }

    for (int i = 0;this->bones.size(); i++) {

        auto inverse = Matrix4();

        inverse.getInverse(this->bones[i]->matrixWorld);


        boneInverses.push_back(inverse);

    }
}

void Skeleton::pose(){
    // recover the bind-time world matrices
    for ( size_t i = 0, il = this->bones.size(); i < il; i ++ ) {
        auto bone = this->bones[ i ];

        if ( bone ) {

            bone->matrixWorld.copy( this->boneInverses[ i ] ).invert();

        }

    }

    // compute the local matrices, positions, rotations and scales
    for ( size_t i = 0, il = this->bones.size(); i < il; i ++ ) {
        auto bone = this->bones[ i ];

        if ( bone ) {
            if ( bone->parent && bone->parent->isBone ) {
                bone->matrix.copy( bone->parent->matrixWorld ).invert();
                bone->matrix.multiply( bone->matrixWorld );

            } else {
                bone->matrix.copy( bone->matrixWorld );

            }

            bone->matrix.decompose( &bone->position, &bone->quaternion, &bone->scale );

        }

    }
}

void Skeleton::update(){
    auto bones = this->bones;
    auto boneInverses = this->boneInverses;
    auto boneMatrices = this->boneMatrices;
    auto boneTexture = this->boneTexture;

    // flatten bone matrices to array
    for ( size_t i = 0, il = bones.size(); i < il; i ++ ) {

        // compute the offset between the current and the original transform
        auto matrix = bones[ i ] ? bones[ i ]->matrixWorld : _identityMatrix;

        _offsetMatrix.multiplyMatrices( matrix, boneInverses[ i ] );
        _offsetMatrix.toArray( boneMatrices.data(), i * 16 );

    }

    if ( boneTexture != nullptr ) {
        boneTexture->needsUpdate = true;
    }

}

Bone* Skeleton::getBoneByName(const std::string& name){
    return nullptr;
}

void Skeleton::dispose(){}