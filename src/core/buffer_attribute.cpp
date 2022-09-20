#include "buffer_attribute.h"

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

Vector3 _vector;
Vector2 _vector2;

// BufferAttribute& BufferAttribute::copyVector2sArray(vector<Vector2> vectors) {
//             const vector<T> array = arrayVec;
//             int offset = 0;

//             for (int i = 0, l = vectors.size(); i < l; i ++ ) {
//                 Vector2 vector = vectors[ i ];

//                 array[ offset ++ ] = vector.x;
//                 array[ offset ++ ] = vector.y;
//             }

//             return *this;
//         }

// BufferAttribute& BufferAttribute::copyVector3sArray(vector<Vector3> vectors) {
// 	const vector<T> array = arrayVec;
// 	int offset = 0;

// 	for ( int i = 0, l = vectors.size(); i < l; i ++ ) {

// 		Vector3 vector = vectors[ i ];

// 		array[ offset ++ ] = vector.x;
// 		array[ offset ++ ] = vector.y;
// 		array[ offset ++ ] = vector.z;
// 	}

// 	return *this;
// }

// BufferAttribute& BufferAttribute::copyVector4sArray(vector<Vector4d> vectors) {
// 	const vector<T> array = arrayVec;
// 	int offset = 0;

// 	for ( int i = 0, l = vectors.size(); i < l; i ++ ) {

// 		Vector4d vector = vectors[ i ];

// 		array[ offset ++ ] = vector.x;
// 		array[ offset ++ ] = vector.y;
// 		array[ offset ++ ] = vector.z;
// 		array[ offset ++ ] = vector.w;
// 	}

// 	return *this;
// }


// BufferAttribute& BufferAttribute::applyMatrix3(Matrix3& m) {
// 	if ( itemSize == 2 ) {

// 		for ( int i = 0, l = this->count; i < l; i ++ ) {
// 			_vector2.fromBufferAttribute( *this, i );
// 			_vector2.applyMatrix3( m );

// 			setXY( i, _vector2.x, _vector2.y );
// 		}

// 	} else if ( itemSize == 3 ) {

// 		for ( int i = 0, l = this->count; i < l; i ++ ) {
// 			_vector.fromBufferAttribute( *this, i );
// 			_vector.applyMatrix3( m );

// 			setXYZ( i, _vector.x, _vector.y, _vector.z );
// 		}

// 	}

// 	return *this;
// }


// BufferAttribute& BufferAttribute::applyMatrix4(Matrix4& m) {
// 	for ( int i = 0, l = this->count; i < l; i ++ ) {

// 		_vector.fromBufferAttribute( this, i );
// 		_vector.applyMatrix4( m );

// 		setXYZ( i, _vector.x, _vector.y, _vector.z );
// 	}

// 	return *this;
// }

// BufferAttribute& BufferAttribute::applyNormalMatrix(Matrix4& m) {

// 	for ( int i = 0, l = this->count; i < l; i ++ ) {
// 		_vector.fromBufferAttribute( *this, i );
// 		_vector.applyMatrix4( m ).normalize();

// 		setXYZ( i, _vector.x, _vector.y, _vector.z );
// 	}

// 	return *this;
// }

// BufferAttribute& BufferAttribute::transformDirection(Matrix4& m){

// 	for ( int i = 0, l = this->count; i < l; i ++ ) {
// 		_vector.fromBufferAttribute( this, i );
// 		_vector.transformDirection( m );

// 		setXYZ( i, _vector.x, _vector.y, _vector.z );
// 	}

// 	return *this;
// }