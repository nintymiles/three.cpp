#ifndef VECTOR4_H
#define VECTOR4_H

#include<exception>
#include "quaternion.h"
#include "matrix4.h"

template <typename T> 
class Vector4 {
    public:
        //public data member
        T x,y,z,w;

		Vector4()=default;
        Vector4(T x,T y,T z,T w):x(x),y(y),z(z),w(w){};
        //only constructors take base initializers
        Vector4& set(T x,T y,T z,T w){
            this->x = x;
            this->y = y;
            this->z = z;
			this->w = w;

            return *this;
        }
		
        Vector4& setScalar(T scalar){
            this->x = scalar;
            this->y = scalar;
            this->z = scalar;

            return *this;
        }
        Vector4& setX(T x){
            this->x = x;
            return *this;
        }
        Vector4& setY(T y){
            this->y = y;
            return *this;
        }
        Vector4& setZ(T z){
            this->z = z;
            return *this;
        }
        Vector4& setComponent(int index, T value){
            switch (index)
            {
            case 0:this->x = value;break;
            case 1:this->y = value;break;
            case 2:this->z = value; break;
            default:throw std::runtime_error("index is out of range: " + std::to_string(index));break;
            }
            return *this;
        }
	    T getComponent(int index) {
		    switch ( index ) {
			case 0: return this->x;
			case 1: return this->y;
			case 2: return this->z;
			default: throw std::runtime_error("index is out of range: " + std::to_string(index));break;
		}

	}
	Vector4 clone() {
		return new Vector4(this->x,this->y,this->z);
	}
    
    Vector4& copy(Vector4 v) {
		*this = v;
		return *this;
	}
    //操作符重载，不熟悉
    Vector4& operator+=(const Vector4 rhs){
        this += rhs;
        return *this;
    }
    Vector4& addVectors(Vector4 a,Vector4 b) {
		this->x = a.x + b.x;
		this->y = a.y + b.y;
		this->z = a.z + b.z;

		return *this;
	}

    Vector4& addScaledVector(Vector4 v,double s) {
		this->x += v.x * s;
		this->y += v.y * s;
		this->z += v.z * s;

		return *this;
	}

    Vector4& sub(Vector4 v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;

		return *this;
	}

	Vector4& subVectors(Vector4 a,Vector4 b) {
		this->x = a.x - b.x;
		this->y = a.y - b.y;
		this->z = a.z - b.z;

		return *this;
	}

    Vector4& multiply(Vector4 v) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;

		return *this;
	}

    Vector4& multiplyScalar(double scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;

		return *this;
	}

    Vector4& multiplyVectors(Vector4 a,Vector4 b) {
		this->x = a.x * b.x;
		this->y = a.y * b.y;
		this->z = a.z * b.z;

		return *this;
	}

    //模版的返回值，可以借助auto实现简化
	auto dot(Vector4 v) ->T{
		return this->x * v.x + this->y * v.y + this->z * v.z;
	}

    Vector4& applyQuaternion(Quaternion q) {

		const auto x = this->x, y = this->y, z = this->z;
		const double qx = q.x(), qy = q.y(), qz = q.z(), qw = q.w();

		// calculate quat * vector
		const auto ix = qw * x + qy * z - qz * y;
		const auto iy = qw * y + qz * x - qx * z;
		const auto iz = qw * z + qx * y - qy * x;
		const auto iw = - qx * x - qy * y - qz * z;

		// calculate result * inverse quat
		this->x = ix * qw + iw * - qx + iy * - qz - iz * - qy;
		this->y = iy * qw + iw * - qy + iz * - qx - ix * - qz;
		this->z = iz * qw + iw * - qz + ix * - qy - iy * - qx;

		return *this;
	}

	Vector4& setFromMatrixColumn(Matrix4 m, int index) {
		return fromArray(m.elements, index * 4);
	}

	Vector4& fromArray(T array[], int offset = 0 ) {
		x = array[ offset ];
		y = array[ offset + 1 ];
		z = array[ offset + 2 ];

		return *this;
	}





	// TODO lengthSquared?
	T lengthSq() {
		return x * x + y * y + z * z;
	}

	T length() {
		return sqrt(lengthSq());
	}

	T manhattanLength(){
		return abs( this->x ) + abs( this->y ) + abs( this->z );
	}

	T normalize() {
		return divideScalar(length()?length():1);
	}

	T divideScalar(T scalar) {
		return multiplyScalar(1 / scalar);

	}

// 	min( v ) {

// 		this->x = Math.min( this->x, v.x );
// 		this->y = Math.min( this->y, v.y );
// 		this->z = Math.min( this->z, v.z );

// 		return *this;

// 	}

// 	max( v ) {

// 		this->x = Math.max( this->x, v.x );
// 		this->y = Math.max( this->y, v.y );
// 		this->z = Math.max( this->z, v.z );

// 		return *this;

// 	}

	Vector4& crossVectors(Vector4 a,Vector4 b){

		const T ax = a.x, ay = a.y, az = a.z;
		const T bx = b.x, by = b.y, bz = b.z;

		this->x = ay * bz - az * by;
		this->y = az * bx - ax * bz;
		this->z = ax * by - ay * bx;

		return *this;
	}


    private:
        
};








// 	applyEuler( euler ) {

// 		if ( ! ( euler && euler.isEuler ) ) {

// 			console.error( 'THREE.Vector4: .applyEuler() now expects an Euler rotation rather than a Vector4 and order.' );

// 		}

// 		return *this-applyQuaternion( _quaternion.setFromEuler( euler ) );

// 	}

// 	applyAxisAngle( axis, angle ) {

// 		return *this-applyQuaternion( _quaternion.setFromAxisAngle( axis, angle ) );

// 	}

// 	applyMatrix3( m ) {

// 		const x = this->x, y = this->y, z = this->z;
// 		const e = m.elements;

// 		this->x = e[ 0 ] * x + e[ 3 ] * y + e[ 6 ] * z;
// 		this->y = e[ 1 ] * x + e[ 4 ] * y + e[ 7 ] * z;
// 		this->z = e[ 2 ] * x + e[ 5 ] * y + e[ 8 ] * z;

// 		return *this;

// 	}

// 	applyNormalMatrix( m ) {

// 		return *this-applyMatrix3( m ).normalize();

// 	}

// 	applyMatrix4( m ) {

// 		const x = this->x, y = this->y, z = this->z;
// 		const e = m.elements;

// 		const w = 1 / ( e[ 3 ] * x + e[ 7 ] * y + e[ 11 ] * z + e[ 15 ] );

// 		this->x = ( e[ 0 ] * x + e[ 4 ] * y + e[ 8 ] * z + e[ 12 ] ) * w;
// 		this->y = ( e[ 1 ] * x + e[ 5 ] * y + e[ 9 ] * z + e[ 13 ] ) * w;
// 		this->z = ( e[ 2 ] * x + e[ 6 ] * y + e[ 10 ] * z + e[ 14 ] ) * w;

// 		return *this;

// 	}

// 	applyQuaternion( q ) {

// 		const x = this->x, y = this->y, z = this->z;
// 		const qx = q.x, qy = q.y, qz = q.z, qw = q.w;

// 		// calculate quat * vector

// 		const ix = qw * x + qy * z - qz * y;
// 		const iy = qw * y + qz * x - qx * z;
// 		const iz = qw * z + qx * y - qy * x;
// 		const iw = - qx * x - qy * y - qz * z;

// 		// calculate result * inverse quat

// 		this->x = ix * qw + iw * - qx + iy * - qz - iz * - qy;
// 		this->y = iy * qw + iw * - qy + iz * - qx - ix * - qz;
// 		this->z = iz * qw + iw * - qz + ix * - qy - iy * - qx;

// 		return *this;

// 	}

// 	project( camera ) {

// 		return *this-applyMatrix4( camera.matrixWorldInverse ).applyMatrix4( camera.projectionMatrix );

// 	}

// 	unproject( camera ) {

// 		return *this-applyMatrix4( camera.projectionMatrixInverse ).applyMatrix4( camera.matrixWorld );

// 	}

// 	transformDirection( m ) {

// 		// input: THREE.Matrix4 affine matrix
// 		// vector interpreted as a direction

// 		const x = this->x, y = this->y, z = this->z;
// 		const e = m.elements;

// 		this->x = e[ 0 ] * x + e[ 4 ] * y + e[ 8 ] * z;
// 		this->y = e[ 1 ] * x + e[ 5 ] * y + e[ 9 ] * z;
// 		this->z = e[ 2 ] * x + e[ 6 ] * y + e[ 10 ] * z;

// 		return *this-normalize();

// 	}

// 	divide( v ) {

// 		this->x /= v.x;
// 		this->y /= v.y;
// 		this->z /= v.z;

// 		return *this;

// 	}

// 	divideScalar( scalar ) {

// 		return *this-multiplyScalar( 1 / scalar );

// 	}

// 	min( v ) {

// 		this->x = Math.min( this->x, v.x );
// 		this->y = Math.min( this->y, v.y );
// 		this->z = Math.min( this->z, v.z );

// 		return *this;

// 	}

// 	max( v ) {

// 		this->x = Math.max( this->x, v.x );
// 		this->y = Math.max( this->y, v.y );
// 		this->z = Math.max( this->z, v.z );

// 		return *this;

// 	}

// 	clamp( min, max ) {

// 		// assumes min < max, componentwise

// 		this->x = Math.max( min.x, Math.min( max.x, this->x ) );
// 		this->y = Math.max( min.y, Math.min( max.y, this->y ) );
// 		this->z = Math.max( min.z, Math.min( max.z, this->z ) );

// 		return *this;

// 	}

// 	clampScalar( minVal, maxVal ) {

// 		this->x = Math.max( minVal, Math.min( maxVal, this->x ) );
// 		this->y = Math.max( minVal, Math.min( maxVal, this->y ) );
// 		this->z = Math.max( minVal, Math.min( maxVal, this->z ) );

// 		return *this;

// 	}

// 	clampLength( min, max ) {
// 		//length()函数返回的是Vector的长度
// 		const length = this-length();

// 		// (length||1)用来保证除数不为1
// 		return *this-divideScalar( length || 1 ).multiplyScalar( Math.max( min, Math.min( max, length ) ) );

// 	}

// 	floor() {

// 		this->x = Math.floor( this->x );
// 		this->y = Math.floor( this->y );
// 		this->z = Math.floor( this->z );

// 		return *this;

// 	}

// 	ceil() {

// 		this->x = Math.ceil( this->x );
// 		this->y = Math.ceil( this->y );
// 		this->z = Math.ceil( this->z );

// 		return *this;

// 	}

// 	round() {

// 		this->x = Math.round( this->x );
// 		this->y = Math.round( this->y );
// 		this->z = Math.round( this->z );

// 		return *this;

// 	}

// 	roundToZero() {

// 		this->x = ( this->x < 0 ) ? Math.ceil( this->x ) : Math.floor( this->x );
// 		this->y = ( this->y < 0 ) ? Math.ceil( this->y ) : Math.floor( this->y );
// 		this->z = ( this->z < 0 ) ? Math.ceil( this->z ) : Math.floor( this->z );

// 		return *this;

// 	}

// 	negate() {

// 		this->x = - this->x;
// 		this->y = - this->y;
// 		this->z = - this->z;

// 		return *this;

// 	}



// 	// TODO lengthSquared?

// 	lengthSq() {

// 		return *this->x * this->x + this->y * this->y + this->z * this->z;

// 	}

// 	length() {

// 		return Math.sqrt( this->x * this->x + this->y * this->y + this->z * this->z );

// 	}

// 	manhattanLength() {

// 		return Math.abs( this->x ) + Math.abs( this->y ) + Math.abs( this->z );

// 	}

// 	normalize() {

// 		return *this-divideScalar( this-length() || 1 );

// 	}

// 	setLength( length ) {

// 		return *this-normalize().multiplyScalar( length );

// 	}

// 	lerp( v, alpha ) {

// 		this->x += ( v.x - this->x ) * alpha;
// 		this->y += ( v.y - this->y ) * alpha;
// 		this->z += ( v.z - this->z ) * alpha;

// 		return *this;

// 	}

// 	lerpVectors( v1, v2, alpha ) {

// 		this->x = v1.x + ( v2.x - v1.x ) * alpha;
// 		this->y = v1.y + ( v2.y - v1.y ) * alpha;
// 		this->z = v1.z + ( v2.z - v1.z ) * alpha;

// 		return *this;

// 	}

// 	cross( v, w ) {

// 		if ( w !== undefined ) {

// 			console.warn( 'THREE.Vector4: .cross() now only accepts one argument. Use .crossVectors( a, b ) instead.' );
// 			return *this-crossVectors( v, w );

// 		}

// 		return *this-crossVectors( this, v );

// 	}

// 	crossVectors( a, b ) {

// 		const ax = a.x, ay = a.y, az = a.z;
// 		const bx = b.x, by = b.y, bz = b.z;

// 		this->x = ay * bz - az * by;
// 		this->y = az * bx - ax * bz;
// 		this->z = ax * by - ay * bx;

// 		return *this;

// 	}

// 	projectOnVector( v ) {

// 		const denominator = v.lengthSq();

// 		if ( denominator === 0 ) return *this-set( 0, 0, 0 );

// 		const scalar = v.dot( this ) / denominator;

// 		return *this-copy( v ).multiplyScalar( scalar );

// 	}

// 	projectOnPlane( planeNormal ) {

// 		_buffer_attribute_vector.copy( this ).projectOnVector( planeNormal );

// 		return *this-sub( _buffer_attribute_vector );

// 	}

// 	reflect( normal ) {

// 		// reflect incident vector off plane orthogonal to normal
// 		// normal is assumed to have unit length

// 		return *this-sub( _buffer_attribute_vector.copy( normal ).multiplyScalar( 2 * this-dot( normal ) ) );

// 	}

// 	angleTo( v ) {

// 		const denominator = Math.sqrt( this-lengthSq() * v.lengthSq() );

// 		if ( denominator === 0 ) return Math.PI / 2;

// 		const theta = this-dot( v ) / denominator;

// 		// clamp, to handle numerical problems

// 		return Math.acos( MathUtils.clamp( theta, - 1, 1 ) );

// 	}

// 	distanceTo( v ) {

// 		return Math.sqrt( this-distanceToSquared( v ) );

// 	}

// 	distanceToSquared( v ) {

// 		const dx = this->x - v.x, dy = this->y - v.y, dz = this->z - v.z;

// 		return dx * dx + dy * dy + dz * dz;

// 	}

// 	manhattanDistanceTo( v ) {

// 		return Math.abs( this->x - v.x ) + Math.abs( this->y - v.y ) + Math.abs( this->z - v.z );

// 	}

// 	setFromSpherical( s ) {

// 		return *this-setFromSphericalCoords( s.radius, s.phi, s.theta );

// 	}

// 	setFromSphericalCoords( radius, phi, theta ) {

// 		const sinPhiRadius = Math.sin( phi ) * radius;

// 		this->x = sinPhiRadius * Math.sin( theta );
// 		this->y = Math.cos( phi ) * radius;
// 		this->z = sinPhiRadius * Math.cos( theta );

// 		return *this;

// 	}

// 	setFromCylindrical( c ) {

// 		return *this-setFromCylindricalCoords( c.radius, c.theta, c.y );

// 	}

// 	setFromCylindricalCoords( radius, theta, y ) {

// 		this->x = radius * Math.sin( theta );
// 		this->y = y;
// 		this->z = radius * Math.cos( theta );

// 		return *this;

// 	}

// 	setFromMatrixPosition( m ) {

// 		const e = m.elements;

// 		this->x = e[ 12 ];
// 		this->y = e[ 13 ];
// 		this->z = e[ 14 ];

// 		return *this;

// 	}

// 	setFromMatrixScale( m ) {

// 		const sx = this-setFromMatrixColumn( m, 0 ).length();
// 		const sy = this-setFromMatrixColumn( m, 1 ).length();
// 		const sz = this-setFromMatrixColumn( m, 2 ).length();

// 		this->x = sx;
// 		this->y = sy;
// 		this->z = sz;

// 		return *this;

// 	}

// 	setFromMatrixColumn( m, index ) {

// 		return *this-fromArray( m.elements, index * 4 );

// 	}

// 	setFromMatrix3Column( m, index ) {

// 		return *this-fromArray( m.elements, index * 3 );

// 	}

// 	setFromEuler( e ) {

// 		this->x = e._x;
// 		this->y = e._y;
// 		this->z = e._z;

// 		return *this;

// 	}

// 	equals( v ) {

// 		return ( ( v.x === this->x ) && ( v.y === this->y ) && ( v.z === this->z ) );

// 	}

// 	fromArray( array, offset = 0 ) {

// 		this->x = array[ offset ];
// 		this->y = array[ offset + 1 ];
// 		this->z = array[ offset + 2 ];

// 		return *this;

// 	}

// 	toArray( array = [], offset = 0 ) {

// 		array[ offset ] = this->x;
// 		array[ offset + 1 ] = this->y;
// 		array[ offset + 2 ] = this->z;

// 		return array;

// 	}

// 	fromBufferAttribute( attribute, index, offset ) {

// 		if ( offset !== undefined ) {

// 			console.warn( 'THREE.Vector4: offset has been removed from .fromBufferAttribute().' );

// 		}

// 		this->x = attribute.getX( index );
// 		this->y = attribute.getY( index );
// 		this->z = attribute.getZ( index );

// 		return *this;

// 	}

// 	random() {

// 		this->x = Math.random();
// 		this->y = Math.random();
// 		this->z = Math.random();

// 		return *this;

// 	}

// 	randomDirection() {

// 		// Derived from https://mathworld.wolfram.com/SpherePointPicking.html

// 		const u = ( Math.random() - 0.5 ) * 2;
// 		const t = Math.random() * Math.PI * 2;
// 		const f = Math.sqrt( 1 - u ** 2 );

// 		this->x = f * Math.cos( t );
// 		this->y = f * Math.sin( t );
// 		this->z = u;

// 		return *this;

// 	}

// 	*[ Symbol.iterator ]() {

// 		yield this->x;
// 		yield this->y;
// 		yield this->z;

// 	}

// }

// Vector4.prototype.isVector4 = true;

// const _buffer_attribute_vector = /*@__PURE__*/ new Vector4();
// const _quaternion = /*@__PURE__*/ new Quaternion();

// export { Vector4 };


//设置常用子类型的alias，以方便使用
// element of type double precision float
typedef Vector4<double> Vector4d;
typedef Vector4<float> Vector4f;
typedef Vector4<unsigned char> Vector4uc;

#endif //Vector4_h