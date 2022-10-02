#ifndef SRC_CORE_BUFFER_GEOMETRY_H_
#define SRC_CORE_BUFFER_GEOMETRY_H_

#include "vector3.h"
#include "vector2.h"
#include "box3.h"
#include "event_dispatcher.h"
#include "buffer_attribute.h"
#include "sphere.h"
#include "object_3d.h"
#include "matrix4.h"
#include "matrix3.h"
#include "math_utils.h"
#include "common_types.h"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <type_traits>




static int _id = 0;

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

using std::string;
using std::vector;
using std::map;
using std::shared_ptr;

template <typename T1,typename T2>
class BufferGeometry : EventDispatcher {
    private:

    public:
        bool isBufferGeometry{true};
        int id;
        string uuid;
        string name;
        string type;
        shared_ptr<BufferAttribute<T1>> index;
        map<string,shared_ptr<BufferAttribute<T2>>> attributes;
        map<string,shared_ptr<BufferAttribute<T2>>> morphAttributes;
        bool morphTargetsRelative;

        shared_ptr<Box3> boundingBox;
        shared_ptr<SphboundingSphere;
        vector<GeometryGroup> groups;
        threecpp::Range drawRange;

        BufferGeometry():uuid(generate_uuid()),name(""),
                            type("BufferGeometry"),index(nullptr),
                            attributes({}),morphAttributes({}),
                            morphTargetsRelative(false),groups({}),
                            boundingBox(nullptr),boundingSphere(nullptr),
                            drawRange({0,INT32_MAX}){}

	// constructor() {

	// 	super();

	// 	Object.defineProperty( this, 'id', { value: _id ++ } );

	// 	this.uuid = MathUtils.generateUUID();

	// 	this.name = '';
	// 	this.type = 'BufferGeometry';

	// 	this.index = null;
	// 	this.attributes = {};

	// 	this.morphAttributes = {};
	// 	this.morphTargetsRelative = false;

	// 	this.groups = [];

	// 	this.boundingBox = null;
	// 	this.boundingSphere = null;

	// 	this.drawRange = { start: 0, count: Infinity };

	// 	this.userData = {};

	// }

        shared_ptr<vector<T1>> getIndex() {
            return index;
        }

        BufferGeometry& setIndex( BufferAttribute<T1>& indexData ) {
            index = std::make_shared<BufferAttribute<T1>)(indexData);
            return *this;
        }

        BufferGeometry& setRawIndex( vector<T1>& indexRawData ) {
            BufferAttribute<T1> baIndex = BufferAttribute<T1>(indexRawData,1);
            index = std::make_shared<BufferAttribute<T1>(baIndex);
            return *this;
        }

        shared_ptr<BufferAttribute<T2>> getAttribute( string name ) {
            return attributes[ name ];
        }

        BufferGeometry& setAttribute( string name,shared_ptr<BufferAttribute<T2>> attribute ) {
            attributes[ name ] = attribute;

            return *this;
        }

        BufferGeometry& deleteAttribute( string name ) {
            attributes.erase(name);

            return *this;
        }

        bool hasAttribute( string name ) {
            return attributes.find(name)!=attributes.end();
        }

		BufferGeometry& addGroup( int start, int count, int materialIndex = 0 ) {
			groups.push_back( {start,count,materialIndex} );

			return *this;	
		}

		BufferGeometry& clearGroups() {
			groups.clear();
		}

		BufferGeometry& setDrawRange( int start, int count ) {
			this->drawRange.start = start;
			this->drawRange.count = count;

			return *this;
		}

		BufferGeometry& applyMatrix4( Matrix4& matrix ) {
			shared_ptr<BufferAttribute<T2>> position = attributes[position];

			if ( position != nullptr ) {
				position->applyMatrix4( matrix );
				position->needsUpdate = true;
			}

			shared_ptr<BufferAttribute<T2>> normal = attributes[normal];

			if ( normal != nullptr ) {
				Matrix3& normalMatrix = Matrix3().getNormalMatrix( matrix );
				normal->applyNormalMatrix( normalMatrix );
				normal->needsUpdate = true;
			}

			shared_ptr<BufferAttribute<T2>> tangent = attributes[tangent];

			if ( tangent != nullptr ) {
				tangent->transformDirection( matrix );
				tangent->needsUpdate = true;
			}

			if ( boundingBox != null ) {
				computeBoundingBox();
			}

			if ( boundingSphere != null ) {
				computeBoundingSphere();
			}

			return *this;
		}

		BufferGeometry& applyQuaternion( Quaternion& q ) {
			_m1.makeRotationFromQuaternion( q );
			applyMatrix4( _m1 );

			return *this;
		}

		BufferGeometry& rotateX( double angle ) {
			// rotate geometry around world x-axis
			_m1.makeRotationX( angle );
			applyMatrix4( _m1 );

			return *this;
		}

		BufferGeometry& rotateY( double angle ) {
			// rotate geometry around world y-axis
			_m1.makeRotationY( angle );
			applyMatrix4( _m1 );

			return *this;
		}

		BufferGeometry& rotateZ( double angle ) {
			// rotate geometry around world z-axis
			_m1.makeRotationZ( angle );
			applyMatrix4( _m1 );

			return *this;
		}

		BufferGeometry& translate( double x, double y, double z ) {
			// translate geometry
			_m1.makeTranslation( x, y, z );
			applyMatrix4( _m1 );

			return *this;
		}

		BufferGeometry& scale( double x, double y, double z ) {
			// scale geometry
			_m1.makeScale( x, y, z );
			applyMatrix4( _m1 );

			return *this;
		}

		BufferGeometry& lookAt( Vector3& vector ) {
			_obj.lookAt( vector );
			_obj.updateMatrix();

			applyMatrix4( _obj.matrix );

			return *this;
		}

		BufferGeometry& center() {
			computeBoundingBox();
			//due to previous calling,it is impossible for boundingBox==nullptr
			boundingBox->getCenter( _offset ).negate();
			translate( _offset.x, _offset.y, _offset.z );

			return *this;
		}

		BufferGeometry& setFromPoints( vector<Vector3> points ) {
			vector<T2> positionData;

			for ( int i = 0, l = points.size; i < l; i ++ ) {
				Vector3 point = points[ i ];
				position.push_back( point.x, point.y, point.z || 0 );
			}

			setAttribute( "position", BufferAttribute<T2>( positionData, 3 ) );

			return *this;
		}

		BufferGeometry& computeBoundingBox() {

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

		BufferGeometry& computeBoundingSphere() {
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

		BufferGeometry& computeTangents() {
			// based on http://www.terathon.com/code/tangent.html
			// (per vertex tangents)

			if ( index == nullptr ||
				attributes["position"] == nullptr ||
				attributes["normal"] == nullptr ||
				attributes["uv"] == nullptr ) {
				throw std::runtime_error("THREECPP.BufferGeometry: .computeTangents() failed. Missing required attributes (index, position, normal or uv)" );
			}

			vector<T1> indices = index->arrayVec;
			vector<T2> positions = attributes["position"]->arrayVec;
			vector<T2> normals = attributes["normal"]->arrayVec;
			vector<T2> uvs = attributes["uv"]->arrayVec;

			int nVertices = positions.length / 3;

			if ( hasAttribute( "tangent" ) == false ) {
				setAttribute( "tangent", std::make_shared<BufferAttribute<float>>(vector<float>(4*nVertices),4));
			}

			vector<Vector3> tangents = getAttribute( "tangent" )->array;

			vector<Vector3> tan1 = [], tan2 = [];

			for ( int i = 0; i < nVertices; i ++ ) {
				tan1[ i ] = Vector3();
				tan2[ i ] = Vector3();
			}

			Vector3 vA(),
				    vB(),
				    vC();  

			Vector2	uvA(),
				    uvB(),
				    uvC();

			Vector3	sdir(),
				    tdir();

			struct HandleTriangle{
				void operator()(int a,int b,int c){
					vA.fromArrayVec( positions, a * 3 );
					vB.fromArrayVec( positions, b * 3 );
					vC.fromArrayVec( positions, c * 3 );

					uvA.fromArrayVec( uvs, a * 2 );
					uvB.fromArrayVec( uvs, b * 2 );
					uvC.fromArrayVec( uvs, c * 2 );

					vB.sub( vA );
					vC.sub( vA );

					uvB.sub( uvA );
					uvC.sub( uvA );

					const double r = 1.0 / ( uvB.x * uvC.y - uvC.x * uvB.y );

					// silently ignore degenerate uv triangles having coincident or colinear vertices
					if ( ! std::is_floating_point( r ) ) return;

					sdir.copy( vB ).multiplyScalar( uvC.y ).addScaledVector( vC, - uvB.y ).multiplyScalar( r );
					tdir.copy( vC ).multiplyScalar( uvB.x ).addScaledVector( vB, - uvC.x ).multiplyScalar( r );

					tan1[ a ].add( sdir );
					tan1[ b ].add( sdir );
					tan1[ c ].add( sdir );

					tan2[ a ].add( tdir );
					tan2[ b ].add( tdir );
					tan2[ c ].add( tdir );
				}
			};

			if ( groups.size() == 0 ) {
				groups = [ {
					0,
					indices.length
				} ];
			}

			for ( int i = 0, il = groups.size(); i < il; ++ i ) {

				GeometryGroup group = groups[ i ];

				int start = group.start;
				int count = group.count;

				for ( int j = start, jl = start + count; j < jl; j += 3 ) {
					HandleTriangle(
						indices[ j + 0 ],
						indices[ j + 1 ],
						indices[ j + 2 ]
					);
				}
			}

			Vector3 tmp(), tmp2(),n(), n2();

			struct HandleVertex{
				void operator()(int v){
					n.fromArray( normals, v * 3 );
					n2.copy( n );

					Vector3 t = tan1[ v ];

					// Gram-Schmidt orthogonalize
					tmp.copy( t );
					tmp.sub( n.multiplyScalar( n.dot( t ) ) ).normalize();

					// Calculate handedness
					tmp2.crossVectors( n2, t );
					const double test = tmp2.dot( tan2[ v ] );
					const double w = ( test < 0.0 ) ? - 1.0 : 1.0;

					tangents[ v * 4 ] = tmp.x;
					tangents[ v * 4 + 1 ] = tmp.y;
					tangents[ v * 4 + 2 ] = tmp.z;
					tangents[ v * 4 + 3 ] = w;
				}
			};

			for ( int i = 0, il = groups.size(); i < il; ++ i ) {

				Geometry group = groups[ i ];

				const int start = group.start;
				const int count = group.count;

				for ( int j = start, jl = start + count; j < jl; j += 3 ) {
					HandleVertex( indices[ j + 1 ] );
					HandleVertex( indices[ j + 2 ] );
					HandleVertex( indices[ j + 0 ] );
				}
			}

		}

		BufferGeometry& computeVertexNormals() {
			shared_ptr<BufferAttribute<T2>> positionAttribute = getAttribute( "position" );

			if ( positionAttribute != nullptr ) {
				BufferAttribute<T2>  normalAttribute = getAttribute( 'normal' );

				if ( normalAttribute == nullptr ) {
					normalAttribute = std::make_shared<BufferAttribute<T2>>(vector<float>(positionAttribute->count*3),3);//new BufferAttribute( new Float32Array( positionAttribute.count * 3 ), 3 );
					setAttribute( 'normal', normalAttribute );
				} else {
					// reset existing normals to zero
					for ( int i = 0, il = normalAttribute->count; i < il; i ++ ) {
						normalAttribute->setXYZ( i, 0, 0, 0 );
					}
				}

				Vector3 pA(), pB(), pC();
				Vector3 nA(), nB(), nC();
				Vector3 cb(), ab();

				// indexed elements
				if ( index ) {
					for ( int i = 0, il = index->count; i < il; i += 3 ) {
						T1 vA = index.getX( i + 0 );
						T2 vB = index.getX( i + 1 );
						T3 vC = index.getX( i + 2 );

						pA.fromBufferAttribute( positionAttribute, vA );
						pB.fromBufferAttribute( positionAttribute, vB );
						pC.fromBufferAttribute( positionAttribute, vC );

						cb.subVectors( pC, pB );
						ab.subVectors( pA, pB );
						cb.cross( ab );

						nA.fromBufferAttribute( normalAttribute, vA );
						nB.fromBufferAttribute( normalAttribute, vB );
						nC.fromBufferAttribute( normalAttribute, vC );

						nA.add( cb );
						nB.add( cb );
						nC.add( cb );

						normalAttribute->setXYZ( vA, nA.x, nA.y, nA.z );
						normalAttribute->setXYZ( vB, nB.x, nB.y, nB.z );
						normalAttribute->setXYZ( vC, nC.x, nC.y, nC.z );
					}
				} else {
					// non-indexed elements (unconnected triangle soup)
					for ( int i = 0, il = positionAttribute->count; i < il; i += 3 ) {

						pA.fromBufferAttribute( positionAttribute, i + 0 );
						pB.fromBufferAttribute( positionAttribute, i + 1 );
						pC.fromBufferAttribute( positionAttribute, i + 2 );

						cb.subVectors( pC, pB );
						ab.subVectors( pA, pB );
						cb.cross( ab );

						normalAttribute.setXYZ( i + 0, cb.x, cb.y, cb.z );
						normalAttribute.setXYZ( i + 1, cb.x, cb.y, cb.z );
						normalAttribute.setXYZ( i + 2, cb.x, cb.y, cb.z );
					}

				}

				normalizeNormals();
				normalAttribute->needsUpdate = true;
			}
		}

		BufferGeometry& merge( BufferGeometry& geometry, int offset ) {
			if ( offset == 0 ) {
				throw std::runtime_error(
					"THREECPP.BufferGeometry.merge(): Overwriting original geometry, starting at offset=0. "
					+ "Use BufferGeometryUtils.mergeBufferGeometries() for lossless merge."
				);
			}

			for ( auto& attribute : attributes ) {

				if ( geometry.attributes[ attribute.first ] == nullptr ) continue;

				shared_ptr<BufferAttribute<T2>> attribute1 = attributes[ attribute.first ];
				vector<T2>& attributeArray1 = attribute1->arrayVec;

				shared_ptr<BufferAttribute<T2>> attribute2 = geometry.attributes[ attribute.first ];
				vector<T2>& attributeArray2 = attribute2->arrayVec;

				const int attributeOffset = attribute2->itemSize * offset;
				const int length = fmin( attributeArray2.size(), attributeArray1.size() - attributeOffset );

				for ( int i = 0, j = attributeOffset; i < length; i ++, j ++ ) {
					attributeArray1[ j ] = attributeArray2[ i ];
				}
			}
			return *this;
		}

		BufferGeometry& normalizeNormals() {
			shared_ptr<BufferAttribute<T2>> normals = attributes["normal"];

			for ( int i = 0, il = normals->count; i < il; i ++ ) {
				_vector.fromBufferAttribute( *normals, i );
				_vector.normalize();
				normals->setXYZ( i, _vector.x, _vector.y, _vector.z );
			}
		}

		// BufferAttribute& toNonIndexed() {
		// 	struct ConvertBufferAttribute{
		// 		BufferAttribute<T2> operator()(BufferAttribute<T2>& attribute,BufferAttribute<T1> indices) {
		// 			vector<T2>& array = attribute.arrayVec;
		// 			int itemSize = attribute.itemSize;
		// 			bool normalized = attribute.normalized;

		// 			vector<T2> array2 = vector<T2>( indices.length * itemSize );

		// 			int index = 0, index2 = 0;

		// 			for ( int i = 0, l = indices.count; i < l; i ++ ) {
		// 				// if ( attribute.isInterleavedBufferAttribute ) {
		// 				// 	index = indices[ i ] * attribute.data.stride + attribute.offset;
		// 				// } else {
		// 					index = indices[ i ] * itemSize;
		// 				// }

		// 				for ( int j = 0; j < itemSize; j ++ ) {
		// 					array2[ index2 ++ ] = array[ index ++ ];
		// 				}

		// 			}

		// 			return BufferAttribute<T2>( array2, itemSize, normalized );
		// 		}
		// 	};

		// 	// function convertBufferAttribute( attribute, indices ) {

		// 	// 	const array = attribute.array;
		// 	// 	const itemSize = attribute.itemSize;
		// 	// 	const normalized = attribute.normalized;

		// 	// 	const array2 = new array.constructor( indices.length * itemSize );

		// 	// 	let index = 0, index2 = 0;

		// 	// 	for ( let i = 0, l = indices.length; i < l; i ++ ) {

		// 	// 		if ( attribute.isInterleavedBufferAttribute ) {

		// 	// 			index = indices[ i ] * attribute.data.stride + attribute.offset;

		// 	// 		} else {

		// 	// 			index = indices[ i ] * itemSize;

		// 	// 		}

		// 	// 		for ( let j = 0; j < itemSize; j ++ ) {

		// 	// 			array2[ index2 ++ ] = array[ index ++ ];

		// 	// 		}

		// 	// 	}

		// 	// 	return new BufferAttribute( array2, itemSize, normalized );

		// 	// }

		// 	//

		// 	if ( index == nullptr ) {
		// 		throw std::runtime_error( "THREECPP.BufferGeometry.toNonIndexed(): BufferGeometry is already non-indexed." );
		// 	}

		// 	BufferGeometry geometry2 = new BufferGeometry();

		// 	vector<T1> indices = index->arrayVec;
		// 	// attributes

		// 	for ( auto attr : attributes ) {
		// 		shared_ptr<BufferAttribute<T2>> attribute = attributes[ attr.first ];

		// 		<BufferAttribute<T2>> newAttribute = ConvertBufferAttribute( attribute, indices );

		// 		geometry2.setAttribute( name, newAttribute );
		// 	}

		// 	// morph attributes
		// 	for ( auto& morphAttr : morphAttributes ) {

		// 		shared_ptr<BufferAttribute<T2>> morphAttribute = morphAttributes[ morphAttr.first ]; // morphAttribute: array of Float32BufferAttributes

		// 		for ( int i = 0, il = morphAttribute->count; i < il; i ++ ) {

		// 			const attribute = morphAttribute->arrayVec[ i ];

		// 			const newAttribute = convertBufferAttribute( attribute, indices );

		// 			morphArray.push( newAttribute );

		// 		}

		// 		geometry2.morphAttributes[ name ] = morphArray;

		// 	}

		// 	geometry2.morphTargetsRelative = this.morphTargetsRelative;

		// 	// groups

		// 	const groups = this.groups;

		// 	for ( let i = 0, l = groups.length; i < l; i ++ ) {

		// 		const group = groups[ i ];
		// 		geometry2.addGroup( group.start, group.count, group.materialIndex );

		// 	}

		// 	return geometry2;

		// }

	// toJSON() {

	// 	const data = {
	// 		metadata: {
	// 			version: 4.5,
	// 			type: 'BufferGeometry',
	// 			generator: 'BufferGeometry.toJSON'
	// 		}
	// 	};

	// 	// standard BufferGeometry serialization

	// 	data.uuid = this.uuid;
	// 	data.type = this.type;
	// 	if ( this.name !== '' ) data.name = this.name;
	// 	if ( Object.keys( this.userData ).length > 0 ) data.userData = this.userData;

	// 	if ( this.parameters !== undefined ) {

	// 		const parameters = this.parameters;

	// 		for ( const key in parameters ) {

	// 			if ( parameters[ key ] !== undefined ) data[ key ] = parameters[ key ];

	// 		}

	// 		return data;

	// 	}

	// 	// for simplicity the code assumes attributes are not shared across geometries, see #15811

	// 	data.data = { attributes: {} };

	// 	const index = this.index;

	// 	if ( index !== null ) {

	// 		data.data.index = {
	// 			type: index.array.constructor.name,
	// 			array: Array.prototype.slice.call( index.array )
	// 		};

	// 	}

	// 	const attributes = this.attributes;

	// 	for ( const key in attributes ) {

	// 		const attribute = attributes[ key ];

	// 		data.data.attributes[ key ] = attribute.toJSON( data.data );

	// 	}

	// 	const morphAttributes = {};
	// 	let hasMorphAttributes = false;

	// 	for ( const key in this.morphAttributes ) {

	// 		const attributeArray = this.morphAttributes[ key ];

	// 		const array = [];

	// 		for ( let i = 0, il = attributeArray.length; i < il; i ++ ) {

	// 			const attribute = attributeArray[ i ];

	// 			array.push( attribute.toJSON( data.data ) );

	// 		}

	// 		if ( array.length > 0 ) {

	// 			morphAttributes[ key ] = array;

	// 			hasMorphAttributes = true;

	// 		}

	// 	}

	// 	if ( hasMorphAttributes ) {

	// 		data.data.morphAttributes = morphAttributes;
	// 		data.data.morphTargetsRelative = this.morphTargetsRelative;

	// 	}

	// 	const groups = this.groups;

	// 	if ( groups.length > 0 ) {

	// 		data.data.groups = JSON.parse( JSON.stringify( groups ) );

	// 	}

	// 	const boundingSphere = this.boundingSphere;

	// 	if ( boundingSphere !== null ) {

	// 		data.data.boundingSphere = {
	// 			center: boundingSphere.center.toArray(),
	// 			radius: boundingSphere.radius
	// 		};

	// 	}

	// 	return data;

	// }

	// clone() {

	// 	 return new this.constructor().copy( this );

	// }

	// copy( source ) {

	// 	// reset

	// 	this.index = null;
	// 	this.attributes = {};
	// 	this.morphAttributes = {};
	// 	this.groups = [];
	// 	this.boundingBox = null;
	// 	this.boundingSphere = null;

	// 	// used for storing cloned, shared data

	// 	const data = {};

	// 	// name

	// 	this.name = source.name;

	// 	// index

	// 	const index = source.index;

	// 	if ( index !== null ) {

	// 		this.setIndex( index.clone( data ) );

	// 	}

	// 	// attributes

	// 	const attributes = source.attributes;

	// 	for ( const name in attributes ) {

	// 		const attribute = attributes[ name ];
	// 		this.setAttribute( name, attribute.clone( data ) );

	// 	}

	// 	// morph attributes

	// 	const morphAttributes = source.morphAttributes;

	// 	for ( const name in morphAttributes ) {

	// 		const array = [];
	// 		const morphAttribute = morphAttributes[ name ]; // morphAttribute: array of Float32BufferAttributes

	// 		for ( let i = 0, l = morphAttribute.length; i < l; i ++ ) {

	// 			array.push( morphAttribute[ i ].clone( data ) );

	// 		}

	// 		this.morphAttributes[ name ] = array;

	// 	}

	// 	this.morphTargetsRelative = source.morphTargetsRelative;

	// 	// groups

	// 	const groups = source.groups;

	// 	for ( let i = 0, l = groups.length; i < l; i ++ ) {

	// 		const group = groups[ i ];
	// 		this.addGroup( group.start, group.count, group.materialIndex );

	// 	}

	// 	// bounding box

	// 	const boundingBox = source.boundingBox;

	// 	if ( boundingBox !== null ) {

	// 		this.boundingBox = boundingBox.clone();

	// 	}

	// 	// bounding sphere

	// 	const boundingSphere = source.boundingSphere;

	// 	if ( boundingSphere !== null ) {

	// 		this.boundingSphere = boundingSphere.clone();

	// 	}

	// 	// draw range

	// 	this.drawRange.start = source.drawRange.start;
	// 	this.drawRange.count = source.drawRange.count;

	// 	// user data

	// 	this.userData = source.userData;

	// 	// geometry generator parameters

	// 	if ( source.parameters !== undefined ) this.parameters = Object.assign( {}, source.parameters );

	// 	return this;

	// }

	// dispose() {

	// 	this.dispatchEvent( { type: 'dispose' } );

	// }

};


#endif /* SRC_CORE_BUFFER_GEOMETRY_H_ */