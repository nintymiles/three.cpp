//
// Created by SeanR on 2023/10/11.
//

#ifndef THREE_CPP_GL_CLIPPING_H
#define THREE_CPP_GL_CLIPPING_H

#include "camera.h"
#include "plane.h"
#include "gl_properties.h"
#include "material.h"

#include "matrix3.h"

class MaterialProperties;
class GLClipping {
private:
    int numGlobalPlanes = 0;
    bool localClippingEnabled = false;
    bool renderingShadows = false;
    Plane plane;
    Matrix3 viewNormalMatrix;
    std::vector<float> globalState;

    bool needUpdate = false;
    std::vector<float> projectPlanes(std::vector<Plane>& planes,const Camera::sptr& camera=nullptr,int dstOffset=std::numeric_limits<int>::quiet_NaN(),bool skipTransform=true);

    void resetGlobalState();
public:
    using sptr = std::shared_ptr<GLClipping>;

    int numPlanes=0;
    int numIntersection = 0;
    std::vector<float> uniform;


    GLClipping() {}

    bool init(std::vector<Plane>& planes, bool enableLocalClipping, const Camera::sptr& camera);

    void beginShadows();

    void endShadows();

    void setState(std::vector<Plane>& planes, bool clipIntersection, bool clipShadows, const Camera::sptr& camera, MaterialProperties& cache, bool fromCache);

};

#endif //THREE_CPP_GL_CLIPPING_H
