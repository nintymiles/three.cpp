//
// Created by SeanR on 2023/10/20.
//

#include "tetrahedron_geometry.h"

namespace tetrahedron_geometry {
    std::vector<float> tet_vertices =
            {
                    1, 1, 1, -1, -1, 1, -1, 1, -1, 1, -1, -1
            };

    std::vector<unsigned> tet_indices =
            {
                    2, 1, 0, 0, 3, 2, 1, 3, 0, 2, 3, 1
            };
}
using namespace tetrahedron_geometry;

TetrahedronGeometry::TetrahedronGeometry(float radius, float detail) :
                PolyhedronBufferGeometry(tet_vertices,tet_indices,radius,detail){}

