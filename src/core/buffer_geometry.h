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
#include <cmath>
#include <climits>

class Object3D;
class DirectGeometry;

class BufferGeometry : public Geometry {
public:
    BufferGeometry();

    BufferGeometry(const BufferGeometry& source);

    BufferGeometry(const BufferAttribute<float>::sptr& position, const BufferAttribute<float>::sptr& color);

    using sptr = std::shared_ptr<BufferGeometry>;

    static sptr create() {
        return std::make_shared<BufferGeometry>();
    }

    static sptr create(const BufferAttribute<float>::sptr& position, const BufferAttribute<float>::sptr& color) {
        return std::make_shared<BufferGeometry>(position, color);
    }

    BufferAttribute<unsigned>::sptr index;

    /*BufferAttribute<float>::ptr position;

    BufferAttribute<float>::ptr normal;

    BufferAttribute<float>::ptr color;

    BufferAttribute<float>::ptr uv;

    BufferAttribute<float>::ptr uv2;*/



    /*BufferAttribute<float>::ptr tangent;

    BufferAttribute<float>::ptr bitangent;

    BufferAttribute<float>::ptr lineDistance;*/

    std::vector<threecpp::DrawRange> drawCalls;

    std::vector<threecpp::DrawRange> offsets;

    /*BufferAttribute<float>::ptr skinIndex;

    BufferAttribute<float>::ptr skinWeight;*/

    std::unordered_map<std::string, std::vector<std::shared_ptr<BufferAttribute<float>>>> morphAttributes;

    //for only morph attribute + i
    std::unordered_map<AttributeNameKey,std::shared_ptr<BufferAttribute<float>>> attributes;

    //std::unordered_map<IndexedAttributeKey, BufferAttribute<float>::ptr> indexedAttribute;

    bool morphTargetsRelative = false;

    threecpp::DrawRange drawRange;// = DrawRange() { start = 0, materialIndex = -1, count = std::numeric_limits<int>::infinity() }

    std::vector<threecpp::DrawRange> groups;

    const BufferAttribute<unsigned>::sptr& getIndex();

    BufferGeometry& setIndex(const BufferAttribute<unsigned>::sptr& indices);

    /*BufferGeometry& setPosition(const BufferAttribute<float>::ptr& position);

    BufferGeometry& setNormal(const BufferAttribute<float>::ptr& normal);

    BufferGeometry& setColor(const BufferAttribute<float>::ptr& color);

    BufferGeometry& setUV(const BufferAttribute<float>::ptr& uv);

    BufferGeometry& setUV2(const BufferAttribute<float>::ptr& uv2);

    BufferGeometry& setTangent(const BufferAttribute<float>::ptr& tangent);

    BufferGeometry& setBitangent(const BufferAttribute<float>::ptr& bitangent);

    BufferGeometry& setLineDistance(const BufferAttribute<float>::ptr& lineDistance);

    BufferGeometry& setSkinIndexAttribute(const BufferAttribute<float>::ptr& skinIndexAttribute);

    BufferGeometry& setSkinWeightAttribute(const BufferAttribute<float>::ptr skinWeightAttribute);*/

    BufferAttribute<float>::sptr& getAttribute(AttributeName name);

    BufferAttribute<float>::sptr& getAttribute(const std::string& name);

    BufferAttribute<float>::sptr& getAttribute(AttributeName name,size_t index) {
        return attributes[{name, index}];
    }
    void setAttribute(AttributeName name, const BufferAttribute<float>::sptr& attribute) {
        attributes[{name, 0}] = attribute;
    }
    void setAttribute(AttributeName name, size_t index, const BufferAttribute<float>::sptr attribute) {
        attributes[{name, index}]= attribute;
    }
    void deleteAttribute(AttributeName name,size_t index) {
        attributes.erase({ name,index });
    }
    //bool containsAttribute(const std::string &name) {
    //	size_t key = getStringHashcode(name);
    //	if (attributes.count(key) > 0) return true;
    //	return false;
    //}
    //template<typename T>
    //const std::shared_ptr<BufferAttribute<T>>& getAttribute(const std::string& name) {
    //
    //	size_t key = getStringHashcode(name);
    //	if (attributes.count(key) > 0)
    //		return attributes[key];
    //	else
    //	{
    //		std::shared_ptr<BufferAttribute<T>> nullAttribute = nullptr;
    //		return nullAttribute;
    //	}
    //}

    //template<typename T>
    //void setAttribute(const std::string& name, const std::shared_ptr<BufferAttribute<T>>& attribute) {
    //	size_t key = getStringHashcode(name);
    //	attributes.insert_or_assign(key, attribute);
    //	/*if (attributes.count(key) > 0) {
    //		std::shared_ptr<BufferAttribute<T>> temp = std::any_cast<std::shared_ptr<BufferAttribute<T>>>(attributes[key]);
    //		delete temp.get();
    //		temp.reset();
    //	}

    //	attributes[key] = attribute;*/
    //}

    void deleteAttribute(AttributeName name);

    virtual void addGroup(const unsigned start, const unsigned count, const int materialIndex);

    void clearGroups();

    void setDrawRange(const unsigned start, const unsigned count);


    virtual BufferGeometry& applyMatrix4(const Matrix4& matrix);

    virtual BufferGeometry& rotateX(const float angle);

    virtual BufferGeometry& rotateY(const float angle);

    virtual BufferGeometry& rotateZ(const float angle);

    virtual BufferGeometry& translate(const float x, const float y, const float z);

    virtual BufferGeometry& scale(const float x, const float y, const float z);

    virtual BufferGeometry& lookAt(const Vector3& vector);

    virtual BufferGeometry& center();

    virtual BufferGeometry& setFromObject(const Object3D& object);

    virtual BufferGeometry& setFromPoints(const std::vector<Vector3>& points);

    virtual BufferGeometry& updateFromObject(const Object3D& object);

    virtual BufferGeometry& fromGeometry(Geometry& geometry);

    virtual BufferGeometry& fromDirectGeometry(DirectGeometry& geometry);

    virtual void computeBoundingBox() override;

    virtual void computeBoundingSphere() override;

    virtual void computeVertexNormals(bool areaWeighted = false) override;

    virtual BufferGeometry& merge(BufferGeometry& geometry, int offset);

    virtual void normalizeNormals();

    virtual void computeFaceNormals() override;

    virtual BufferGeometry* clone() override ;

    virtual BufferGeometry& copy(const BufferGeometry& source);

    virtual void toNonIndexed(const BufferGeometry::sptr& geometry2);


    virtual const BufferAttribute<unsigned>::sptr& mergeAttributeInt(const BufferAttribute<unsigned>::sptr& source, const BufferAttribute<unsigned>::sptr& target, int offset);

    virtual const BufferAttribute<float>::sptr& mergeAttribute(const BufferAttribute<float>::sptr& source, const BufferAttribute<float>::sptr& target, int offset);


};

//using std::string;
//using std::vector;
//using std::map;
//using std::shared_ptr;

//template <typename T1,typename T2>
//class BufferGeometry : EventDispatcher {
//    private:
//        static int _id;
//
//    public:
//        bool isBufferGeometry{true};
//        int id;
//        string uuid;
//        string name;
//        string type;
//        shared_ptr<BufferAttribute<T1>> index;
//        map<string,shared_ptr<BufferAttribute<T2>>> attributes;
//        map<string,shared_ptr<BufferAttribute<T2>>> morphAttributes;
//        bool morphTargetsRelative;
//
//        shared_ptr<Box3> boundingBox;
//        shared_ptr<Sphere> boundingSphere;
//        vector<threecpp::GeometryGroup> groups;
//        threecpp::Range drawRange;
//        //use map<string,double> as parameters storage type
//        std::map<std::string,threecpp::GeometryValue> parameters;
//
//        // constructor() {
//
//        // 	super();
//
//        // 	Object.defineProperty( this, 'id', { value: _id ++ } );
//
//        // 	this.uuid = MathUtils.generateUUID();
//
//        // 	this.name = '';
//        // 	this.type = 'BufferGeometry';
//
//        // 	this.index = null;
//        // 	this.attributes = {};
//
//        // 	this.morphAttributes = {};
//        // 	this.morphTargetsRelative = false;
//
//        // 	this.groups = [];
//
//        // 	this.boundingBox = null;
//        // 	this.boundingSphere = null;
//
//        // 	this.drawRange = { start: 0, count: Infinity };
//
//        // 	this.userData = {};
//
//        // }
//        BufferGeometry():uuid(MathUtils::generate_uuid()),name(""),id(_id++),
//                            type("BufferGeometry"),index(nullptr),
//                            attributes({}),morphAttributes({}),
//                            morphTargetsRelative(false),groups({}),
//                            boundingBox(nullptr),boundingSphere(nullptr),
//                            drawRange({0,INT32_MAX}){}
//
//
//
//        shared_ptr<vector<T1>> getIndex() {
//            return index;
//        }
//
//        BufferGeometry& setIndex( BufferAttribute<T1>& indexData ) {
//            index = std::make_shared<BufferAttribute<T1>>(indexData);
//            return *this;
//        }
//
//        BufferGeometry& setRawIndex( vector<T1>& indexRawData ) {
//            BufferAttribute<T1> baIndex = BufferAttribute<T1>(indexRawData,1);
//            index = std::make_shared<BufferAttribute<T1>>(baIndex);
//            return *this;
//        }
//
//        shared_ptr<BufferAttribute<T2>> getAttribute( string name ) {
//            return attributes[ name ];
//        }
//
//        BufferGeometry& setAttribute( string name,shared_ptr<BufferAttribute<T2>> attribute ) {
//            attributes[ name ] = attribute;
//
//            return *this;
//        }
//
//        BufferGeometry& deleteAttribute( string name ) {
//            attributes.erase(name);
//
//            return *this;
//        }
//
//        bool hasAttribute( string name ) {
//            return attributes.find(name)!=attributes.end();
//        }
//
//		BufferGeometry& addGroup( int start, int count, int materialIndex = 0 ) {
//			groups.push_back( {start,count,materialIndex} );
//
//			return *this;
//		}
//
//		BufferGeometry& clearGroups() {
//			groups.clear();
//		}
//
//		BufferGeometry& setDrawRange( int start, int count ) {
//			this->drawRange.start = start;
//			this->drawRange.count = count;
//
//			return *this;
//		}
//
//		BufferGeometry& applyMatrix4( Matrix4& matrix ) {
//			shared_ptr<BufferAttribute<T2>> position = attributes[position];
//
//			if ( position != nullptr ) {
//				position->applyMatrix4( matrix );
//				position->needsUpdate = true;
//			}
//
//			shared_ptr<BufferAttribute<T2>> normal = attributes[normal];
//
//			if ( normal != nullptr ) {
//				Matrix3& normalMatrix = Matrix3().getNormalMatrix( matrix );
//				normal->applyNormalMatrix( normalMatrix );
//				normal->needsUpdate = true;
//			}
//
//			shared_ptr<BufferAttribute<T2>> tangent = attributes[tangent];
//
//			if ( tangent != nullptr ) {
//				tangent->transformDirection( matrix );
//				tangent->needsUpdate = true;
//			}
//
//			if ( boundingBox != nullptr ) {
//				computeBoundingBox();
//			}
//
//			if ( boundingSphere != nullptr ) {
//				computeBoundingSphere();
//			}
//
//			return *this;
//		}
//
//		BufferGeometry& applyQuaternion( Quaternion& q );
//
//		BufferGeometry& rotateX( double angle );
//
//		BufferGeometry& rotateY( double angle );
//
//		BufferGeometry& rotateZ( double angle );
//
//		BufferGeometry& translate( double x, double y, double z );
//
//		BufferGeometry& scale( double x, double y, double z );
//
//		BufferGeometry& lookAt( Vector3& vector );
//
//		BufferGeometry& center();
//
//		BufferGeometry& setFromPoints( vector<Vector3> points );
//
//		BufferGeometry& computeBoundingBox();
//
//		BufferGeometry& computeBoundingSphere();
//
//		BufferGeometry& computeTangents() {
//			// based on http://www.terathon.com/code/tangent.html
//			// (per vertex tangents)
//
//			if ( index == nullptr ||
//				attributes["position"] == nullptr ||
//				attributes["normal"] == nullptr ||
//				attributes["uv"] == nullptr ) {
//				throw std::runtime_error("THREECPP.BufferGeometry: .computeTangents() failed. Missing required attributes (index, position, normal or uv)" );
//			}
//
//			vector<T1> indices = index->arrayVec;
//			vector<T2> positions = attributes["position"]->arrayVec;
//			vector<T2> normals = attributes["normal"]->arrayVec;
//			vector<T2> uvs = attributes["uv"]->arrayVec;
//
//			int nVertices = positions.length / 3;
//
//			if ( hasAttribute( "tangent" ) == false ) {
//				setAttribute( "tangent", std::make_shared<BufferAttribute<float>>(vector<float>(4*nVertices),4));
//			}
//
//			vector<T2> tangents = getAttribute( "tangent" )->arrayVec;
//
//			vector<Vector3> tan1 = {}, tan2 = {};
//
//			for ( int i = 0; i < nVertices; i ++ ) {
//				tan1[ i ] = Vector3();
//				tan2[ i ] = Vector3();
//			}
//
//			Vector3 vA,
//				    vB,
//				    vC;
//
//			Vector2	uvA,
//				    uvB,
//				    uvC;
//
//			Vector3	sdir,
//				    tdir;
//
//			struct HandleTriangle{
//				void operator()(int a,int b,int c){
//					vA.fromArrayVec( positions, a * 3 );
//					vB.fromArrayVec( positions, b * 3 );
//					vC.fromArrayVec( positions, c * 3 );
//
//					uvA.fromArray( uvs, a * 2 );
//					uvB.fromArray( uvs, b * 2 );
//					uvC.fromArray( uvs, c * 2 );
//
//					vB.sub( vA );
//					vC.sub( vA );
//
//					uvB.sub( uvA );
//					uvC.sub( uvA );
//
//					const double r = 1.0 / ( uvB.x * uvC.y - uvC.x * uvB.y );
//
//					// silently ignore degenerate uv triangles having coincident or colinear vertices
//					if ( !isfinite(r)  ) return;
//
//					sdir.copy( vB ).multiplyScalar( uvC.y ).addScaledVector( vC, - uvB.y ).multiplyScalar( r );
//					tdir.copy( vC ).multiplyScalar( uvB.x ).addScaledVector( vB, - uvC.x ).multiplyScalar( r );
//
//					tan1[ a ].add( sdir );
//					tan1[ b ].add( sdir );
//					tan1[ c ].add( sdir );
//
//					tan2[ a ].add( tdir );
//					tan2[ b ].add( tdir );
//					tan2[ c ].add( tdir );
//				}
//			};
//
//			if ( groups.size() == 0 ) {
//				groups = { {
//					0,
//					indices.length
//				} };
//			}
//
//			for ( int i = 0, il = groups.size(); i < il; ++ i ) {
//
//				threecpp::GeometryGroup group = groups[ i ];
//
//				int start = group.start;
//				int count = group.count;
//
//				for ( int j = start, jl = start + count; j < jl; j += 3 ) {
//					HandleTriangle(
//						indices[ j + 0 ],
//						indices[ j + 1 ],
//						indices[ j + 2 ]
//					);
//				}
//			}
//
//			Vector3 tmp, tmp2,n, n2;
//
//			struct HandleVertex{
//				void operator()(int v){
//					n.fromArray( normals, v * 3 );
//					n2.copy( n );
//
//					Vector3 t = tan1[ v ];
//
//					// Gram-Schmidt orthogonalize
//					tmp.copy( t );
//					tmp.sub( n.multiplyScalar( n.dot( t ) ) ).normalize();
//
//					// Calculate handedness
//					tmp2.crossVectors( n2, t );
//					const double test = tmp2.dot( tan2[ v ] );
//					const double w = ( test < 0.0 ) ? - 1.0 : 1.0;
//
//					tangents[ v * 4 ] = tmp.x;
//					tangents[ v * 4 + 1 ] = tmp.y;
//					tangents[ v * 4 + 2 ] = tmp.z;
//					tangents[ v * 4 + 3 ] = w;
//				}
//			};
//
//			for ( int i = 0, il = groups.size(); i < il; ++ i ) {
//
//				threecpp::GeometryGroup group = groups[ i ];
//
//				const int start = group.start;
//				const int count = group.count;
//
//				for ( int j = start, jl = start + count; j < jl; j += 3 ) {
//					HandleVertex( (int)indices[ j + 1 ] );
//					HandleVertex( (int)indices[ j + 2 ] );
//					HandleVertex( (int)indices[ j + 0 ] );
//				}
//			}
//
//		}
//
//		BufferGeometry& computeVertexNormals() {
//			shared_ptr<BufferAttribute<T2>> positionAttribute = getAttribute( "position" );
//
//			if ( positionAttribute != nullptr ) {
//				BufferAttribute<T2>  normalAttribute = getAttribute( "normal" );
//
//				if ( normalAttribute == nullptr ) {
//					normalAttribute = std::make_shared<BufferAttribute<T2>>(vector<float>(positionAttribute->count*3),3);//new BufferAttribute( new Float32Array( positionAttribute.count * 3 ), 3 );
//					setAttribute( "normal", normalAttribute );
//				} else {
//					// reset existing normals to zero
//					for ( int i = 0, il = normalAttribute->count; i < il; i ++ ) {
//						normalAttribute->setXYZ( i, 0, 0, 0 );
//					}
//				}
//
//				Vector3 pA, pB, pC;
//				Vector3 nA, nB, nC;
//				Vector3 cb, ab;
//
//				// indexed elements
//				if ( index ) {
//					for ( int i = 0, il = index->count; i < il; i += 3 ) {
//						T1 vA = index.getX( i + 0 );
//						T1 vB = index.getX( i + 1 );
//						T1 vC = index.getX( i + 2 );
//
//						pA.fromBufferAttribute( positionAttribute, vA );
//						pB.fromBufferAttribute( positionAttribute, vB );
//						pC.fromBufferAttribute( positionAttribute, vC );
//
//						cb.subVectors( pC, pB );
//						ab.subVectors( pA, pB );
//						cb.cross( ab );
//
//						nA.fromBufferAttribute( normalAttribute, vA );
//						nB.fromBufferAttribute( normalAttribute, vB );
//						nC.fromBufferAttribute( normalAttribute, vC );
//
//						nA.add( cb );
//						nB.add( cb );
//						nC.add( cb );
//
//						normalAttribute->setXYZ( vA, nA.x, nA.y, nA.z );
//						normalAttribute->setXYZ( vB, nB.x, nB.y, nB.z );
//						normalAttribute->setXYZ( vC, nC.x, nC.y, nC.z );
//					}
//				} else {
//					// non-indexed elements (unconnected triangle soup)
//					for ( int i = 0, il = positionAttribute->count; i < il; i += 3 ) {
//
//						pA.fromBufferAttribute( positionAttribute, i + 0 );
//						pB.fromBufferAttribute( positionAttribute, i + 1 );
//						pC.fromBufferAttribute( positionAttribute, i + 2 );
//
//						cb.subVectors( pC, pB );
//						ab.subVectors( pA, pB );
//						cb.cross( ab );
//
//						normalAttribute.setXYZ( i + 0, cb.x, cb.y, cb.z );
//						normalAttribute.setXYZ( i + 1, cb.x, cb.y, cb.z );
//						normalAttribute.setXYZ( i + 2, cb.x, cb.y, cb.z );
//					}
//
//				}
//
//				normalizeNormals();
//				normalAttribute->needsUpdate = true;
//			}
//		}
//
//		BufferGeometry& merge( BufferGeometry& geometry, int offset ) {
//			if ( offset == 0 ) {
//				throw std::runtime_error(
//					"THREECPP.BufferGeometry.merge(): Overwriting original geometry, starting at offset=0. \n"
//					 "Use BufferGeometryUtils.mergeBufferGeometries() for lossless merge."
//				);
//			}
//
//			for ( auto& attribute : attributes ) {
//
//				if ( geometry.attributes[ attribute.first ] == nullptr ) continue;
//
//				shared_ptr<BufferAttribute<T2>> attribute1 = attributes[ attribute.first ];
//				vector<T2>& attributeArray1 = attribute1->arrayVec;
//
//				shared_ptr<BufferAttribute<T2>> attribute2 = geometry.attributes[ attribute.first ];
//				vector<T2>& attributeArray2 = attribute2->arrayVec;
//
//				const int attributeOffset = attribute2->itemSize * offset;
//				const int length = fmin( attributeArray2.size(), attributeArray1.size() - attributeOffset );
//
//				for ( int i = 0, j = attributeOffset; i < length; i ++, j ++ ) {
//					attributeArray1[ j ] = attributeArray2[ i ];
//				}
//			}
//			return *this;
//		}
//
//		BufferGeometry& normalizeNormals();
//
////		 BufferAttribute& toNonIndexed() {
////		 	struct ConvertBufferAttribute{
////		 		BufferAttribute<T2> operator()(BufferAttribute<T2>& attribute,BufferAttribute<T1> indices) {
////		 			vector<T2>& array = attribute.arrayVec;
////		 			int itemSize = attribute.itemSize;
////		 			bool normalized = attribute.normalized;
////
////		 			vector<T2> array2 = vector<T2>( indices.length * itemSize );
////
////		 			int index = 0, index2 = 0;
////
////		 			for ( int i = 0, l = indices.count; i < l; i ++ ) {
////		 				// if ( attribute.isInterleavedBufferAttribute ) {
////		 				// 	index = indices[ i ] * attribute.data.stride + attribute.offset;
////		 				// } else {
////		 					index = indices[ i ] * itemSize;
////		 				// }
////
////		 				for ( int j = 0; j < itemSize; j ++ ) {
////		 					array2[ index2 ++ ] = array[ index ++ ];
////		 				}
////
////		 			}
////
////		 			return BufferAttribute<T2>( array2, itemSize, normalized );
////		 		}
////		 	};
//
//		 	// function convertBufferAttribute( attribute, indices ) {
//
//		 	// 	const array = attribute.array;
//		 	// 	const itemSize = attribute.itemSize;
//		 	// 	const normalized = attribute.normalized;
//
//		 	// 	const array2 = new array.constructor( indices.length * itemSize );
//
//		 	// 	let index = 0, index2 = 0;
//
//		 	// 	for ( let i = 0, l = indices.length; i < l; i ++ ) {
//
//		 	// 		if ( attribute.isInterleavedBufferAttribute ) {
//
//		 	// 			index = indices[ i ] * attribute.data.stride + attribute.offset;
//
//		 	// 		} else {
//
//		 	// 			index = indices[ i ] * itemSize;
//
//		 	// 		}
//
//		 	// 		for ( let j = 0; j < itemSize; j ++ ) {
//
//		 	// 			array2[ index2 ++ ] = array[ index ++ ];
//
//		 	// 		}
//
//		 	// 	}
//
//		 	// 	return new BufferAttribute( array2, itemSize, normalized );
//
//		 	// }
//
//		 	//
//
////		 	if ( index == nullptr ) {
////		 		throw std::runtime_error( "THREECPP.BufferGeometry.toNonIndexed(): BufferGeometry is already non-indexed." );
////		 	}
////
////		 	BufferGeometry geometry2 = new BufferGeometry();
////
////		 	vector<T1> indices = index->arrayVec;
////		 	// attributes
////
////		 	for ( auto attr : attributes ) {
////		 		shared_ptr<BufferAttribute<T2>> attribute = attributes[ attr.first ];
////
////		 		<BufferAttribute<T2>> newAttribute = ConvertBufferAttribute( attribute, indices );
////
////		 		geometry2.setAttribute( name, newAttribute );
////		 	}
////
////		 	// morph attributes
////		 	for ( auto& morphAttr : morphAttributes ) {
////
////		 		shared_ptr<BufferAttribute<T2>> morphAttribute = morphAttributes[ morphAttr.first ]; // morphAttribute: array of Float32BufferAttributes
////
////		 		for ( int i = 0, il = morphAttribute->count; i < il; i ++ ) {
////
////		 			const attribute = morphAttribute->arrayVec[ i ];
////
////		 			const newAttribute = convertBufferAttribute( attribute, indices );
////
////		 			morphArray.push( newAttribute );
////
////		 		}
////
////		 		geometry2.morphAttributes[ name ] = morphArray;
////
////		 	}
////
////		 	geometry2.morphTargetsRelative = this.morphTargetsRelative;
////
////		 	// groups
////
////		 	const groups = this.groups;
////
////		 	for ( let i = 0, l = groups.length; i < l; i ++ ) {
////
////		 		const group = groups[ i ];
////		 		geometry2.addGroup( group.start, group.count, group.materialIndex );
////
////		 	}
////
////		 	return geometry2;
////
////		 }
//
//	// toJSON() {
//
//	// 	const data = {
//	// 		metadata: {
//	// 			version: 4.5,
//	// 			type: 'BufferGeometry',
//	// 			generator: 'BufferGeometry.toJSON'
//	// 		}
//	// 	};
//
//	// 	// standard BufferGeometry serialization
//
//	// 	data.uuid = this.uuid;
//	// 	data.type = this.type;
//	// 	if ( this.name !== '' ) data.name = this.name;
//	// 	if ( Object.keys( this.userData ).length > 0 ) data.userData = this.userData;
//
//	// 	if ( this.parameters !== undefined ) {
//
//	// 		const parameters = this.parameters;
//
//	// 		for ( const key in parameters ) {
//
//	// 			if ( parameters[ key ] !== undefined ) data[ key ] = parameters[ key ];
//
//	// 		}
//
//	// 		return data;
//
//	// 	}
//
//	// 	// for simplicity the code assumes attributes are not shared across geometries, see #15811
//
//	// 	data.data = { attributes: {} };
//
//	// 	const index = this.index;
//
//	// 	if ( index !== null ) {
//
//	// 		data.data.index = {
//	// 			type: index.array.constructor.name,
//	// 			array: Array.prototype.slice.call( index.array )
//	// 		};
//
//	// 	}
//
//	// 	const attributes = this.attributes;
//
//	// 	for ( const key in attributes ) {
//
//	// 		const attribute = attributes[ key ];
//
//	// 		data.data.attributes[ key ] = attribute.toJSON( data.data );
//
//	// 	}
//
//	// 	const morphAttributes = {};
//	// 	let hasMorphAttributes = false;
//
//	// 	for ( const key in this.morphAttributes ) {
//
//	// 		const attributeArray = this.morphAttributes[ key ];
//
//	// 		const array = [];
//
//	// 		for ( let i = 0, il = attributeArray.length; i < il; i ++ ) {
//
//	// 			const attribute = attributeArray[ i ];
//
//	// 			array.push( attribute.toJSON( data.data ) );
//
//	// 		}
//
//	// 		if ( array.length > 0 ) {
//
//	// 			morphAttributes[ key ] = array;
//
//	// 			hasMorphAttributes = true;
//
//	// 		}
//
//	// 	}
//
//	// 	if ( hasMorphAttributes ) {
//
//	// 		data.data.morphAttributes = morphAttributes;
//	// 		data.data.morphTargetsRelative = this.morphTargetsRelative;
//
//	// 	}
//
//	// 	const groups = this.groups;
//
//	// 	if ( groups.length > 0 ) {
//
//	// 		data.data.groups = JSON.parse( JSON.stringify( groups ) );
//
//	// 	}
//
//	// 	const boundingSphere = this.boundingSphere;
//
//	// 	if ( boundingSphere !== null ) {
//
//	// 		data.data.boundingSphere = {
//	// 			center: boundingSphere.center.toArray(),
//	// 			radius: boundingSphere.radius
//	// 		};
//
//	// 	}
//
//	// 	return data;
//
//	// }
//
//	// clone() {
//
//	// 	 return new this.constructor().copy( this );
//
//	// }
//
//	// copy( source ) {
//
//	// 	// reset
//
//	// 	this.index = null;
//	// 	this.attributes = {};
//	// 	this.morphAttributes = {};
//	// 	this.groups = [];
//	// 	this.boundingBox = null;
//	// 	this.boundingSphere = null;
//
//	// 	// used for storing cloned, shared data
//
//	// 	const data = {};
//
//	// 	// name
//
//	// 	this.name = source.name;
//
//	// 	// index
//
//	// 	const index = source.index;
//
//	// 	if ( index !== null ) {
//
//	// 		this.setIndex( index.clone( data ) );
//
//	// 	}
//
//	// 	// attributes
//
//	// 	const attributes = source.attributes;
//
//	// 	for ( const name in attributes ) {
//
//	// 		const attribute = attributes[ name ];
//	// 		this.setAttribute( name, attribute.clone( data ) );
//
//	// 	}
//
//	// 	// morph attributes
//
//	// 	const morphAttributes = source.morphAttributes;
//
//	// 	for ( const name in morphAttributes ) {
//
//	// 		const array = [];
//	// 		const morphAttribute = morphAttributes[ name ]; // morphAttribute: array of Float32BufferAttributes
//
//	// 		for ( let i = 0, l = morphAttribute.length; i < l; i ++ ) {
//
//	// 			array.push( morphAttribute[ i ].clone( data ) );
//
//	// 		}
//
//	// 		this.morphAttributes[ name ] = array;
//
//	// 	}
//
//	// 	this.morphTargetsRelative = source.morphTargetsRelative;
//
//	// 	// groups
//
//	// 	const groups = source.groups;
//
//	// 	for ( let i = 0, l = groups.length; i < l; i ++ ) {
//
//	// 		const group = groups[ i ];
//	// 		this.addGroup( group.start, group.count, group.materialIndex );
//
//	// 	}
//
//	// 	// bounding box
//
//	// 	const boundingBox = source.boundingBox;
//
//	// 	if ( boundingBox !== null ) {
//
//	// 		this.boundingBox = boundingBox.clone();
//
//	// 	}
//
//	// 	// bounding sphere
//
//	// 	const boundingSphere = source.boundingSphere;
//
//	// 	if ( boundingSphere !== null ) {
//
//	// 		this.boundingSphere = boundingSphere.clone();
//
//	// 	}
//
//	// 	// draw range
//
//	// 	this.drawRange.start = source.drawRange.start;
//	// 	this.drawRange.count = source.drawRange.count;
//
//	// 	// user data
//
//	// 	this.userData = source.userData;
//
//	// 	// geometry generator parameters
//
//	// 	if ( source.parameters !== undefined ) this.parameters = Object.assign( {}, source.parameters );
//
//	// 	return this;
//
//	// }
//
//	// dispose() {
//
//	// 	this.dispatchEvent( { type: 'dispose' } );
//
//	// }
//
//};


#endif /* SRC_CORE_BUFFER_GEOMETRY_H_ */
