#ifndef BUFFER_ATTRIBUTE_H
#define BUFFER_ATTRIBUTE_H
#include "vector3.h"
#include "vector4.h"
#include "vector2.h"
#include "constants.h"

//#include <ranges>
#include <algorithm>
#include <string>
#include <vector>

static Vector3 _vector;
static Vector2 _vector2;

typedef std::function<void()> uploadCallback;

using std::vector;

template <typename T>
class BufferAttribute {
    private:
        std::string name;
        int itemSize;
        int count;
        bool normalized;
        int usage;
        //std::ranges::range<int> updateRange();
        int version;

		uploadCallback onUploadCallback;
        


    public:
        vector<T> arrayVec;

		BufferAttribute()=default;
		
        BufferAttribute(vector<T> arrVec,int itemSize,bool normalized = true):arrayVec(arrVec),itemSize(itemSize),normalized(normalized),name(""),usage(StaticDrawUsage),version(0){
            count = arrVec.size()/itemSize;
            //updateRange = std::ranges:range<int>(0,0);
        }

	// constructor( array, itemSize, normalized ) {

	// 	if ( Array.isArray( array ) ) {

	// 		throw new TypeError( 'THREE.BufferAttribute: array should be a Typed Array.' );

	// 	}

	// 	this.name = '';

	// 	this.array = array;
	// 	this.itemSize = itemSize;
	// 	this.count = array !== undefined ? array.length / itemSize : 0;
	// 	this.normalized = normalized === true;

	// 	this.usage = StaticDrawUsage;
	// 	this.updateRange = { offset: 0, count: - 1 };

	// 	this.version = 0;

	// }

        BufferAttribute& needsUpdate(bool updateFlag){
            if(updateFlag)
                version++;

            return *this;
        }

        BufferAttribute& setUsage(int usageValue) {
            usage = usageValue;

            return *this;
        }

        BufferAttribute& copy(BufferAttribute& source) {

            name = source.name;
            arrayVec =  source.arrayVec;
            itemSize = source.itemSize;
            count = source.count;
            normalized = source.normalized;

            usage = source.usage;

            return *this;
        }

        BufferAttribute& copyAt(int& index1, BufferAttribute& attribute, int& index2){
            index1 *= itemSize;
            index2 *= attribute.itemSize;

            for(int i = 0, l = itemSize; i < l; i ++){
                arrayVec[ index1 + i ] = attribute.arrayVec[ index2 + i ];
            }

            return *this;
        }

        BufferAttribute& copyArray(vector<T> arrVec) {
            arrayVec.copy(arrVec);

            return *this;
        }

	// copyColorsArray( colors ) {

	// 	const array = this.array;
	// 	let offset = 0;

	// 	for ( let i = 0, l = colors.length; i < l; i ++ ) {

	// 		let color = colors[ i ];

	// 		if ( color === undefined ) {

	// 			console.warn( 'THREE.BufferAttribute.copyColorsArray(): color is undefined', i );
	// 			color = new Color();

	// 		}

	// 		array[ offset ++ ] = color.r;
	// 		array[ offset ++ ] = color.g;
	// 		array[ offset ++ ] = color.b;

	// 	}

	// 	return this;

	// }

        BufferAttribute& copyVector2sArray(vector<Vector2> vectors) {
            const vector<T> array = arrayVec;
            int offset = 0;

            for (int i = 0, l = vectors.size(); i < l; i ++ ) {
                Vector2 vector = vectors[ i ];

                array[ offset ++ ] = vector.x;
                array[ offset ++ ] = vector.y;
            }

            return *this;
        }

        BufferAttribute& copyVector3sArray(vector<Vector3> vectors) {
            const vector<T> array = arrayVec;
            int offset = 0;

            for ( int i = 0, l = vectors.size(); i < l; i ++ ) {

                Vector3 vector = vectors[ i ];

                array[ offset ++ ] = vector.x;
                array[ offset ++ ] = vector.y;
                array[ offset ++ ] = vector.z;
            }

            return *this;
        }

        BufferAttribute& copyVector4sArray(vector<Vector4d> vectors) {
            const vector<T> array = arrayVec;
            int offset = 0;

            for ( int i = 0, l = vectors.size(); i < l; i ++ ) {

                Vector4d vector = vectors[ i ];

                array[ offset ++ ] = vector.x;
                array[ offset ++ ] = vector.y;
                array[ offset ++ ] = vector.z;
                array[ offset ++ ] = vector.w;
            }

            return *this;
        }
        

		BufferAttribute& applyMatrix3(Matrix3& m) {
			if ( itemSize == 2 ) {

				for ( int i = 0, l = this->count; i < l; i ++ ) {
					_vector2.fromBufferAttribute( *this, i );
					_vector2.applyMatrix3( m );

					setXY( i, _vector2.x, _vector2.y );
				}

			} else if ( itemSize == 3 ) {

				for ( int i = 0, l = this->count; i < l; i ++ ) {
					_vector.fromBufferAttribute( *this, i );
					_vector.applyMatrix3( m );

					setXYZ( i, _vector.x, _vector.y, _vector.z );
				}

			}

			return *this;
		}

		BufferAttribute& applyMatrix4(Matrix4& m) {
			for ( int i = 0, l = this->count; i < l; i ++ ) {

				_vector.fromBufferAttribute( this, i );
				_vector.applyMatrix4( m );

				setXYZ( i, _vector.x, _vector.y, _vector.z );
			}

			return *this;
		}

		BufferAttribute& applyNormalMatrix(Matrix4& m) {

			for ( int i = 0, l = this->count; i < l; i ++ ) {
				_vector.fromBufferAttribute( *this, i );
				_vector.applyMatrix4( m ).normalize();

				setXYZ( i, _vector.x, _vector.y, _vector.z );
			}

			return *this;
		}

		BufferAttribute& transformDirection(Matrix4& m){

			for ( int i = 0, l = this->count; i < l; i ++ ) {
				_vector.fromBufferAttribute( this, i );
				_vector.transformDirection( m );

				setXYZ( i, _vector.x, _vector.y, _vector.z );
			}

			return *this;
		}

		BufferAttribute& set(vector<T> value, int offset = 0) {
			auto vBegin = value.begin();
			auto vEnd = value.end();
			vBegin += offset;
			
			copy(vBegin,vEnd,std::back_inserter(arrayVec));

			return *this;
		}

		T getX(int index) {
			return arrayVec[ index * itemSize ];
		}

		BufferAttribute& setX(int index, T x) {
			arrayVec[ index * itemSize ] = x;
			return *this;
		}

		T getY(int index) {
			return arrayVec[ index * itemSize + 1 ];
		}

		BufferAttribute& setY(int index, T y){
			arrayVec[ index * itemSize + 1 ] = y;
			return *this;
		}

		T getZ(int index) {
			return arrayVec[ index * itemSize + 2 ];
		}

		BufferAttribute& setZ(int index,T z){
			arrayVec[ index * itemSize + 2 ] = z;
			return *this;
		}

		T getW(int index) {
			return arrayVec[ index * itemSize + 3 ];
		}

		BufferAttribute& setW(int index,T w) {
			arrayVec[ index * itemSize + 3 ] = w;
			return *this;
		}

		BufferAttribute& setXY(int index,T x,T y) {
			index *= itemSize;

			arrayVec[ index + 0 ] = x;
			arrayVec[ index + 1 ] = y;

			return *this;
		}

		BufferAttribute& setXYZ(int index,T x,T y,T z) {
			index *= itemSize;

			arrayVec[ index + 0 ] = x;
			arrayVec[ index + 1 ] = y;
			arrayVec[ index + 2 ] = z;

			return *this;
		}

		BufferAttribute& setXYZW(int index,T x,T y,T z,T w) {
			index *= itemSize;

			arrayVec[ index + 0 ] = x;
			arrayVec[ index + 1 ] = y;
			arrayVec[ index + 2 ] = z;
			arrayVec[ index + 3 ] = w;

			return *this;
		}

		BufferAttribute& onUpload(uploadCallback callback) {
			onUploadCallback = callback;

			return *this;
		}

		BufferAttribute clone() {
			return BufferAttribute(arrayVec,itemSize);
		}

		// toJSON() {

		// 	const data = {
		// 		itemSize: this.itemSize,
		// 		type: this.array.constructor.name,
		// 		array: Array.prototype.slice.call( this.array ),
		// 		normalized: this.normalized
		// 	};

		// 	if ( this.name !== '' ) data.name = this.name;
		// 	if ( this.usage !== StaticDrawUsage ) data.usage = this.usage;
		// 	if ( this.updateRange.offset !== 0 || this.updateRange.count !== - 1 ) data.updateRange = this.updateRange;

		// 	return data;
		// }

};

typedef BufferAttribute<int8_t> Int8BufferAttribute;
typedef BufferAttribute<u_int8_t> Uint8BufferAttribute;

typedef BufferAttribute<int16_t> Int16BufferAttribute;
typedef BufferAttribute<u_int16_t> Uint16BufferAttribute;

typedef BufferAttribute<int32_t> Int32BufferAttribute;
typedef BufferAttribute<u_int32_t> Uint32BufferAttribute;

typedef BufferAttribute<float> Float16BufferAttribute;
typedef BufferAttribute<float> Float32BufferAttribute;
typedef BufferAttribute<double> Float64BufferAttribute;

// class Uint8ClampedBufferAttribute extends BufferAttribute {

// 	constructor( array, itemSize, normalized ) {

// 		super( new Uint8ClampedArray( array ), itemSize, normalized );

// 	}

// }

#endif /* BUFFER_ATTRIBUTE_H */
