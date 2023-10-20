//
// Created by SeanR on 2023/10/20.
//

#include "icosahedron_geometry.h"

#include <cmath>

namespace icosahedron_geometry {
    float t = (1 + (float)std::sqrt(5)) / 2;
    std::vector<float> vertices =
            {
                    -1, t, 0, 1, t, 0, -1, -t, 0, 1, -t, 0,
                    0, -1, t, 0, 1, t, 0, -1, -t, 0, 1, -t,
                    t, 0, -1, t, 0, 1, -t, 0, -1, -t, 0, 1
            };

    std::vector<unsigned> indices =
            {
                    0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
                    1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 7, 1, 8,
                    3, 9, 4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9,
                    4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7, 9, 8, 1
            };
}

IcosahedronBufferGeometry::IcosahedronBufferGeometry(float radius, float detail):
        PolyhedronBufferGeometry(icosahedron_geometry::vertices, icosahedron_geometry::indices, radius, detail){}

IcosahedronGeometry::IcosahedronGeometry(float radius, float detail){
    IcosahedronBufferGeometry::ptr bufferGeometry = std::make_shared<IcosahedronBufferGeometry>(radius, detail);

    fromBufferGeometry(*bufferGeometry);
    mergeVertices();
}