//
// Created by SeanR on 2022/12/29.
//

#ifndef THREE_CPP_SRC_CORE_INSTANCED_BUFFER_GEOMETRY_H
#define THREE_CPP_SRC_CORE_INSTANCED_BUFFER_GEOMETRY_H

#include "buffer_geometry.h"
#include "number.h"

struct InstancedGroup {
    int start;
    int count;
    int instances;

    InstancedGroup(int start, int count, int instances) : start(start), count(count), instances(instances) {}
};

class InstancedBufferGeometry : public BufferGeometry {
public:
    using ptr = std::shared_ptr<InstancedBufferGeometry>;

    std::vector<InstancedGroup> groups;

    int maxInstancedCount=-1;

    InstancedBufferGeometry() : BufferGeometry(),maxInstancedCount(-1) {}

    InstancedBufferGeometry(const InstancedBufferGeometry& source) : BufferGeometry(source) {
        if (source.groups.size() > 0) {
            groups.reserve(source.groups.size());
            std::copy(source.groups.begin(), source.groups.end(), groups.begin());
        }
        maxInstancedCount = source.maxInstancedCount;
    }

    void addGroup(int start, int count, int instances) {
        groups.push_back(InstancedGroup(start, count, instances));
    }

    InstancedBufferGeometry* clone() {
        return new InstancedBufferGeometry(*this);
    }

    InstancedBufferGeometry& copy(const InstancedBufferGeometry& source) {
        BufferGeometry::copy(source);

        if (source.groups.size() > 0) {
            groups.reserve(source.groups.size());
            std::copy(source.groups.begin(), source.groups.end(), groups.begin());
        }

        maxInstancedCount = source.maxInstancedCount;

        return *this;
    }
};

//class InstancedBufferGeometry:public BufferGeometry<int,double>{
//public:
//    std::string type = "InstancedBufferGeometry";
//    int instanceCount = Number::MAX_INT;
//    bool isInstancedBufferGeometry = true;
//
//    InstancedBufferGeometry()=default;
//
//    //InstancedBufferGeometry(const InstancedBufferGeometry& ibg):instanceCount(ibg.instanceCount){}
//};
//
////class InstancedBufferGeometry extends BufferGeometry {
////
////constructor() {
////
////    super();
////
////    this.type = 'InstancedBufferGeometry';
////    this.instanceCount = Infinity;
////
////}
////
////copy( source ) {
////
////    super.copy( source );
////
////    this.instanceCount = source.instanceCount;
////
////    return this;
////
////}
////
////clone() {
////
////    return new this.constructor().copy( this );
////
////}
////
////toJSON() {
////
////    const data = super.toJSON( this );
////
////    data.instanceCount = this.instanceCount;
////
////    data.isInstancedBufferGeometry = true;
////
////    return data;
////
////}
////
////}
////
////InstancedBufferGeometry.prototype.isInstancedBufferGeometry = true;
////
////export { InstancedBufferGeometry };

#endif //THREE_CPP_SRC_CORE_INSTANCED_BUFFER_GEOMETRY_H
