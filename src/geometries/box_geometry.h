//
// Created by SeanR on 2022/10/9.
//

#ifndef THREE_CPP_SRC_GEOMETRIES_BOX_GEOMETRY_H
#define THREE_CPP_SRC_GEOMETRIES_BOX_GEOMETRY_H

#include "buffer_geometry.h"
#include "buffer_attribute.h"

class BoxGeometryParameter {
protected:
    float width;
    float height;
    float depth;
    float widthSegments;
    float heightSegments;
    float depthSegments;
};

class BoxGeometry : public BufferGeometry,public BoxGeometryParameter {
private:
    std::vector<unsigned> _indices;
    std::vector<float> _vertices;
    std::vector<float> _uvs;
    std::vector<float> _normals;

    unsigned numberOfVertices = 0;
    unsigned groupStart = 0;

    void buildPlane(unsigned char u, unsigned char v, unsigned char w, int udir, int vdir, float width, float height, float depth, int gridX, int gridY, int materialIndex);


public:
    using sptr = std::shared_ptr<BoxGeometry>;

    BoxGeometry(float width = 0, float height = 0, float depth = 0, int widthSegments = 0, int heightSegments = 0, int depthSegments = 0);


    static sptr create(float width = 0, float height = 0, float depth = 0, int widthSegments = 0, int heightSegments = 0, int depthSegments = 0){
        return std::make_shared<BoxGeometry>(width,height,depth,widthSegments,heightSegments,depthSegments);
    }
};

//class BoxGeometry : public Geometry,public BoxGeometryParameter {
//public:
//    using sptr = std::shared_ptr<BoxGeometry>;
//    BoxGeometry(float width = 0, float height = 0, float depth = 0, int widthSegments = 0, int heightSegments = 0, int depthSegments = 0);
//
//    static sptr create(float width = 0, float height = 0, float depth = 0, int widthSegments = 0, int heightSegments = 0, int depthSegments = 0) {
//        return std::make_shared<BoxGeometry>(width, height, depth, widthSegments, heightSegments, depthSegments);
//    }
//};

//#include "buffer_geometry.h"
//#include "buffer_attribute.h"
//#include "vector3.h"
//
//#include <vector>
//#include <string>
//
//struct BoxGeometryParameter{
//    double width,
//             height,
//             depth;
//    int widthSegments,
//        heightSegments,
//        depthSegments;
//};
//
//class BoxGeometry:public BufferGeometry<int,double>{
//private:
//    // buffers
//    std::vector<double> vertices{};
//    std::vector<double> normals{};
//    std::vector<double> uvs{};
//    std::vector<int> indices{};
//
//    int numberOfVertices = 0;
//    int groupStart = 0;
//
//public:
//    //BoxGeometryParameter parameters;
//    std::string type;
////    :parameters{
////        width,height,depth,widthSegments,heightSegments,depthSegments
////    },
//    BoxGeometry(double width = 1,double height = 1, double depth = 1,
//                int widthSegments = 1,int heightSegments = 1,int depthSegments = 1): type("BoxGeometry"){
//        this->parameters = {{"width",{width}},{"height",{height}},{"depth",{depth}},
//            {"widthSegments",{.i=widthSegments}},{"heightSegments",{.i=heightSegments}},{"depthSegments",{.i=depthSegments}}};
//        //父级节点应该有一个统一的parameter storage变量
//        //this->parameters
////        widthSegments = floor( widthSegments );
////        heightSegments = floor( heightSegments );
////        depthSegments = floor( depthSegments );
//
//        // build each side of the box geometry
//        buildPlane( 'z', 'y', 'x', - 1, - 1, depth, height, width, depthSegments, heightSegments, 0 ); // px
//        buildPlane( 'z', 'y', 'x', 1, - 1, depth, height, - width, depthSegments, heightSegments, 1 ); // nx
//        buildPlane( 'x', 'z', 'y', 1, 1, width, depth, height, widthSegments, depthSegments, 2 ); // py
//        buildPlane( 'x', 'z', 'y', 1, - 1, width, depth, - height, widthSegments, depthSegments, 3 ); // ny
//        buildPlane( 'x', 'y', 'z', 1, - 1, width, height, depth, widthSegments, heightSegments, 4 ); // pz
//        buildPlane( 'x', 'y', 'z', - 1, - 1, width, height, - depth, widthSegments, heightSegments, 5 ); // nz
//
//        // build geometry
//        BufferAttribute<int> indicesBuf{indices,(int)indices.size()};
//        this->setIndex( indicesBuf );
//        Float64BufferAttribute positionBuf{vertices,3};
//        this->setAttribute( "position", std::make_shared<Float64BufferAttribute>(positionBuf) );
//        Float64BufferAttribute normalBuf{normals,3};
//        this->setAttribute( "normal", std::make_shared<Float64BufferAttribute>(normalBuf) );
//        Float64BufferAttribute uvBuf{uvs,3};
//        this->setAttribute( "uv", std::make_shared<Float64BufferAttribute>(uvBuf) );
//
//    };
//
//
//
//    BoxGeometry& buildPlane( int u, int v, int w, int udir, int vdir, double width, double height, double depth, int gridX, int gridY, int materialIndex ) {
//
//        const double segmentWidth = width / gridX;
//        const double segmentHeight = height / gridY;
//
//        const double widthHalf = width / 2;
//        const double heightHalf = height / 2;
//        const double depthHalf = depth / 2;
//
//        const int gridX1 = gridX + 1;
//        const int gridY1 = gridY + 1;
//
//        int vertexCounter = 0;
//        int groupCount = 0;
//
//        Vector3 vector;
//
//        // generate vertices, normals and uvs
//        for ( int iy = 0; iy < gridY1; iy ++ ) {
//            const double y = iy * segmentHeight - heightHalf;
//
//            for ( int ix = 0; ix < gridX1; ix ++ ) {
//                const double x = ix * segmentWidth - widthHalf;
//
//                // set values to correct vector component
//                vector.setComponent(u, x * udir);
//                vector.setComponent(v,y * vdir);
//                vector.setComponent(w,depthHalf);
//
//                // now apply vector to vertex buffer
//                vertices.push_back(vector.x);
//                vertices.push_back(vector.y);
//                vertices.push_back(vector.z);
//
//                // set values to correct vector component
//                vector.setComponent(u, 0);
//                vector.setComponent(v,0);
//                vector.setComponent(w,depth > 0 ? 1 : - 1);
//
//                // now apply vector to normal buffer
//                normals.push_back(vector.x);
//                normals.push_back(vector.y);
//                normals.push_back(vector.z);
//
//                // uvs
//                uvs.push_back(ix/gridX);
//                uvs.push_back(1-(iy/gridY));
//
//                // counters
//                vertexCounter += 1;
//
//            }
//        }
//
//        // indices
//
//        // 1. you need three indices to draw a single face
//        // 2. a single segment consists of two faces
//        // 3. so we need to generate six (2*3) indices per segment
//        for ( int iy = 0; iy < gridY; iy ++ ) {
//            for ( int ix = 0; ix < gridX; ix ++ ) {
//                const int a = numberOfVertices + ix + gridX1 * iy;
//                const int b = numberOfVertices + ix + gridX1 * ( iy + 1 );
//                const int c = numberOfVertices + ( ix + 1 ) + gridX1 * ( iy + 1 );
//                const int d = numberOfVertices + ( ix + 1 ) + gridX1 * iy;
//
//                // faces
//                indices.insert(indices.end(),{a,b,d});
//                indices.insert(indices.end(),{b, c, d});
//
//                // increase counter
//                groupCount += 6;
//            }
//        }
//
//        // add a group to the geometry. this will ensure multi material support
//        this->addGroup( groupStart, groupCount, materialIndex );
//
//        // calculate new start value for groups
//        groupStart += groupCount;
//
//        // update total number of vertices
//        numberOfVertices += vertexCounter;
//    }
//};
//
//
////static fromJSON( data ) {
////
////    return new BoxGeometry( data.width, data.height, data.depth, data.widthSegments, data.heightSegments, data.depthSegments );
////
////}

#endif //THREE_CPP_SRC_GEOMETRIES_BOX_GEOMETRY_H
