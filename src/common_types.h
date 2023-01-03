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

union GeometryValue{
    double d;
    int i;
};

union UniformValue{
    double d;
    int i;
};

struct GLViewPort
{
    float x;
    float y;
    float width;
    float height;

    GLViewPort(float x,float y,float width,float height):x(x),y(y),width(width),height(height){};
};

enum class LightType{
    Light,DirectionalLight,SpotLight,PointLight,HemisphereLight,RectAreaLight
};





}

#endif /* SRC_COMMON_TYPES_H_ */
