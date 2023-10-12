//
// Created by SeanR on 2023/10/12.
//

#ifndef THREE_CPP_FACE3_H
#define THREE_CPP_FACE3_H

#include "vector3.h"
#include "color.h"

class Face3 {
    static size_t _faceId;
public:
    union {
        struct { unsigned a, b, c; };
        unsigned elements[3];
    };

    size_t _id;

    Vector3 normal;

    std::vector<Vector3> vertexNormals;

    Color color;

    std::vector<Color> vertexColors;

    unsigned materialIndex;

    Face3();


    Face3(unsigned a, unsigned b, unsigned c);



    Face3(unsigned a, unsigned b, unsigned c, Vector3 normal, Color color, unsigned materialIndex=0);

    Face3(unsigned a, unsigned b, unsigned c, std::vector<Vector3> * normal, Color* color, unsigned materialIndex = 0);

    Face3(unsigned a, unsigned b, unsigned c, std::vector<Vector3>* normal, std::vector<Color> * colors, unsigned materialIndex = 0);

    Face3(const Face3& source);

    Face3& clone(Face3* target);

    Face3& copy(Face3& source);

    Vector3 __originalFaceNormal;
    std::vector<Vector3> __originalVertexNormals;


};

#endif //THREE_CPP_FACE3_H
