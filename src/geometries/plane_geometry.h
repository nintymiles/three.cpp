//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_PLANE_GEOMETRY_H
#define THREE_CPP_PLANE_GEOMETRY_H

#include "geometry.h"
#include "buffer_geometry.h"

struct PlaneParameter {
    float width;
    float height;
    float widthSegments;
    float heightSegments;
};

class PlaneGeometry : public BufferGeometry {
protected:
    float width;
    float height;
    float widthSegments;
    float heightSegments;
public:
    using sptr = std::shared_ptr<PlaneGeometry>;
    PlaneParameter parameters;

    PlaneGeometry(float width = 0, float height = 0, float widthSegments = 0, float heightSegments = 0);

    static sptr create(float width=0, float height=0, float widthSegments = 0, float heightSegments = 0) {
        return std::make_shared<PlaneGeometry>(width, height, widthSegments, heightSegments);
    }
};

//class PlaneGeometry : public Geometry {
//protected:
//    float width;
//    float height;
//    float widthSegments;
//    float heightSegments;
//public:
//    using sptr = std::shared_ptr<PlaneGeometry>;
//    PlaneParameter parameters;
//
//    PlaneGeometry(float width = 0, float height = 0, float widthSegments = 0, float heightSegments = 0);
//
//    static sptr create(float width = 0, float height = 0, float widthSegments = 0, float heightSegments = 0) {
//        return std::make_shared<PlaneGeometry>(width, height, widthSegments, heightSegments);
//    }
//
//};


#endif //THREE_CPP_PLANE_GEOMETRY_H
