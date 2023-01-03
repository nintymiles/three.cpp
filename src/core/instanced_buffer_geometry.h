//
// Created by SeanR on 2022/12/29.
//

#ifndef THREE_CPP_SRC_CORE_INSTANCED_BUFFER_GEOMETRY_H
#define THREE_CPP_SRC_CORE_INSTANCED_BUFFER_GEOMETRY_H

#include "buffer_geometry.h"
#include "number.h"

class InstancedBufferGeometry:public BufferGeometry<int,double>{
public:
    std::string type = "InstancedBufferGeometry";
    int instanceCount = Number::MAX_INT;
    bool isInstancedBufferGeometry = true;

    InstancedBufferGeometry()=default;

    //InstancedBufferGeometry(const InstancedBufferGeometry& ibg):instanceCount(ibg.instanceCount){}
};

//class InstancedBufferGeometry extends BufferGeometry {
//
//constructor() {
//
//    super();
//
//    this.type = 'InstancedBufferGeometry';
//    this.instanceCount = Infinity;
//
//}
//
//copy( source ) {
//
//    super.copy( source );
//
//    this.instanceCount = source.instanceCount;
//
//    return this;
//
//}
//
//clone() {
//
//    return new this.constructor().copy( this );
//
//}
//
//toJSON() {
//
//    const data = super.toJSON( this );
//
//    data.instanceCount = this.instanceCount;
//
//    data.isInstancedBufferGeometry = true;
//
//    return data;
//
//}
//
//}
//
//InstancedBufferGeometry.prototype.isInstancedBufferGeometry = true;
//
//export { InstancedBufferGeometry };

#endif //THREE_CPP_SRC_CORE_INSTANCED_BUFFER_GEOMETRY_H
