//
// Created by SeanR on 2022/10/13.
//

#ifndef THREE_CPP_SRC_OBJECTS_LINE_H
#define THREE_CPP_SRC_OBJECTS_LINE_H

#include "object_3d.h"
#include "geometry.h"
//#include "line_basic_material.h"

#include <list>

class Line :public Object3D {
public:
    using sptr = std::shared_ptr<Line>;

    unsigned lineStrip = 0;
    unsigned linePieces = 1;


    Line(const Geometry::sptr& geometry = nullptr, const Material::sptr& material = nullptr);

    Line(const Geometry::sptr& geometry, const std::vector<Material::sptr>& materials);

    static sptr create(const Geometry::sptr& geometry=nullptr, const Material::sptr& material=nullptr) {
        return std::make_shared<Line>(geometry, material);
    }

    static sptr create(const Geometry::sptr& geometry, const std::vector<Material::sptr>& materials) {
        return std::make_shared<Line>(geometry, materials);
    }

    virtual Line& computeLineDistances();

    virtual void raycast();

};

class LineLoop : public Line {
public:
    using sptr = std::shared_ptr<LineLoop>;
    LineLoop(const Geometry::sptr& geometry = nullptr, const Material::sptr& material = nullptr) : Line(geometry, material) {
        type = "LineLoop";
    }

    LineLoop(const Geometry::sptr& geometry, const std::vector<Material::sptr>& materials) : Line(geometry, materials) {
        type = "LineLoop";
    }

};

class LineSegments : public Line {
public:
    using sptr = std::shared_ptr<LineSegments>;

    LineSegments(const Geometry::sptr& geometry = nullptr, const Material::sptr& material = nullptr) : Line(geometry, material) {
        type = "LineSegments";
    }


    LineSegments(const Geometry::sptr& geometry, const std::vector<Material::sptr>& materials) : Line(geometry, materials) {
        type = "LineSegments";
    }

    static sptr create(const Geometry::sptr& geometry = nullptr, const Material::sptr& material = nullptr) {
        return std::make_shared<LineSegments>(geometry, material);
    }

    static sptr create(const Geometry::sptr& geometry, const std::vector<Material::sptr>& materials) {
        return std::make_shared<LineSegments>(geometry, materials);
    }

    virtual LineSegments& computeLineDistances();
};

//class Line : public Object3D {
//public:
//    std::string type = "Line";
//    bool isLine = true;
//    BufferGeometry<int,double> geometry;
//
//    Line(BufferGeometry<int,double> geometry = BufferGeometry<int,double>()){
//        //updateMorphTargets();
//    };
//
//
//    Line& updateMorphTargets() {
//        BufferGeometry<int,double> geometry = this->geometry;
//
//        if ( geometry.isBufferGeometry ) {
//
//            map<string,shared_ptr<BufferAttribute<double>>> morphAttributes = geometry.morphAttributes;
//
//            //set keys = morphAttributes;
//            std::vector<std::string> keys{};
////            int arraySize = morphAttributes.size();
////            std::array<std::string,arraySize> keys;
//
//            // C++11 alternative:
//            for (const auto& n : morphAttributes) {
//                keys.push_back(n.first);
//            }
//
//            if ( keys.size() > 0 ) {
//                std::shared_ptr<BufferAttribute<double>> morphAttribute = morphAttributes[ keys[ 0 ] ];
//
////                if ( morphAttribute != nullptr ) {
////                    this.morphTargetInfluences = [];
////                    this.morphTargetDictionary = {};
////
////                    for ( let m = 0, ml = morphAttribute.length; m < ml; m ++ ) {
////
////                        const name = morphAttribute[ m ].name || String( m );
////
////                        this.morphTargetInfluences.push( 0 );
////                        this.morphTargetDictionary[ name ] = m;
////
////                    }
////
////                }
//            }
//
//        } else {
////            const morphTargets = geometry.morphTargets;
//
////            if ( morphTargets !== undefined && morphTargets.length > 0 ) {
////
////                console.error( 'THREE.Line.updateMorphTargets() does not support THREE.Geometry. Use THREE.BufferGeometry instead.' );
////
////            }
//
//        }
//
//    }
//
//
//};
//
//
////constructor( geometry = new BufferGeometry(), material = new LineBasicMaterial() ) {
////
////super();
////
////this.type = 'Line';
////
////this.geometry = geometry;
////this.material = material;
////
////this.updateMorphTargets();
////
////}
////
////copy( source ) {
////
////    super.copy( source );
////
////    this.material = source.material;
////    this.geometry = source.geometry;
////
////    return this;
////
////}
////
////computeLineDistances() {
////
////    const geometry = this.geometry;
////
////    if ( geometry.isBufferGeometry ) {
////
////        // we assume non-indexed geometry
////
////        if ( geometry.index === null ) {
////
////            const positionAttribute = geometry.attributes.position;
////            const lineDistances = [ 0 ];
////
////            for ( let i = 1, l = positionAttribute.count; i < l; i ++ ) {
////
////                _start.fromBufferAttribute( positionAttribute, i - 1 );
////                _end.fromBufferAttribute( positionAttribute, i );
////
////                lineDistances[ i ] = lineDistances[ i - 1 ];
////                lineDistances[ i ] += _start.distanceTo( _end );
////
////            }
////
////            geometry.setAttribute( 'lineDistance', new Float32BufferAttribute( lineDistances, 1 ) );
////
////        } else {
////
////            console.warn( 'THREE.Line.computeLineDistances(): Computation only possible with non-indexed BufferGeometry.' );
////
////        }
////
////    } else if ( geometry.isGeometry ) {
////
////        console.error( 'THREE.Line.computeLineDistances() no longer supports THREE.Geometry. Use THREE.BufferGeometry instead.' );
////
////    }
////
////    return this;
////
////}
////
////raycast( raycaster, intersects ) {
////
////const geometry = this.geometry;
////const matrixWorld = this.matrixWorld;
////const threshold = raycaster.params.Line.threshold;
////const drawRange = geometry.drawRange;
////
////// Checking boundingSphere distance to ray
////
////if ( geometry.boundingSphere === null ) geometry.computeBoundingSphere();
////
////_sphere.copy( geometry.boundingSphere );
////_sphere.applyMatrix4( matrixWorld );
////_sphere.radius += threshold;
////
////if ( raycaster.ray.intersectsSphere( _sphere ) === false ) return;
////
//////
////
////_inverseMatrix.copy( matrixWorld ).invert();
////_ray.copy( raycaster.ray ).applyMatrix4( _inverseMatrix );
////
////const localThreshold = threshold / ( ( this.scale.x + this.scale.y + this.scale.z ) / 3 );
////const localThresholdSq = localThreshold * localThreshold;
////
////const vStart = new Vector3();
////const vEnd = new Vector3();
////const interSegment = new Vector3();
////const interRay = new Vector3();
////const step = this.isLineSegments ? 2 : 1;
////
////if ( geometry.isBufferGeometry ) {
////
////const index = geometry.index;
////const attributes = geometry.attributes;
////const positionAttribute = attributes.position;
////
////if ( index !== null ) {
////
////const start = Math.max( 0, drawRange.start );
////const end = Math.min( index.count, ( drawRange.start + drawRange.count ) );
////
////for ( let i = start, l = end - 1; i < l; i += step ) {
////
////const a = index.getX( i );
////const b = index.getX( i + 1 );
////
////vStart.fromBufferAttribute( positionAttribute, a );
////vEnd.fromBufferAttribute( positionAttribute, b );
////
////const distSq = _ray.distanceSqToSegment( vStart, vEnd, interRay, interSegment );
////
////if ( distSq > localThresholdSq ) continue;
////
////interRay.applyMatrix4( this.matrixWorld ); //Move back to world space for distance calculation
////
////const distance = raycaster.ray.origin.distanceTo( interRay );
////
////if ( distance < raycaster.near || distance > raycaster.far ) continue;
////
////intersects.push( {
////
////distance: distance,
////// What do we want? intersection point on the ray or on the segment??
////// point: raycaster.ray.at( distance ),
////point: interSegment.clone().applyMatrix4( this.matrixWorld ),
////index: i,
////face: null,
////faceIndex: null,
////object: this
////
////} );
////
////}
////
////} else {
////
////const start = Math.max( 0, drawRange.start );
////const end = Math.min( positionAttribute.count, ( drawRange.start + drawRange.count ) );
////
////for ( let i = start, l = end - 1; i < l; i += step ) {
////
////vStart.fromBufferAttribute( positionAttribute, i );
////vEnd.fromBufferAttribute( positionAttribute, i + 1 );
////
////const distSq = _ray.distanceSqToSegment( vStart, vEnd, interRay, interSegment );
////
////if ( distSq > localThresholdSq ) continue;
////
////interRay.applyMatrix4( this.matrixWorld ); //Move back to world space for distance calculation
////
////const distance = raycaster.ray.origin.distanceTo( interRay );
////
////if ( distance < raycaster.near || distance > raycaster.far ) continue;
////
////intersects.push( {
////
////distance: distance,
////// What do we want? intersection point on the ray or on the segment??
////// point: raycaster.ray.at( distance ),
////point: interSegment.clone().applyMatrix4( this.matrixWorld ),
////index: i,
////face: null,
////faceIndex: null,
////object: this
////
////} );
////
////}
////
////}
////
////} else if ( geometry.isGeometry ) {
////
////console.error( 'THREE.Line.raycast() no longer supports THREE.Geometry. Use THREE.BufferGeometry instead.' );
////
////}
////
////}
//
////}

#endif //THREE_CPP_SRC_OBJECTS_LINE_H
