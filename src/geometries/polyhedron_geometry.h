//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_POLYHEDRON_GEOMETRY_H
#define THREE_CPP_POLYHEDRON_GEOMETRY_H

#include "buffer_geometry.h"

class PolyhedronBufferGeometry : public BufferGeometry {

private :
    std::vector<float> _vertices;
    std::vector<unsigned> _indices;
    std::vector<float> _vertexBuffer;
    std::vector<float> _uvBuffer;

    void getVertexByIndex(unsigned index, Vector3& vertex);

    void pushVertex(const Vector3& vertex);

    void subdivide(float detail);

    void subdivideFace(const Vector3& a,const Vector3& b, const Vector3& c, float detail);


    void applyRadius(float radius);

    void generateUVs();

    void correctSeam();

    void correctUVs();

    void correctUV(const Vector2& uv, int stride, const Vector3& vector, float azimuth);

    float azimuth(const Vector3& vector);

    float inclination(const Vector3& vector);


public:
    using sptr = std::shared_ptr<PolyhedronBufferGeometry>;
    PolyhedronBufferGeometry(const std::vector<float>& vertices, const std::vector<unsigned>& indices, float radius = 0, float detail = 0);

    static sptr create(const std::vector<float>& vertices, const std::vector<unsigned>& indices, float radius = 0, float detail = 0) {
        return std::make_shared<PolyhedronBufferGeometry>(vertices, indices, radius, detail);
    }
};

class PolyhedronGeometry : public Geometry {
public:
    PolyhedronGeometry(const std::vector<float>& vertices, const std::vector<unsigned>& indices, float radius = 0, float detail = 0);

    using sptr = std::shared_ptr<PolyhedronGeometry>;

    static sptr create(const std::vector<float>& vertices, const std::vector<unsigned>& indices, float radius = 0, float detail = 0) {
        return std::make_shared<PolyhedronGeometry>(vertices, indices, radius, detail);
    }
};

#endif //THREE_CPP_POLYHEDRON_GEOMETRY_H
