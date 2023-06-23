#ifndef SRC_COMMON_TYPES_H_
#define SRC_COMMON_TYPES_H_

#include <GLES3/gl3.h>

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
    unsigned u;
};

struct GLActiveInfo{
    char* name;
    GLint size;
    GLenum type;
};

struct GLVertexAttribute{
    GLenum type;
    GLint location;
    GLint locationSize;
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
    Light,DirectionalLight,SpotLight,PointLight,AmbientLight,LightProbe,HemisphereLight,RectAreaLight
};





}

#endif /* SRC_COMMON_TYPES_H_ */
