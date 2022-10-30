#ifndef SRC_COMMON_TYPES_H_
#define SRC_COMMON_TYPES_H_

namespace threecpp {

struct Range{
    int start;
    int count;

    //Range(const Range& r):start(r.start),count(r.count){};
};


struct GeometryGroup{
    int start;
    int count;
    int materialIndex;
};

union UniformValue{
    int i;
    double d;
};





}

#endif /* SRC_COMMON_TYPES_H_ */
