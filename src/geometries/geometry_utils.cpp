//
// Created by SeanR on 2023/11/7.
//

#include "geometry_utils.h"

namespace geometry_utils {

    std::vector<Vector3> hilbert3D(Vector3 center, size_t size, int iterations,
                                   int v0, int v1, int v2, int v3, int v4, int v5, int v6, int v7) {
        //default Vars
        const float half = size / 2.0;

        std::vector<Vector3> vec_s = {
                Vector3{center.x - half, center.y + half, center.z - half},
                Vector3{center.x - half, center.y + half, center.z + half},
                Vector3{center.x - half, center.y - half, center.z + half},
                Vector3{center.x - half, center.y - half, center.z - half},
                Vector3{center.x + half, center.y - half, center.z - half},
                Vector3{center.x + half, center.y - half, center.z + half},
                Vector3{center.x + half, center.y + half, center.z + half},
                Vector3{center.x + half, center.y + half, center.z - half}
        };

        std::vector<Vector3> vec = {
                vec_s[v0],
                vec_s[v1],
                vec_s[v2],
                vec_s[v3],
                vec_s[v4],
                vec_s[v5],
                vec_s[v6],
                vec_s[v7]
        };

        // Recurse iterations
        if (--iterations >= 0) {
            std::vector<Vector3> v0s = hilbert3D(vec[0], half, iterations, v0, v3, v4, v7, v6, v5, v2, v1);
            vec.insert(vec.end(), v0s.begin(), v0s.end());
            std::vector<Vector3> v1s = hilbert3D(vec[1], half, iterations, v0, v7, v6, v1, v2, v5, v4, v3);
            vec.insert(vec.end(), v1s.begin(), v1s.end());
            std::vector<Vector3> v2s = hilbert3D(vec[2], half, iterations, v0, v7, v6, v1, v2, v5, v4, v3);
            vec.insert(vec.end(), v2s.begin(), v2s.end());
            std::vector<Vector3> v3s = hilbert3D(vec[3], half, iterations, v2, v3, v0, v1, v6, v7, v4, v5);
            vec.insert(vec.end(), v3s.begin(), v3s.end());
            std::vector<Vector3> v4s = hilbert3D(vec[4], half, iterations, v2, v3, v0, v1, v6, v7, v4, v5);
            vec.insert(vec.end(), v4s.begin(), v4s.end());
            std::vector<Vector3> v5s = hilbert3D(vec[5], half, iterations, v4, v3, v2, v5, v6, v1, v0, v7);
            vec.insert(vec.end(), v5s.begin(), v5s.end());
            std::vector<Vector3> v6s = hilbert3D(vec[6], half, iterations, v4, v3, v2, v5, v6, v1, v0, v7);
            vec.insert(vec.end(), v6s.begin(), v6s.end());
            std::vector<Vector3> v7s = hilbert3D(vec[7], half, iterations, v6, v5, v2, v1, v0, v3, v4, v7);
            vec.insert(vec.end(), v7s.begin(), v7s.end());
//             return std::vector<Vector3>{
//                     hilbert3D( vec[0], half, iterations, v0, v3, v4, v7, v6, v5, v2, v1 ),
//                     hilbert3D( vec[1], half, iterations, v0, v7, v6, v1, v2, v5, v4, v3 ),
//                     hilbert3D( vec[2], half, iterations, v0, v7, v6, v1, v2, v5, v4, v3 ),
//                     hilbert3D( vec[3], half, iterations, v2, v3, v0, v1, v6, v7, v4, v5 ),
//                     hilbert3D( vec[4], half, iterations, v2, v3, v0, v1, v6, v7, v4, v5 ),
//                     hilbert3D( vec[5], half, iterations, v4, v3, v2, v5, v6, v1, v0, v7 ),
//                     hilbert3D( vec[6], half, iterations, v4, v3, v2, v5, v6, v1, v0, v7 ),
//                     hilbert3D( vec[7], half, iterations, v6, v5, v2, v1, v0, v3, v4, v7 )
//             }
        }

        // Return complete Hilbert Curve.
        return vec;

    }
}
