#ifndef SRC_CORE_BUFFER_ATTRIBUTE_H_
#define SRC_CORE_BUFFER_ATTRIBUTE_H_

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "constants.h"
#include "common_types.h"
#include "color.h"
#include "math_utils.h"
#include "sole.h"
#include "simplesignal.h"

#include <algorithm>
#include <string>
#include <vector>
#include <functional>

using namespace math;
/*namespace math {
    class Vector2;
    class Vector3;
    class Vector4;
    class Matrix3;
    class Matrix4;
}*/

class Vector2;

template<typename Type>
class BufferAttribute {
private:
    bool _needsUpdate;

public:
    using sptr = std::shared_ptr<BufferAttribute>;

    BufferAttribute(){
        this->version = 0;

        this->count = 0;

        this->itemSize = 0;

        this->normalized = false;

        this->size = 0;

        this->usage = Usage::StaticDrawUsage;

        this->_needsUpdate = false;

        //this->array = nullptr;
    }

    BufferAttribute(Type* array1, unsigned size, unsigned _itemSize, bool normalized = false){

        this->version = 0;

        this->size = size;

        this->itemSize = _itemSize;

        this->normalized = normalized;

        this->usage = Usage::StaticDrawUsage;

        this->_needsUpdate = false;

        this->count = this->size / this->itemSize;

        this->array.resize(size);

        if(array1!=nullptr)
            memcpy(&this->array[0], array1, sizeof(Type) * size);
    }

    BufferAttribute(const std::vector<Type>& array1, unsigned _itemSize, bool normalized = false) {
        this->version = 0;

        this->size = array1.size();

        this->itemSize = _itemSize;

        this->normalized = normalized;

        this->usage = Usage::StaticDrawUsage;

        this->_needsUpdate = false;

        this->count = this->size / this->itemSize;

        this->array.resize(size);

        this->updateRange = {0,-1};

        std::copy(array1.begin(), array1.end(), this->array.begin());
    }

    BufferAttribute(const BufferAttribute& source) {
        this->name = source.name;

        if (source.array.size() > 0) {
            if (array.size() != source.array.size())
                array.resize(source.array.size());
            std::copy(source.array.begin(), source.array.end(), this->array.begin());
        }
        else {
            array.clear();
        }

        this->itemSize = source.itemSize;

        this->size = source.size;

        this->count = source.count;

        this->normalized = source.normalized;

        this->usage = source.usage;
    }
    virtual ~BufferAttribute() = default;

    static sptr create() {
        return sptr(new BufferAttribute());
    }
    static sptr create(unsigned size, unsigned itemSize, bool normalized = false) {
        return sptr(new BufferAttribute(nullptr, size, itemSize, normalized));
    }

    static sptr create(Type* array, unsigned size, unsigned itemSize, bool normalized = false) {
        return sptr(new BufferAttribute(array, size, itemSize, normalized));
    }

    static sptr create(const std::vector<Type>&	array, unsigned itemSize, bool normalized = false) {
        return sptr(new BufferAttribute(array, itemSize, normalized));
    }
    std::string name;

    std::vector<Type> array;

    unsigned itemSize;

    sole::uuid uuid = sole::uuid1();

    unsigned size;

    Usage usage;

    threecpp::UpdateRange updateRange{};

    unsigned version;

    bool normalized;

    unsigned count;

    void setNeedsUpdate(bool value) {
        version++;
        _needsUpdate = value;
    }

    bool needsUpdate() {
        return _needsUpdate;
    }

    threecpp::Signal<void(const BufferAttribute&)> onUpload;

    BufferAttribute& setUsage(Usage usage)
    {
        this->usage = usage;
        return *this;
    }

    BufferAttribute& copy(const BufferAttribute& source);

    BufferAttribute* clone();

    BufferAttribute& copyAt(unsigned index1, BufferAttribute& attribute, unsigned  index2);

    BufferAttribute& copyArray(Type* array, unsigned length);

    BufferAttribute& copyArrayAt(Type *array, unsigned stride,unsigned size);

    BufferAttribute& copyVector2sArray(const std::vector<Vector2>& vectors);

    BufferAttribute& copyVector3sArray(const std::vector<Vector3>& vectors);

    BufferAttribute& copyColor3sArray(const std::vector<Color>& colors);

    BufferAttribute& copyVector4sArray(const std::vector<Vector4> vectors);

    BufferAttribute& applyMatrix3(const Matrix3& m);

    BufferAttribute& applyMatrix4(const Matrix4& m);

    BufferAttribute& applyNormalMatrix(const Matrix3& m);

    BufferAttribute& transformDirection(const Matrix4& m);

    BufferAttribute& set(Type* value, unsigned length, unsigned offset = 0);

    Type getX(unsigned index) const;

    BufferAttribute& setX(unsigned index, Type x);

    Type getY(unsigned index) const;

    BufferAttribute& setY(unsigned index, Type y);

    Type getZ(unsigned index) const;

    BufferAttribute& setZ(unsigned index, Type z);

    Type getW(unsigned index) const;

    BufferAttribute& setW(unsigned index, Type w);


    BufferAttribute& setXY(unsigned index, Type x, Type y);

    BufferAttribute& setXYZ(unsigned index, Type x, Type y, Type z);

    BufferAttribute& setXYZW(unsigned index, Type x, Type y, Type z, Type w);

    void convertBufferAttributeToNonIndexed(const BufferAttribute& attribute, const std::vector<unsigned>& indices);

};

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::copy(const BufferAttribute& source)
{
    this->name = source.name;

    if (source.array.size() > 0) {
        if (array.size() != source.array.size())
            array.resize(source.array.size());
        std::copy(source.array.begin(), source.array.end(), this->array.begin());
    }
    else {
        array.clear();
    }

    this->itemSize = source.itemSize;

    this->size = source.size;

    this->count = source.count;

    this->normalized = source.normalized;

    this->usage = source.usage;

    return *this;

}

template<typename Type>
inline BufferAttribute<Type>* BufferAttribute<Type>::clone()
{
    return new BufferAttribute<Type>(*this);
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::copyAt(unsigned index1, BufferAttribute& attribute, unsigned index2)
{
    index1 *= this->itemSize;
    index2 *= attribute.itemSize;

    for (unsigned i = 0; i < this->itemSize; i++) {

        this->array[index1 + i] = attribute.array[index2 + i];

    }

    return *this;
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::copyArray(Type* array, unsigned length)
{
    //std::memcpy(&this->array[0], array, sizeof(Type) * length);

    for (unsigned i = 0; i<length; i++) {
        printf("%d: %f\n", i, array[i]);
        this->array.push_back(array[i]);
    }

    return *this;
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::copyArrayAt(Type *array, unsigned stride,unsigned size) {
    for (unsigned i = 0; array[i] != NULL; i = i + stride) {
        printf("%d: %s\n", i, array[i]);
        for(unsigned j = 0; j < size; j++){
            this->array.push_back(array[i+j]);
        }

    }
    return *this;
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::copyVector2sArray(const std::vector<Vector2>& vectors)
{
    //Type* array = this->array;
    /*if (this->array.size() == 0 || this->array.size() != length)
        this->array.resize(length);*/
    unsigned offset = 0;

    for (unsigned i = 0; i < vectors.size(); i++) {

        auto vector = vectors[i];


        array[offset++] = vector.x;
        array[offset++] = vector.y;

    }
    return *this;
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::copyVector3sArray(const std::vector<Vector3>& vectors)
{
    //Type* array = this->array;
    //if (this->array.size() == 0 || this->array.size() != length)
    //	this->array.resize(length);
    unsigned offset = 0;

    for (unsigned i = 0; i < vectors.size(); i++) {
        auto vector = vectors[i];

        array[offset++] = vector.x;
        array[offset++] = vector.y;
        array[offset++] = vector.z;

    }
    return *this;
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::copyColor3sArray(const std::vector<Color>& colors)
{
    //Type* array = this->array;
    /*if (this->array.size() == 0 || this->array.size() != length)
        this->array.resize(length);*/
    unsigned offset = 0;

    for (unsigned i = 0; i < colors.size(); i++) {
        auto color = colors[i];

        array[offset++] = color.r;
        array[offset++] = color.g;
        array[offset++] = color.b;

    }
    return *this;
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::copyVector4sArray(std::vector<Vector4> vectors)
{
    //Type* array = this->array;
    /*if (this->array.size() == 0 || this->array.size() != length)
        this->array.resize(length);*/
    unsigned offset = 0;

    for (unsigned i = 0; i < vectors.size(); i++) {

        auto vector = vectors[i];


        array[offset++] = vector.x;
        array[offset++] = vector.y;
        array[offset++] = vector.z;
        array[offset++] = vector.w;

    }
    return *this;
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::applyMatrix3(const Matrix3& m)
{
    Vector3 _vector;
    for (int i = 0; i < this->count; i++) {

        _vector.x = this->getX(i);
        _vector.y = this->getY(i);
        _vector.z = this->getZ(i);

        _vector.applyMatrix3(const_cast<Matrix3&>(m));

        this->setXYZ(i, _vector.x, _vector.y, _vector.z);

    }

    return *this;
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::applyMatrix4(const Matrix4& m)
{
    Vector3 _vector;
    for (int i = 0; i < this->count; i++) {

        _vector.x = this->getX(i);
        _vector.y = this->getY(i);
        _vector.z = this->getZ(i);

        _vector.applyMatrix4(const_cast<Matrix4&>(m));

        this->setXYZ(i, _vector.x, _vector.y, _vector.z);

    }

    return *this;
}

template<typename type>
inline BufferAttribute<type>& BufferAttribute<type>::applyNormalMatrix(const Matrix3& m)
{
    Vector3 _vector;
    for (int i = 0; i < this->count; i++) {

        _vector.x = this->getX(i);
        _vector.y = this->getY(i);
        _vector.z = this->getZ(i);

        _vector.applyNormalMatrix(const_cast<Matrix3&>(m));

        this->setXYZ(i, _vector.x, _vector.y, _vector.z);

    }

    return *this;
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::transformDirection(const Matrix4& m)
{
    Vector3 _vector;
    for (int i = 0; i < this->count; i++) {

        _vector.x = this->getX(i);
        _vector.y = this->getY(i);
        _vector.z = this->getZ(i);

        _vector.transformDirection(const_cast<Matrix4&>(m));

        this->setXYZ(i, _vector.x, _vector.y, _vector.z);

    }

    return *this;
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::set(Type* value, unsigned length, unsigned offset)
{
    /*if (this->array.size() == 0 || this->array.size() != length)
        this->array.resize(length);*/

    std::memcpy(&this->array[0]+offset, value, sizeof(Type) * length);



    return *this;
}

template<typename Type>
inline Type BufferAttribute<Type>::getX(unsigned index) const
{
    return this->array[index * this->itemSize];
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::setX(unsigned index, Type x)
{
    this->array[index * this->itemSize] = x;
    return *this;
}

template<typename Type>
inline Type BufferAttribute<Type>::getY(unsigned index) const
{
    return this->array[index * this->itemSize + 1];
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::setY(unsigned index, Type y)
{
    this->array[index * this->itemSize + 1] = y;
    return *this;
}

template<typename Type>
inline Type BufferAttribute<Type>::getZ(unsigned index) const
{
    return this->array[index * this->itemSize + 2];
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::setZ(unsigned index, Type z)
{
    this->array[index * this->itemSize + 2] = z;
    return *this;
}

template<typename Type>
inline Type BufferAttribute<Type>::getW(unsigned index) const
{
    return this->array[index * this->itemSize + 3];
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::setW(unsigned index, Type w)
{
    this->array[index * this->itemSize + 3] = w;
    return *this;
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::setXY(unsigned index, Type x, Type y)
{
    index *= this->itemSize;

    this->array[index + 0] = x;
    this->array[index + 1] = y;

    return *this;
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::setXYZ(unsigned index, Type x, Type y, Type z)
{
    index *= this->itemSize;

    this->array[index + 0] = x;
    this->array[index + 1] = y;
    this->array[index + 2] = z;

    return *this;
}

template<typename Type>
inline BufferAttribute<Type>& BufferAttribute<Type>::setXYZW(unsigned index, Type x, Type y, Type z, Type w)
{
    index *= this->itemSize;

    this->array[index + 0] = x;
    this->array[index + 1] = y;
    this->array[index + 2] = z;
    this->array[index + 3] = w;

    return *this;
}

template<typename Type>
inline void BufferAttribute<Type>::convertBufferAttributeToNonIndexed(const BufferAttribute<Type>& attribute, const std::vector<unsigned>& indices)
{
    auto array = attribute.array;
    auto itemSize = attribute.itemSize;

    std::vector<float> array2(indices.size() * itemSize, 0.0f);
    //float[] array2 = new float[indices.Length * itemSize];

    int index = 0;
    int index2 = 0;

    for (unsigned int i = 0; i < indices.size(); i++)
    {
        index = indices[i] * itemSize;
        for (unsigned int j = 0; j < itemSize; j++)
        {
            array2[index2++] = array[index++];
        }
    }
    this->size = array2.size();
    this->itemSize = attribute.itemSize;
    this->count = this->size / this->itemSize;

    this->array.clear();
    for (unsigned i = 0; i < array2.size(); i++)
        this->array.push_back(array2[i]);

    //std::copy(array2.begin(), array2.end(), this->array.begin());
}

//class Vector3;
//class Vector2;
//template <typename T> class Vector4;
//class Matrix3;
//class Matrix4;
//
//typedef std::function<void()> uploadCallback;
//
//using std::vector;
//
//template <typename T>
//class BufferAttribute {
//    private:
//        std::string name{""};
//        int itemSize{0};
//        int _count{0};
//        bool normalized{false};
//        int usage{0};
//        //std::ranges::range<int> updateRange;
//        threecpp::Range updateRange{0,0};
//        int version{0};
//
//		uploadCallback onUploadCallback;
//
//
//    public:
//        using sptr = std::shared_ptr<BufferAttribute>;
//        vector<T> arrayVec;
//
//        BufferAttribute() = default;
//
//        BufferAttribute(vector<T> arrVec, int itemSize, bool normalized = true)
//            : arrayVec(arrVec), itemSize(itemSize), normalized(normalized), name(""), usage(StaticDrawUsage), version(0) {
//            _count = arrVec.size() / itemSize;
//            //updateRange = std::ranges:range<int>(0,0);
//        }
//
//        const int count() const {
//            return _count;
//        }
//
//	// constructor( array, itemSize, normalized ) {
//
//	// 	if ( Array.isArray( array ) ) {
//
//	// 		throw new TypeError( 'THREE.BufferAttribute: array should be a Typed Array.' );
//
//	// 	}
//
//	// 	this.name = '';
//
//	// 	this.array = array;
//	// 	this.itemSize = itemSize;
//	// 	this.count = array !== undefined ? array.length / itemSize : 0;
//	// 	this.normalized = normalized === true;
//
//	// 	this.usage = StaticDrawUsage;
//	// 	this.updateRange = { offset: 0, count: - 1 };
//
//	// 	this.version = 0;
//
//	// }
//
//    BufferAttribute(const BufferAttribute& ba):name(ba.name),arrayVec(ba.arrayVec),itemSize(ba.itemSize),
//                                                _count(ba.count()),normalized(ba.normalized),
//                                                updateRange(ba.updateRange),usage(ba.usage){}
//
//    BufferAttribute clone(){
//        return *this;
//    }
//
//    BufferAttribute& needsUpdate(bool updateFlag) {
//        if (updateFlag)
//            version++;
//
//        return *this;
//    }
//
//    BufferAttribute& setUsage(int usageValue) {
//        usage = usageValue;
//
//        return *this;
//    }
//
//    BufferAttribute& copy(const BufferAttribute& source) {
//        name = source.name;
//        arrayVec = source.arrayVec;
//        itemSize = source.itemSize;
//        count = source.count;
//        normalized = source.normalized;
//        updateRange = source.updateRange;
//
//        usage = source.usage;
//
//        return *this;
//    }
//
//    BufferAttribute &
//    copyAt(int &index1, BufferAttribute &attribute, int &index2) {
//        index1 *= itemSize;
//        index2 *= attribute.itemSize;
//
//        for (int i = 0, l = itemSize; i < l; i++) {
//            arrayVec[index1 + i] = attribute.arrayVec[index2 + i];
//        }
//
//        return *this;
//    }
//
//    BufferAttribute &
//    copyArray(vector<T> arrVec) {
//        arrayVec.copy(arrVec);
//
//        return *this;
//    }
//
//    BufferAttribute &
//    copyColorsArray(std::vector<Color> colors) {
//        std::vector<T> array = arrayVec;
//        int offset = 0;
//
//        for (Color color : colors) {
//
//            array[offset++] = color.r;
//            array[offset++] = color.g;
//            array[offset++] = color.b;
//        }
//
//        return *this;
//
//    }
//
//    BufferAttribute &
//    copyVector2sArray(std::vector<Vector2> vectors);
//
//    BufferAttribute &
//    copyVector3sArray(std::vector<Vector3> vectors);
//
//    BufferAttribute &
//    copyVector4sArray(std::vector<Vector4<double>> vectors);
//
//    BufferAttribute &
//    applyMatrix3(Matrix3 &m);
//
//    BufferAttribute &
//    applyMatrix4(Matrix4 &m);
//
//    BufferAttribute &
//    applyNormalMatrix(Matrix4 &m);
//
//    BufferAttribute &
//    transformDirection(Matrix4 &m);
//
//    BufferAttribute &
//    set(vector<T> value, int offset = 0) {
//        auto vBegin = value.begin();
//        auto vEnd = value.end();
//        vBegin += offset;
//
//        copy(vBegin, vEnd, std::back_inserter(arrayVec));
//
//        return *this;
//    }
//
//    T
//    getX(int index) {
//        return arrayVec[index * itemSize];
//    }
//
//    BufferAttribute &
//    setX(int index, T x) {
//        arrayVec[index * itemSize] = x;
//        return *this;
//    }
//
//    T
//    getY(int index) {
//        return arrayVec[index * itemSize + 1];
//    }
//
//    BufferAttribute &
//    setY(int index, T y) {
//        arrayVec[index * itemSize + 1] = y;
//        return *this;
//    }
//
//    T
//    getZ(int index) {
//        return arrayVec[index * itemSize + 2];
//    }
//
//    BufferAttribute &
//    setZ(int index, T z) {
//        arrayVec[index * itemSize + 2] = z;
//        return *this;
//    }
//
//    T
//    getW(int index) {
//        return arrayVec[index * itemSize + 3];
//    }
//
//    BufferAttribute &
//    setW(int index, T w) {
//        arrayVec[index * itemSize + 3] = w;
//        return *this;
//    }
//
//    BufferAttribute &
//    setXY(int index, T x, T y) {
//        index *= itemSize;
//
//        arrayVec[index + 0] = x;
//        arrayVec[index + 1] = y;
//
//        return *this;
//    }
//
//    BufferAttribute &
//    setXYZ(int index, T x, T y, T z) {
//        index *= itemSize;
//
//        arrayVec[index + 0] = x;
//        arrayVec[index + 1] = y;
//        arrayVec[index + 2] = z;
//
//        return *this;
//    }
//
//    BufferAttribute &
//    setXYZW(int index, T x, T y, T z, T w) {
//        index *= itemSize;
//
//        arrayVec[index + 0] = x;
//        arrayVec[index + 1] = y;
//        arrayVec[index + 2] = z;
//        arrayVec[index + 3] = w;
//
//        return *this;
//    }
//
//    BufferAttribute& onUpload(uploadCallback callback) {
//        onUploadCallback = callback;
//
//        return *this;
//    }
//
//
//
//		// toJSON() {
//
//		// 	const data = {
//		// 		itemSize: this.itemSize,
//		// 		type: this.array.constructor.name,
//		// 		array: Array.prototype.slice.call( this.array ),
//		// 		normalized: this.normalized
//		// 	};
//
//		// 	if ( this.name !== '' ) data.name = this.name;
//		// 	if ( this.usage !== StaticDrawUsage ) data.usage = this.usage;
//		// 	if ( this.updateRange.offset !== 0 || this.updateRange.count !== - 1 ) data.updateRange = this.updateRange;
//
//		// 	return data;
//		// }
//
//};
//
//typedef BufferAttribute<int8_t> Int8BufferAttribute;
//typedef BufferAttribute<unsigned char> Uint8BufferAttribute;
//
//typedef BufferAttribute<int16_t> Int16BufferAttribute;
//typedef BufferAttribute<unsigned int> Uint16BufferAttribute;
//
//typedef BufferAttribute<int32_t> Int32BufferAttribute;
//typedef BufferAttribute<unsigned long> Uint32BufferAttribute;
//
//typedef BufferAttribute<float> Float16BufferAttribute;
//typedef BufferAttribute<float> Float32BufferAttribute;
//typedef BufferAttribute<double> Float64BufferAttribute;
//
//// class Uint8ClampedBufferAttribute extends BufferAttribute {
//
//// 	constructor( array, itemSize, normalized ) {
//
//// 		super( new Uint8ClampedArray( array ), itemSize, normalized );
//
//// 	}
//
//// }

#endif /* SRC_CORE_BUFFER_ATTRIBUTE_H_ */
