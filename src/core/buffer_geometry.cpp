//
// Created by SeanR on 2022/10/6.
//
#include "buffer_geometry.h"


// const _buffer_geometry_m1 = /*@__PURE__*/ new Matrix4();
// const _obj = /*@__PURE__*/ new Object3D();
// const _offset = /*@__PURE__*/ new Vector3();
// const _buffer_geometry_box = /*@__PURE__*/ new Box3();
// const _buffer_geometry_boxMorphTargets = /*@__PURE__*/ new Box3();
// const _buffer_geometry_vector = /*@__PURE__*/ new Vector3();
Matrix4 _buffer_geometry_m1;
Object3D _obj;
Vector3 _offset;
Box3 _buffer_geometry_box;
Box3 _buffer_geometry_boxMorphTargets;
Vector3 _buffer_geometry_vector;

template <typename T1,typename T2>
int BufferGeometry<T1,T2>::_id = 0;

template<typename T1,typename T2>
BufferGeometry<T1,T2>& BufferGeometry<T1,T2>::applyQuaternion( Quaternion& q ) {
    _buffer_geometry_m1.makeRotationFromQuaternion( q );
    applyMatrix4( _buffer_geometry_m1 );

    return *this;
}

template<typename T1,typename T2>
BufferGeometry<T1,T2>& BufferGeometry<T1,T2>::rotateX( double angle ) {
    // rotate geometry around world x-axis
    _buffer_geometry_m1.makeRotationX( angle );
    applyMatrix4( _buffer_geometry_m1 );

    return *this;
}

template<typename T1,typename T2>
BufferGeometry<T1,T2>& BufferGeometry<T1,T2>::rotateY( double angle ) {
    // rotate geometry around world y-axis
    _buffer_geometry_m1.makeRotationY( angle );
    applyMatrix4( _buffer_geometry_m1 );

    return *this;
}

template<typename T1,typename T2>
BufferGeometry<T1,T2>& BufferGeometry<T1,T2>::rotateZ( double angle ) {
    // rotate geometry around world z-axis
    _buffer_geometry_m1.makeRotationZ( angle );
    applyMatrix4( _buffer_geometry_m1 );

    return *this;
}

template<typename T1,typename T2>
BufferGeometry<T1,T2>& BufferGeometry<T1,T2>::translate( double x, double y, double z ) {
    // translate geometry
    _buffer_geometry_m1.makeTranslation( x, y, z );
    applyMatrix4( _buffer_geometry_m1 );

    return *this;
}

template<typename T1,typename T2>
BufferGeometry<T1,T2>& BufferGeometry<T1,T2>::scale( double x, double y, double z ) {
    // scale geometry
    _buffer_geometry_m1.makeScale( x, y, z );
    applyMatrix4( _buffer_geometry_m1 );

    return *this;
}

//template<typename T1,typename T2>
//BufferGeometry<T1,T2>& BufferGeometry<T1,T2>::lookAt( Vector3& vector ) {
//    _obj.lookAt( vector );
//    _obj.updateMatrix();
//
//    applyMatrix4( _obj.matrix );
//
//    return *this;
//}
//
//template<typename T1,typename T2>
//BufferGeometry<T1,T2>& BufferGeometry<T1,T2>::center() {
//    computeBoundingBox();
//    //due to previous calling,it is impossible for boundingBox==nullptr
//    boundingBox->getCenter( _offset ).negate();
//    translate( _offset.x, _offset.y, _offset.z );
//
//    return *this;
//}

//template<typename T1,typename T2>
//BufferGeometry<T1,T2>& BufferGeometry<T1,T2>::setFromPoints( vector<Vector3> points ) {
//    vector<T2> positionData;
//
//    for ( int i = 0, l = points.size; i < l; i ++ ) {
//        Vector3 point = points[ i ];
//        position.push_back( point.x, point.y, point.z || 0 );
//    }
//
//    setAttribute( "position", BufferAttribute<T2>( positionData, 3 ) );
//
//    return *this;
//}
//
//
//template<typename T1,typename T2>
//BufferGeometry<T1,T2>& BufferGeometry<T1,T2>::computeBoundingBox() {
//
//    if ( boundingBox == nullptr ) {
//        boundingBox = std::make_shared<Box3>();
//    }
//
//    BufferAttribute<T2> position = attributes["position"];
//    BufferAttribute<T2> morphAttributesPosition = morphAttributes["position"];
//
//    // if ( position && position.isGLBufferAttribute ) {
//
//    // 	console.error( 'THREE.BufferGeometry.computeBoundingBox(): GLBufferAttribute requires a manual bounding box. Alternatively set "mesh.frustumCulled" to "false".', this );
//
//    // 	this.boundingBox.set(
//    // 		new Vector3( - Infinity, - Infinity, - Infinity ),
//    // 		new Vector3( + Infinity, + Infinity, + Infinity )
//    // 	);
//
//    // 	return;
//
//    // }
//
//    if ( position != nullptr ) {
//        boundingBox->setFromBufferAttribute( position );
//
//        // process morph attributes if present
//        if ( morphAttributesPosition ) {
//
//            for ( int i = 0, il = morphAttributesPosition.size(); i < il; i ++ ) {
//
//                auto morphAttribute = morphAttributesPosition[ i ].second;
//                _buffer_geometry_box.setFromBufferAttribute( morphAttribute );
//
//                if ( morphTargetsRelative ) {
//                    _buffer_geometry_vector.addVectors( boundingBox->min, _buffer_geometry_box.min );
//                    boundingBox->expandByPoint( _buffer_geometry_vector );
//
//                    _buffer_geometry_vector.addVectors( boundingBox->max, _buffer_geometry_box.max );
//                    boundingBox->expandByPoint( _buffer_geometry_vector );
//                } else {
//                    boundingBox->expandByPoint( _buffer_geometry_box.min );
//                    boundingBox->expandByPoint( _buffer_geometry_box.max );
//                }
//            }
//        }
//
//    } else {
//        boundingBox->makeEmpty();
//    }
//
//    return *this;
//}
//
//template<typename T1,typename T2>
//BufferGeometry<T1,T2>& BufferGeometry<T1,T2>::computeBoundingSphere() {
//    if ( boundingSphere == nullptr ) {
//        boundingSphere = std::shared_ptr<Sphere>();
//    }
//
//    const position = attributes["position"];
//    const morphAttributesPosition = morphAttributes["position"];
//
//    // if ( position && position.isGLBufferAttribute ) {
//    // 	console.error( 'THREE.BufferGeometry.computeBoundingSphere(): GLBufferAttribute requires a manual bounding sphere. Alternatively set "mesh.frustumCulled" to "false".', this );
//    // 	this.boundingSphere.set( new Vector3(), Infinity );
//    // 	return;
//    // }
//    if ( position ) {
//
//        // first, find the center of the bounding sphere
//
//        const center = this.boundingSphere.center;
//
//        _buffer_geometry_box.setFromBufferAttribute( position );
//
//        // process morph attributes if present
//
//        if ( morphAttributesPosition ) {
//
//            for ( let i = 0, il = morphAttributesPosition.length; i < il; i ++ ) {
//
//                const morphAttribute = morphAttributesPosition[ i ];
//                _buffer_geometry_boxMorphTargets.setFromBufferAttribute( morphAttribute );
//
//                if ( this.morphTargetsRelative ) {
//
//                    _buffer_geometry_vector.addVectors( _buffer_geometry_box.min, _buffer_geometry_boxMorphTargets.min );
//                    _buffer_geometry_box.expandByPoint( _buffer_geometry_vector );
//
//                    _buffer_geometry_vector.addVectors( _buffer_geometry_box.max, _buffer_geometry_boxMorphTargets.max );
//                    _buffer_geometry_box.expandByPoint( _buffer_geometry_vector );
//
//                } else {
//
//                    _buffer_geometry_box.expandByPoint( _buffer_geometry_boxMorphTargets.min );
//                    _buffer_geometry_box.expandByPoint( _buffer_geometry_boxMorphTargets.max );
//
//                }
//
//            }
//
//        }
//
//        _buffer_geometry_box.getCenter( center );
//
//        // second, try to find a boundingSphere with a radius smaller than the
//        // boundingSphere of the boundingBox: sqrt(3) smaller in the best case
//
//        let maxRadiusSq = 0;
//
//        for ( let i = 0, il = position.count; i < il; i ++ ) {
//
//            _buffer_geometry_vector.fromBufferAttribute( position, i );
//
//            maxRadiusSq = Math.max( maxRadiusSq, center.distanceToSquared( _buffer_geometry_vector ) );
//
//        }
//
//        // process morph attributes if present
//        if ( morphAttributesPosition ) {
//            for ( let i = 0, il = morphAttributesPosition.size(); i < il; i ++ ) {
//
//                auto morphAttribute = morphAttributesPosition[ i ].second;
//                bool morphTargetsRelative = morphTargetsRelative;
//
//                for ( int j = 0, jl = morphAttribute.count; j < jl; j ++ ) {
//
//                    _buffer_geometry_vector.fromBufferAttribute( morphAttribute, j );
//
//                    if ( morphTargetsRelative ) {
//                        _offset.fromBufferAttribute( position, j );
//                        _buffer_geometry_vector.add( _offset );
//                    }
//
//                    maxRadiusSq = fmax( maxRadiusSq, center.distanceToSquared( _buffer_geometry_vector ) );
//                }
//            }
//        }
//        boundingSphere->radius = sqrt( maxRadiusSq );
//    }
//
//}
//
//template<typename T1,typename T2>
//BufferGeometry<T1,T2>& BufferGeometry<T1,T2>::normalizeNormals() {
//    shared_ptr<BufferAttribute<T2>> normals = attributes["normal"];
//
//    for ( int i = 0, il = normals->count; i < il; i ++ ) {
//        _buffer_geometry_vector.fromBufferAttribute( *normals, i );
//        _buffer_geometry_vector.normalize();
//        normals->setXYZ( i, _buffer_geometry_vector.x, _buffer_geometry_vector.y, _buffer_geometry_vector.z );
//    }
//}

