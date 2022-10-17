#ifndef SRC_CORE_BUFFER_ATTRIBUTE_H_
#define SRC_CORE_BUFFER_ATTRIBUTE_H_
// #include "vector3.h"
// #include "vector4.h"
// #include "vector2.h"
#include "constants.h"
#include "common_types.h"
#include "color.h"

//#include <ranges>
#include <algorithm>
#include <string>
#include <vector>
#include <functional>

class Vector3;
class Vector2;
template <typename T> class Vector4;
class Matrix3;
class Matrix4;

typedef std::function<void()> uploadCallback;

using std::vector;

template <typename T>
class BufferAttribute {
    private:
        std::string name{""};
        int itemSize{0};
        int count{0};
        bool normalized{false};
        int usage{0};
        //std::ranges::range<int> updateRange;
        threecpp::Range updateRange{0,0};
        int version{0};

		uploadCallback onUploadCallback;


    public:
        vector<T> arrayVec;

        BufferAttribute() = default;

        BufferAttribute(vector<T> arrVec, int itemSize, bool normalized = true)
            : arrayVec(arrVec), itemSize(itemSize), normalized(normalized), name(""), usage(StaticDrawUsage), version(0) {
            count = arrVec.size() / itemSize;
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

    BufferAttribute &
    needsUpdate(bool updateFlag) {
        if (updateFlag)
            version++;

        return *this;
    }

    BufferAttribute &
    setUsage(int usageValue) {
        usage = usageValue;

        return *this;
    }

    BufferAttribute &
    copy(BufferAttribute &source) {
        name = source.name;
        arrayVec = source.arrayVec;
        itemSize = source.itemSize;
        count = source.count;
        normalized = source.normalized;
        updateRange = source.updateRange;

        usage = source.usage;

        return *this;
    }

    BufferAttribute &
    copyAt(int &index1, BufferAttribute &attribute, int &index2) {
        index1 *= itemSize;
        index2 *= attribute.itemSize;

        for (int i = 0, l = itemSize; i < l; i++) {
            arrayVec[index1 + i] = attribute.arrayVec[index2 + i];
        }

        return *this;
    }

    BufferAttribute &
    copyArray(vector<T> arrVec) {
        arrayVec.copy(arrVec);

        return *this;
    }

    BufferAttribute &
    copyColorsArray(std::vector<Color> colors) {
        std::vector<T> array = arrayVec;
        int offset = 0;

        for (Color color : colors) {

            array[offset++] = color.r;
            array[offset++] = color.g;
            array[offset++] = color.b;
        }

        return *this;

    }

    BufferAttribute &
    copyVector2sArray(std::vector<Vector2> vectors);

    BufferAttribute &
    copyVector3sArray(std::vector<Vector3> vectors);

    BufferAttribute &
    copyVector4sArray(std::vector<Vector4<double>> vectors);

    BufferAttribute &
    applyMatrix3(Matrix3 &m);

    BufferAttribute &
    applyMatrix4(Matrix4 &m);

    BufferAttribute &
    applyNormalMatrix(Matrix4 &m);

    BufferAttribute &
    transformDirection(Matrix4 &m);

    BufferAttribute &
    set(vector<T> value, int offset = 0) {
        auto vBegin = value.begin();
        auto vEnd = value.end();
        vBegin += offset;

        copy(vBegin, vEnd, std::back_inserter(arrayVec));

        return *this;
    }

    T
    getX(int index) {
        return arrayVec[index * itemSize];
    }

    BufferAttribute &
    setX(int index, T x) {
        arrayVec[index * itemSize] = x;
        return *this;
    }

    T
    getY(int index) {
        return arrayVec[index * itemSize + 1];
    }

    BufferAttribute &
    setY(int index, T y) {
        arrayVec[index * itemSize + 1] = y;
        return *this;
    }

    T
    getZ(int index) {
        return arrayVec[index * itemSize + 2];
    }

    BufferAttribute &
    setZ(int index, T z) {
        arrayVec[index * itemSize + 2] = z;
        return *this;
    }

    T
    getW(int index) {
        return arrayVec[index * itemSize + 3];
    }

    BufferAttribute &
    setW(int index, T w) {
        arrayVec[index * itemSize + 3] = w;
        return *this;
    }

    BufferAttribute &
    setXY(int index, T x, T y) {
        index *= itemSize;

        arrayVec[index + 0] = x;
        arrayVec[index + 1] = y;

        return *this;
    }

    BufferAttribute &
    setXYZ(int index, T x, T y, T z) {
        index *= itemSize;

        arrayVec[index + 0] = x;
        arrayVec[index + 1] = y;
        arrayVec[index + 2] = z;

        return *this;
    }

    BufferAttribute &
    setXYZW(int index, T x, T y, T z, T w) {
        index *= itemSize;

        arrayVec[index + 0] = x;
        arrayVec[index + 1] = y;
        arrayVec[index + 2] = z;
        arrayVec[index + 3] = w;

        return *this;
    }

//	 	BufferAttribute& onUpload(uploadCallback callback) {
//	 		onUploadCallback = callback;
//
//	 		return *this;
//	 	}
//
//	 	BufferAttribute clone() {
//	 		return BufferAttribute(arrayVec,itemSize);
//	 	}

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
typedef BufferAttribute<unsigned char> Uint8BufferAttribute;

typedef BufferAttribute<int16_t> Int16BufferAttribute;
typedef BufferAttribute<unsigned int> Uint16BufferAttribute;

typedef BufferAttribute<int32_t> Int32BufferAttribute;
typedef BufferAttribute<unsigned long> Uint32BufferAttribute;

typedef BufferAttribute<float> Float16BufferAttribute;
typedef BufferAttribute<float> Float32BufferAttribute;
typedef BufferAttribute<double> Float64BufferAttribute;

// class Uint8ClampedBufferAttribute extends BufferAttribute {

// 	constructor( array, itemSize, normalized ) {

// 		super( new Uint8ClampedArray( array ), itemSize, normalized );

// 	}

// }

#endif /* SRC_CORE_BUFFER_ATTRIBUTE_H_ */
