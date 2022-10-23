#include "buffer_attribute.h"

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

#include "matrix4.h"
#include "matrix3.h"

Vector3 _buffer_attribute_vector{};
Vector2 _buffer_attribute_vector2{};

//模版类成员函数的定义实现方式
template<typename T>
BufferAttribute<T>& BufferAttribute<T>::copyVector2sArray(std::vector<Vector2> vectors) {
     std::vector<T> array = arrayVec;
     int offset = 0;

     for (int i = 0, l = vectors.size(); i < l; i ++ ) {
         Vector2 vector = vectors[ i ];

         array[ offset ++ ] = vector.x;
         array[ offset ++ ] = vector.y;
     }

     return *this;
}

template<typename T>
BufferAttribute<T>& BufferAttribute<T>::copyVector3sArray(vector<Vector3> vectors) {
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

template<typename T>
BufferAttribute<T>& BufferAttribute<T>::copyVector4sArray(vector<Vector4d> vectors) {
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

template<typename T>
BufferAttribute<T>& BufferAttribute<T>::applyMatrix3(Matrix3& m) {
 	if ( itemSize == 2 ) {
 		for ( int i = 0, l = this->count; i < l; i ++ ) {
 			_buffer_attribute_vector2.fromBufferAttribute(*this, i );
 			_buffer_attribute_vector2.applyMatrix3(m );

 			setXY(i, _buffer_attribute_vector2.x, _buffer_attribute_vector2.y );
 		}

 	} else if ( itemSize == 3 ) {

 		for ( int i = 0, l = this->count; i < l; i ++ ) {
 			_buffer_attribute_vector.fromBufferAttribute(*this, i );
 			_buffer_attribute_vector.applyMatrix3(m );

 			setXYZ(i, _buffer_attribute_vector.x, _buffer_attribute_vector.y, _buffer_attribute_vector.z );
 		}

 	}

 	return *this;
}

template<typename T>
BufferAttribute<T>& BufferAttribute<T>::applyMatrix4(Matrix4 &m) {
    for (int i = 0, l = this->count; i < l; i++) {

        _buffer_attribute_vector.fromBufferAttribute(this, i);
        _buffer_attribute_vector.applyMatrix4(m);

        setXYZ(i, _buffer_attribute_vector.x, _buffer_attribute_vector.y, _buffer_attribute_vector.z);
    }

    return *this;
}

template<typename T>
BufferAttribute<T>& BufferAttribute<T>::applyNormalMatrix(Matrix4 &m) {

    for (int i = 0, l = this->count; i < l; i++) {
        _buffer_attribute_vector.fromBufferAttribute(*this, i);
        _buffer_attribute_vector.applyMatrix4(m).normalize();

        setXYZ(i, _buffer_attribute_vector.x, _buffer_attribute_vector.y, _buffer_attribute_vector.z);
    }

    return *this;
}

template<typename T>
BufferAttribute<T>& BufferAttribute<T>::transformDirection(Matrix4 &m) {

    for (int i = 0, l = this->count; i < l; i++) {
        _buffer_attribute_vector.fromBufferAttribute(this, i);
        _buffer_attribute_vector.transformDirection(m);

        setXYZ(i, _buffer_attribute_vector.x, _buffer_attribute_vector.y, _buffer_attribute_vector.z);
    }

    return *this;
}