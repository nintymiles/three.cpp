//
// Created by SeanR on 2022/10/6.
//
#include "buffer_geometry.h"



// const _m1 = /*@__PURE__*/ new Matrix4();
// const _obj = /*@__PURE__*/ new Object3D();
// const _offset = /*@__PURE__*/ new Vector3();
// const _box = /*@__PURE__*/ new Box3();
// const _boxMorphTargets = /*@__PURE__*/ new Box3();
// const _vector = /*@__PURE__*/ new Vector3();
Matrix4 _m1;
Object3D _obj;
Vector3 _offset;
Box3 _box;
Box3 _boxMorphTargets;
Vector3 _vector;

int BufferGeometry::_id = 0;

BufferGeometry& BufferGeometry::applyQuaternion( Quaternion& q ) {
    _m1.makeRotationFromQuaternion( q );
    applyMatrix4( _m1 );

    return *this;
}

BufferGeometry& BufferGeometry::rotateX( double angle ) {
    // rotate geometry around world x-axis
    _m1.makeRotationX( angle );
    applyMatrix4( _m1 );

    return *this;
}

BufferGeometry& BufferGeometry::rotateY( double angle ) {
    // rotate geometry around world y-axis
    _m1.makeRotationY( angle );
    applyMatrix4( _m1 );

    return *this;
}

BufferGeometry& BufferGeometry::rotateZ( double angle ) {
    // rotate geometry around world z-axis
    _m1.makeRotationZ( angle );
    applyMatrix4( _m1 );

    return *this;
}

BufferGeometry& BufferGeometry::translate( double x, double y, double z ) {
    // translate geometry
    _m1.makeTranslation( x, y, z );
    applyMatrix4( _m1 );

    return *this;
}

BufferGeometry& BufferGeometry::scale( double x, double y, double z ) {
    // scale geometry
    _m1.makeScale( x, y, z );
    applyMatrix4( _m1 );

    return *this;
}

BufferGeometry& BufferGeometry::lookAt( Vector3& vector ) {
    _obj.lookAt( vector );
    _obj.updateMatrix();

    applyMatrix4( _obj.matrix );

    return *this;
}

BufferGeometry& BufferGeometry::center() {
    computeBoundingBox();
    //due to previous calling,it is impossible for boundingBox==nullptr
    boundingBox->getCenter( _offset ).negate();
    translate( _offset.x, _offset.y, _offset.z );

    return *this;
}

BufferGeometry& BufferGeometry::setFromPoints( vector<Vector3> points ) {
    vector<T2> positionData;

    for ( int i = 0, l = points.size; i < l; i ++ ) {
        Vector3 point = points[ i ];
        position.push_back( point.x, point.y, point.z || 0 );
    }

    setAttribute( "position", BufferAttribute<T2>( positionData, 3 ) );

    return *this;
}


BufferGeometry& BufferGeometry::computeBoundingBox() {

    if ( boundingBox == nullptr ) {
        boundingBox = std::make_shared<Box3>();
    }

    BufferAttribute<T2> position = attributes["position"];
    BufferAttribute<T2> morphAttributesPosition = morphAttributes["position"];

    // if ( position && position.isGLBufferAttribute ) {

    // 	console.error( 'THREE.BufferGeometry.computeBoundingBox(): GLBufferAttribute requires a manual bounding box. Alternatively set "mesh.frustumCulled" to "false".', this );

    // 	this.boundingBox.set(
    // 		new Vector3( - Infinity, - Infinity, - Infinity ),
    // 		new Vector3( + Infinity, + Infinity, + Infinity )
    // 	);

    // 	return;

    // }

    if ( position != nullptr ) {
        boundingBox->setFromBufferAttribute( position );

        // process morph attributes if present
        if ( morphAttributesPosition ) {

            for ( int i = 0, il = morphAttributesPosition.size(); i < il; i ++ ) {

                auto morphAttribute = morphAttributesPosition[ i ].second;
                _box.setFromBufferAttribute( morphAttribute );

                if ( morphTargetsRelative ) {
                    _vector.addVectors( boundingBox->min, _box.min );
                    boundingBox->expandByPoint( _vector );

                    _vector.addVectors( boundingBox->max, _box.max );
                    boundingBox->expandByPoint( _vector );
                } else {
                    boundingBox->expandByPoint( _box.min );
                    boundingBox->expandByPoint( _box.max );
                }
            }
        }

    } else {
        boundingBox->makeEmpty();
    }

    return *this;
}

BufferGeometry& BufferGeometry::computeBoundingSphere() {
    if ( boundingSphere == nullptr ) {
        boundingSphere = std::shared_ptr<Sphere>();
    }

    const position = attributes["position"];
    const morphAttributesPosition = morphAttributes["position"];

    // if ( position && position.isGLBufferAttribute ) {
    // 	console.error( 'THREE.BufferGeometry.computeBoundingSphere(): GLBufferAttribute requires a manual bounding sphere. Alternatively set "mesh.frustumCulled" to "false".', this );
    // 	this.boundingSphere.set( new Vector3(), Infinity );
    // 	return;
    // }
    if ( position ) {

        // first, find the center of the bounding sphere

        const center = this.boundingSphere.center;

        _box.setFromBufferAttribute( position );

        // process morph attributes if present

        if ( morphAttributesPosition ) {

            for ( let i = 0, il = morphAttributesPosition.length; i < il; i ++ ) {

                const morphAttribute = morphAttributesPosition[ i ];
                _boxMorphTargets.setFromBufferAttribute( morphAttribute );

                if ( this.morphTargetsRelative ) {

                    _vector.addVectors( _box.min, _boxMorphTargets.min );
                    _box.expandByPoint( _vector );

                    _vector.addVectors( _box.max, _boxMorphTargets.max );
                    _box.expandByPoint( _vector );

                } else {

                    _box.expandByPoint( _boxMorphTargets.min );
                    _box.expandByPoint( _boxMorphTargets.max );

                }

            }

        }

        _box.getCenter( center );

        // second, try to find a boundingSphere with a radius smaller than the
        // boundingSphere of the boundingBox: sqrt(3) smaller in the best case

        let maxRadiusSq = 0;

        for ( let i = 0, il = position.count; i < il; i ++ ) {

            _vector.fromBufferAttribute( position, i );

            maxRadiusSq = Math.max( maxRadiusSq, center.distanceToSquared( _vector ) );

        }

        // process morph attributes if present
        if ( morphAttributesPosition ) {
            for ( let i = 0, il = morphAttributesPosition.size(); i < il; i ++ ) {

                auto morphAttribute = morphAttributesPosition[ i ].second;
                bool morphTargetsRelative = morphTargetsRelative;

                for ( int j = 0, jl = morphAttribute.count; j < jl; j ++ ) {

                    _vector.fromBufferAttribute( morphAttribute, j );

                    if ( morphTargetsRelative ) {
                        _offset.fromBufferAttribute( position, j );
                        _vector.add( _offset );
                    }

                    maxRadiusSq = fmax( maxRadiusSq, center.distanceToSquared( _vector ) );
                }
            }
        }
        boundingSphere->radius = sqrt( maxRadiusSq );
    }

}

BufferGeometry& BufferGeometry::normalizeNormals() {
    shared_ptr<BufferAttribute<T2>> normals = attributes["normal"];

    for ( int i = 0, il = normals->count; i < il; i ++ ) {
        _vector.fromBufferAttribute( *normals, i );
        _vector.normalize();
        normals->setXYZ( i, _vector.x, _vector.y, _vector.z );
    }
}

