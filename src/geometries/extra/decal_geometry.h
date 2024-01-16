//
// Created by SeanR on 2024/1/16.
//

#ifndef THREE_CPP_DECAL_GEOMETRY_H
#define THREE_CPP_DECAL_GEOMETRY_H

#include "buffer_geometry.h"

#include "vector3.h"
#include "matrix4.h"
#include "euler.h"

#include "mesh.h"

#include "container_utils.h"

// helper
struct DecalVertex {
    Vector3 position;
    Vector3 normal;

    DecalVertex(){}
    DecalVertex( Vector3& position, Vector3& normal ):position(position),normal(normal) {}

    DecalVertex clone() {
        return DecalVertex( this->position.clone(), this->normal.clone() );
    }

};


/**
 * You can use this geometry to create a decal mesh, that serves different kinds of purposes.
 * e.g. adding unique details to models, performing dynamic visual environmental changes or covering seams.
 *
 * Constructor parameter:
 *
 * mesh — Any mesh object
 * position — Position of the decal projector
 * orientation — Orientation of the decal projector
 * size — Size of the decal projector
 *
 * reference: http://blog.wolfire.com/2009/06/how-to-project-decals/
 *
 */
class DecalGeometry : public BufferGeometry {
    Vector3 size;
    Euler orientation;
    Vector3 position;
    Mesh::sptr mesh;

    // helpers
    Matrix4 projectorMatrixInverse;
    Matrix4 projectorMatrix;
    Vector3 plane;

    // buffers
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> uvs;

public:
    using sptr = std::shared_ptr<DecalGeometry>;
    DecalGeometry(Mesh::sptr mesh, const Vector3& position, const Euler& orientation, const Vector3& size)
                                :BufferGeometry(),mesh(mesh),position(position),size(size),orientation(orientation){

        // this matrix represents the transformation of the decal projector
        projectorMatrix.makeRotationFromEuler( orientation );
        projectorMatrix.setPosition( position );

        projectorMatrixInverse.copy( projectorMatrix ).invert();

        // generate buffers
        generate();

        // build geometry
        this->setAttribute( AttributeName::position, BufferAttribute<float>::create( vertices, 3 ) );
        this->setAttribute( AttributeName::normal, BufferAttribute<float>::create( normals, 3 ) );
        this->setAttribute( AttributeName::uv, BufferAttribute<float>::create( uvs, 2 ) );
    }

    static sptr create(Mesh::sptr mesh, const Vector3& position, const Euler& orientation, const Vector3& size){
        return std::make_shared<DecalGeometry>(mesh,position,orientation,size);
    }

    void generate() {

        std::vector<DecalVertex> decalVertices{};

        Vector3 vertex{};
        Vector3 normal{};

        // handle different geometry types
        auto geometry = std::dynamic_pointer_cast<BufferGeometry>(mesh->geometry);

        auto positionAttribute = geometry->getAttribute(AttributeName::position);
        auto normalAttribute = geometry->getAttribute(AttributeName::normal);;

        // first, create an array of 'DecalVertex' objects
        // three consecutive 'DecalVertex' objects represent a single face
        //
        // this data structure will be later used to perform the clipping

        if ( geometry->index ) {

            // indexed BufferGeometry
            auto index = geometry->index;

            for ( size_t i = 0; i < index->count; i ++ ) {

                auto idx = index->getX(i);
                vertex.set(positionAttribute->getX(idx),positionAttribute->getY(idx),positionAttribute->getZ(idx));
                normal.set( normalAttribute->getX(idx), normalAttribute->getY(idx),normalAttribute->getZ(idx) );

                pushDecalVertex( decalVertices, vertex, normal );
            }

        } else {

            // non-indexed BufferGeometry
            for ( size_t i = 0; i < positionAttribute->count; i ++ ) {

                auto idx = i;
                vertex.set(positionAttribute->getX(idx),positionAttribute->getY(idx),positionAttribute->getZ(idx));
                normal.set( normalAttribute->getX(idx), normalAttribute->getY(idx),normalAttribute->getZ(idx) );

                pushDecalVertex( decalVertices, vertex, normal );
            }

        }

        // second, clip the geometry so that it doesn't extend out from the projector

        decalVertices = clipGeometry( decalVertices, plane.set( 1, 0, 0 ) );
        decalVertices = clipGeometry( decalVertices, plane.set( - 1, 0, 0 ) );
        decalVertices = clipGeometry( decalVertices, plane.set( 0, 1, 0 ) );
        decalVertices = clipGeometry( decalVertices, plane.set( 0, - 1, 0 ) );
        decalVertices = clipGeometry( decalVertices, plane.set( 0, 0, 1 ) );
        decalVertices = clipGeometry( decalVertices, plane.set( 0, 0, - 1 ) );

        // third, generate final vertices, normals and uvs
        for ( size_t i = 0; i < decalVertices.size(); i ++ ) {

            auto decalVertex = decalVertices[ i ];

            // create texture coordinates (we are still in projector space)
            container_utils::fillDoubleIntoVector<float>(uvs,
                    0.5 + ( decalVertex.position.x / size.x ),
                    0.5 + ( decalVertex.position.y / size.y )
            );


            // transform the vertex back to world space
            decalVertex.position.applyMatrix4( projectorMatrix );

            // now create vertex and normal buffer data
            container_utils::fillTripleIntoVector(vertices, decalVertex.position.x, decalVertex.position.y, decalVertex.position.z );
            container_utils::fillTripleIntoVector( normals,decalVertex.normal.x, decalVertex.normal.y, decalVertex.normal.z );

        }

    }

    void pushDecalVertex( std::vector<DecalVertex>& decalVertices, Vector3 vertex, Vector3 normal ) {

        // transform the vertex to world space, then to projector space
        vertex.applyMatrix4( mesh->matrixWorld );
        vertex.applyMatrix4( projectorMatrixInverse );

        normal.transformDirection( mesh->matrixWorld );

        decalVertices.push_back( DecalVertex( vertex.clone(), normal.clone() ) );
    }

    std::vector<DecalVertex> clipGeometry( std::vector<DecalVertex>& inVertices, Vector3& plane ) {

        std::vector<DecalVertex> outVertices{};

        auto s = 0.5 * math::abs( size.dot( plane ) );

        // a single iteration clips one face,
        // which consists of three consecutive 'DecalVertex' objects
        for ( size_t i = 0; i < inVertices.size(); i += 3 ) {

            auto total = 0;
            DecalVertex nV1;
            DecalVertex nV2;
            DecalVertex nV3;
            DecalVertex nV4;

            auto d1 = inVertices[ i + 0 ].position.dot( plane ) - s;
            auto d2 = inVertices[ i + 1 ].position.dot( plane ) - s;
            auto d3 = inVertices[ i + 2 ].position.dot( plane ) - s;

            auto v1Out = d1 > 0;
            auto v2Out = d2 > 0;
            auto v3Out = d3 > 0;

            // calculate, how many vertices of the face lie outside of the clipping plane
            total = ( v1Out ? 1 : 0 ) + ( v2Out ? 1 : 0 ) + ( v3Out ? 1 : 0 );

            switch ( total ) {

                case 0: {

                    // the entire face lies inside of the plane, no clipping needed
                    outVertices.push_back( inVertices[ i ] );
                    outVertices.push_back( inVertices[ i + 1 ] );
                    outVertices.push_back( inVertices[ i + 2 ] );
                    break;
                }

                case 1: {

                    // one vertex lies outside of the plane, perform clipping
                    if ( v1Out ) {

                        nV1 = inVertices[ i + 1 ];
                        nV2 = inVertices[ i + 2 ];
                        nV3 = clip( inVertices[ i ], nV1, plane, s );
                        nV4 = clip( inVertices[ i ], nV2, plane, s );
                    }

                    if ( v2Out ) {

                        nV1 = inVertices[ i ];
                        nV2 = inVertices[ i + 2 ];
                        nV3 = clip( inVertices[ i + 1 ], nV1, plane, s );
                        nV4 = clip( inVertices[ i + 1 ], nV2, plane, s );

                        outVertices.push_back( nV3 );
                        outVertices.push_back( nV2.clone() );
                        outVertices.push_back( nV1.clone() );

                        outVertices.push_back( nV2.clone() );
                        outVertices.push_back( nV3.clone() );
                        outVertices.push_back( nV4 );
                        break;
                    }

                    if ( v3Out ) {

                        nV1 = inVertices[ i ];
                        nV2 = inVertices[ i + 1 ];
                        nV3 = clip( inVertices[ i + 2 ], nV1, plane, s );
                        nV4 = clip( inVertices[ i + 2 ], nV2, plane, s );
                    }

                    outVertices.push_back( nV1.clone() );
                    outVertices.push_back( nV2.clone() );
                    outVertices.push_back( nV3 );

                    outVertices.push_back( nV4 );
                    outVertices.push_back( nV3.clone() );
                    outVertices.push_back( nV2.clone() );

                    break;
                }

                case 2: {

                    // two vertices lies outside of the plane, perform clipping
                    if ( ! v1Out ) {

                        nV1 = inVertices[ i ].clone();
                        nV2 = clip( nV1, inVertices[ i + 1 ], plane, s );
                        nV3 = clip( nV1, inVertices[ i + 2 ], plane, s );
                        outVertices.push_back( nV1 );
                        outVertices.push_back( nV2 );
                        outVertices.push_back( nV3 );
                    }

                    if ( ! v2Out ) {

                        nV1 = inVertices[ i + 1 ].clone();
                        nV2 = clip( nV1, inVertices[ i + 2 ], plane, s );
                        nV3 = clip( nV1, inVertices[ i ], plane, s );
                        outVertices.push_back( nV1 );
                        outVertices.push_back( nV2 );
                        outVertices.push_back( nV3 );
                    }

                    if ( ! v3Out ) {

                        nV1 = inVertices[ i + 2 ].clone();
                        nV2 = clip( nV1, inVertices[ i ], plane, s );
                        nV3 = clip( nV1, inVertices[ i + 1 ], plane, s );
                        outVertices.push_back( nV1 );
                        outVertices.push_back( nV2 );
                        outVertices.push_back( nV3 );
                    }

                    break;

                }

                case 3: {

                    // the entire face lies outside of the plane, so let's discard the corresponding vertices

                    break;

                }

            }

        }

        return outVertices;

    }

    DecalVertex clip( DecalVertex& v0, DecalVertex& v1, Vector3& p, float s ) {

        auto d0 = v0.position.dot( p ) - s;
        auto d1 = v1.position.dot( p ) - s;

        auto s0 = d0 / ( d0 - d1 );

        Vector3 position = Vector3(
                v0.position.x + s0 * ( v1.position.x - v0.position.x ),
                v0.position.y + s0 * ( v1.position.y - v0.position.y ),
                v0.position.z + s0 * ( v1.position.z - v0.position.z )
        );
        Vector3 normal = Vector3(
                v0.normal.x + s0 * ( v1.normal.x - v0.normal.x ),
                v0.normal.y + s0 * ( v1.normal.y - v0.normal.y ),
                v0.normal.z + s0 * ( v1.normal.z - v0.normal.z )
        );

        DecalVertex v = DecalVertex(position,normal);

        // need to clip more values (texture coordinates)? do it this way:
        // intersectpoint.value = a.value + s * ( b.value - a.value );

        return v;

    }

};



#endif //THREE_CPP_DECAL_GEOMETRY_H
