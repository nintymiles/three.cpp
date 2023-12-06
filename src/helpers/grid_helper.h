//
// Created by SeanR on 2023/12/6.
//

#ifndef THREE_CPP_GRID_HELPER_H
#define THREE_CPP_GRID_HELPER_H

#include "line.h"

#include "color.h"
#include "line_basic_material.h"
#include "buffer_geometry.h"

#include <iterator>

class GridHelper: public LineSegments{
    Color color1,color2;
    int size,divisions;

    std::string type = "GridHelper";



public:
    GridHelper(int size = 10,int divisions = 10,Color color1 = 0x444444,Color color2 = 0x888888):size(size),divisions(divisions),color1(color1),color2(color2){

        int center = divisions / 2;
        float step = (float)size / divisions;
        int halfSize = size / 2;

        //const vertices = [], colors = [];
        std::vector<float> vertices;
        std::vector<float> colors;

        //float colorsArr[divisions * 3 * 4];
        for ( int i = 0, j = 0, k = - halfSize; i <= divisions; i ++, k += step ) {
            vertices.push_back( - halfSize );
            vertices.push_back( 0 );
            vertices.push_back( k );
            vertices.push_back( halfSize );
            vertices.push_back( 0 );
            vertices.push_back( k );

            vertices.push_back( k );
            vertices.push_back( 0 );
            vertices.push_back( - halfSize );
            vertices.push_back( k );
            vertices.push_back( 0 );
            vertices.push_back( halfSize );

            Color &color = i == center ? color1 : color2;

//            color.toArray( colorsArr, j ); j += 3;
//            color.toArray( colorsArr, j ); j += 3;
//            color.toArray( colorsArr, j ); j += 3;
//            color.toArray( colorsArr, j ); j += 3;

            color.appendToVector(colors).appendToVector(colors).appendToVector(colors).appendToVector(colors);

        }

        //colors.insert(std::begin(colorsArr),std::end(colorsArr));

        BufferGeometry::sptr geometry = BufferGeometry::create();
        geometry->setAttribute( AttributeName::position, BufferAttribute<float>::create( vertices, 3 ) );
        geometry->setAttribute( AttributeName::color, BufferAttribute<float>::create( colors, 3 ) );

        LineBasicMaterial::sptr material = LineBasicMaterial::create();
        material->vertexColors = true;
        material->toneMapped = false;

        LineSegments( geometry, material );
    }

};


#endif //THREE_CPP_GRID_HELPER_H