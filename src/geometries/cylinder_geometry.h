//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_CYLINDER_GEOMETRY_H
#define THREE_CPP_CYLINDER_GEOMETRY_H

#include "buffer_geometry.h"
#include "buffer_attribute.h"
#include <limits>

struct CylinderParameters {
    float radiusTop;

    float radiusBottom;

    float height;

    float radialSegments;

    float heightSegments;

    bool openEnded;

    float thetaStart;

    float thetaLength;

};

class CylinderBufferGeometry : public BufferGeometry{
private:
    std::vector<unsigned> _indices;

    std::vector<float> _vertices;

    std::vector<float> _normals;

    std::vector<float> _uvs;

    std::vector<std::vector<int>> _indexArray;

    float halfHeight;

    int groupStart = 0;

    int index = 0;

    void generateTorso();
    void generateCap(bool top);
public:
    float radiusTop;

    float radiusBottom;

    float height;

    float radialSegments;

    float heightSegments;

    bool openEnded;

    float thetaStart;

    float thetaLength;

    CylinderParameters parameters;

    using sptr = std::shared_ptr<CylinderBufferGeometry>;

    CylinderBufferGeometry(float radiusTop, float radiusBottom, float height,
                           float radialSegments = std::numeric_limits<float>::quiet_NaN(),
                           float heightSegments = std::numeric_limits<float>::quiet_NaN(),
                           bool openEnded = false,
                           float thetaStart = std::numeric_limits<float>::quiet_NaN(),
                           float thetaLength = std::numeric_limits<float>::quiet_NaN());

};

class CylinderGeometry : public Geometry{
public:
    float radiusTop;

    float radiusBottom;

    float height;

    float radialSegments;

    float heightSegments;

    bool openEnded;

    float thetaStart;

    float thetaLength;

    CylinderParameters parameters;

    using sptr = std::shared_ptr<CylinderGeometry>;
    CylinderGeometry(float radiusTop, float radiusBottom, float height,
                     float radialSegments = std::numeric_limits<float>::quiet_NaN(),
                     float heightSegments = std::numeric_limits<float>::quiet_NaN(),
                     bool openEnded = false,
                     float thetaStart = std::numeric_limits<float>::quiet_NaN(),
                     float thetaLength = std::numeric_limits<float>::quiet_NaN());
};

#endif //THREE_CPP_CYLINDER_GEOMETRY_H
