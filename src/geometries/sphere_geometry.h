//
// Created by SeanR on 2022/12/6.
//

#ifndef THREE_CPP_SRC_GEOMETRIES_SPHERE_GEOMETRY_H
#define THREE_CPP_SRC_GEOMETRIES_SPHERE_GEOMETRY_H

#include "buffer_geometry.h"
#include "buffer_attribute.h"
#include "vector3.h"
#include "number.h"

#include <string>
#include <cmath>

struct SphereGeometryParameters{
    double radius,
        phiStart,
        phiLength,
        thetaStart,
        thetaLength;
    int widthSegments,
        heightSegments;
};
class SphereGeometry : public BufferGeometry<int,double>{
public:
    std::string type = "SphereGeometry";
    //SphereGeometryParameters parameters;
    //int widthSegments,heightSegments;



    SphereGeometry(double radius = 1, int widthSegments = 32, int heightSegments = 16, double phiStart = 0, double phiLength = Number::PI * 2, double thetaStart = 0, double thetaLength = Number::PI) {
        this->parameters =
            {{"radius", {radius}}, {"phiStart", {phiStart}}, {"phiLength", {phiLength}}, {"thetaStart", {thetaStart}},
             {"thetaLength", {thetaLength}}, {"widthSegments", {.i=widthSegments}},
             {"heightSegments", {.i=heightSegments}}};

        widthSegments = std::max<int>(3, std::floor(widthSegments));
        heightSegments = std::max<int>(2, std::floor(heightSegments));

        const double thetaEnd = std::min<double>(thetaStart + thetaLength, Number::PI);

        int index = 0;
        std::vector<std::vector<int>> grid;

        Vector3 vertex;
        Vector3 normal;

        // buffers
        std::vector<int> indices;
        std::vector<double> vertices;
        std::vector<double> normals;
        std::vector<double> uvs;
        std::vector<int> verticesRow;

        // generate vertices, normals and uvs
        for (auto iy = 0; iy <= heightSegments; iy++) {
            const double v = iy / heightSegments;

            // special case for the poles
            double uOffset = 0;
            if ( iy == 0 && thetaStart == 0 ) {
                uOffset = 0.5 / widthSegments;
            } else if ( iy == heightSegments && thetaEnd == Number::PI ) {
                uOffset = - 0.5 / widthSegments;
            }

            for ( auto ix = 0; ix <= widthSegments; ix ++ ) {

                const double u = ix / widthSegments;
                // vertex
                vertex.x = - radius * std::cos( phiStart + u * phiLength ) * std::sin( thetaStart + v * thetaLength );
                vertex.y = radius * std::cos( thetaStart + v * thetaLength );
                vertex.z = radius * std::sin( phiStart + u * phiLength ) * std::sin( thetaStart + v * thetaLength );

                vertices.insert(vertices.end(), {vertex.x, vertex.y, vertex.z});

                // normal
                normal.copy( vertex ).normalize();
                normals.insert(normals.end(),{normal.x, normal.y, normal.z} );

                // uv
                uvs.insert(uvs.end(),{ u + uOffset, 1 - v} );

                verticesRow.push_back( index ++ );
            }

            grid.push_back( verticesRow );

        }

        // indices
        for ( auto iy = 0; iy < heightSegments; iy ++ ) {
            for ( auto ix = 0; ix < widthSegments; ix ++ ) {
                const int a = grid[ iy ][ ix + 1 ];
                const int b = grid[ iy ][ ix ];
                const int c = grid[ iy + 1 ][ ix ];
                const int d = grid[ iy + 1 ][ ix + 1 ];

                if ( iy != 0 || thetaStart > 0 ) indices.insert(indices.end(), {a, b, d} );
                if ( iy != heightSegments - 1 || thetaEnd < Number::PI ) indices.insert(indices.end(), {b, c, d} );
            }
        }

        // build geometry
        BufferAttribute<int> indicesBuf{indices,(int)indices.size()};
        this->setIndex( indicesBuf );
        Float64BufferAttribute positionBuf{vertices,3};
        this->setAttribute( "position", std::make_shared<Float64BufferAttribute>(positionBuf));
        Float64BufferAttribute normalBuf{normals,3};
        this->setAttribute( "normal", std::make_shared<Float64BufferAttribute>(normalBuf) );
        Float64BufferAttribute uvBuf{uvs,3};
        this->setAttribute( "uv", std::make_shared<Float64BufferAttribute>(uvBuf) );

    }

};

//static fromJSON( data ) {
//
//    return new SphereGeometry( data.radius, data.widthSegments, data.heightSegments, data.phiStart, data.phiLength, data.thetaStart, data.thetaLength );
//
//}
//
//}
//
//export { SphereGeometry, SphereGeometry as SphereBufferGeometry };


#endif //THREE_CPP_SRC_GEOMETRIES_SPHERE_GEOMETRY_H
