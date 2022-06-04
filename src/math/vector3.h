#ifndef vector3_h
#define vector3_h

#include<exception>
#include<memory>
#include "quaternion.h"
#include "matrix4.h"

template <typename T> 
class Vector3 {
    public:
        //public data member
        T x,y,z;

		Vector3();
        Vector3(T x,T y,T z):x(x),y(y),z(z){};
        //only constructors take base initializers
        Vector3& set(T x,T y,T z){
            this->x = x;
            this->y = y;
            this->z = z;

            return *this;
        }
        Vector3& setScalar(T scalar){
            this->x = scalar;
            this->y = scalar;
            this->z = scalar;

            return *this;
        }
        Vector3& setX(T x){
            this->x = x;
            return *this;
        }
        Vector3& setY(T y){
            this->y = y;
            return *this;
        }
        Vector3& setZ(T z){
            this->z = z;
            return *this;
        }
        Vector3& setComponent(int index, T value){
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
	Vector3 clone() {
		return new Vector3(this->x,this->y,this->z);
	}
    
    Vector3& copy(Vector3 v) {
		*this = v;
		return *this;
	}
    //操作符重载，不熟悉
    Vector3& operator+=(const Vector3 rhs){
        this += rhs;
        return *this;
    }
    Vector3& addVectors(Vector3 a,Vector3 b) {
		this->x = a.x + b.x;
		this->y = a.y + b.y;
		this->z = a.z + b.z;

		return *this;
	}

    Vector3& addScaledVector(Vector3 v,double s) {
		this->x += v.x * s;
		this->y += v.y * s;
		this->z += v.z * s;

		return *this;
	}

    Vector3& sub(Vector3 v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;

		return *this;
	}

	Vector3& subVectors(Vector3 a,Vector3 b) {
		this->x = a.x - b.x;
		this->y = a.y - b.y;
		this->z = a.z - b.z;

		return *this;
	}

    Vector3& multiply(Vector3 v) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;

		return *this;
	}

    Vector3& multiplyScalar(double scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;

		return *this;
	}

    Vector3& multiplyVectors(Vector3 a,Vector3 b) {
		this->x = a.x * b.x;
		this->y = a.y * b.y;
		this->z = a.z * b.z;

		return *this;
	}

    //模版的返回值，可以借助auto实现简化
	auto dot(Vector3 v) ->T{
		return this->x * v.x + this->y * v.y + this->z * v.z;
	}

    Vector3& applyQuaternion(Quaternion q) {

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

	Vector3& setFromMatrixColumn(Matrix4 m, int index) {
		return fromArray(m.elements, index * 4);
	}

// 	applyEuler( euler ) {

// 		if ( ! ( euler && euler.isEuler ) ) {

// 			console.error( 'THREE.Vector3: .applyEuler() now expects an Euler rotation rather than a Vector3 and order.' );

// 		}

// 		return *this-applyQuaternion( _quaternion.setFromEuler( euler ) );

// 	}

	Vector3& applyAxisAngle(Vector3 axis, double angle) {
		applyQuaternion( _quaternion->setFromAxisAngle(axis,angle));

		return *this;
	}

// 	applyMatrix3( m ) {

// 		const x = this->x, y = this->y, z = this->z;
// 		const e = m.elements;

// 		this->x = e[ 0 ] * x + e[ 3 ] * y + e[ 6 ] * z;
// 		this->y = e[ 1 ] * x + e[ 4 ] * y + e[ 7 ] * z;
// 		this->z = e[ 2 ] * x + e[ 5 ] * y + e[ 8 ] * z;

// 		return *this;

// 	}

	// applyNormalMatrix( m ) {

	// 	return *this-applyMatrix3( m ).normalize();

	// }

	Vector3 applyMatrix4(Matrix4 m) {
		const T x = this->x, y = this->y, z = this->z;

		const double w = 1 / (m.elements[ 3 ] * x + m.elements[ 7 ] * y + m.elements[ 11 ] * z + m.elements[ 15 ]);

		this->x = ( m.elements[ 0 ] * x + m.elements[ 4 ] * y + m.elements[ 8 ] * z + m.elements[ 12 ] ) * w;
		this->y = ( m.elements[ 1 ] * x + m.elements[ 5 ] * y + m.elements[ 9 ] * z + m.elements[ 13 ] ) * w;
		this->z = ( m.elements[ 2 ] * x + m.elements[ 6 ] * y + m.elements[ 10 ] * z + m.elements[ 14 ] ) * w;

		return *this;
	}

	// Vector3& project(Matrix4 camera) {
	// 	applyMatrix4(camera.matrixWorldInverse ).applyMatrix4(camera.projectionMatrix);
		
	// 	return *this;
	// }

// 	unproject( camera ) {

// 		return *this-applyMatrix4( camera.projectionMatrixInverse ).applyMatrix4( camera.matrixWorld );

// 	}

	Vector3& transformDirection(Matrix4 m) {
		// input: THREE.Matrix4 affine matrix
		// vector interpreted as a direction

		const T x = this->x, y = this->y, z = this->z;

		this->x = m.elements[ 0 ] * x + m.elements[ 4 ] * y + m.elements[ 8 ] * z;
		this->y = m.elements[ 1 ] * x + m.elements[ 5 ] * y + m.elements[ 9 ] * z;
		this->z = m.elements[ 2 ] * x + m.elements[ 6 ] * y + m.elements[ 10 ] * z;

		return this->normalize();

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

	Vector3& normalize() {
		return divideScalar(length()?length():1);
	}

	Vector3& setLength(double length) {

		return *this-normalize().multiplyScalar( length );

	}

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

	Vector3& divide(Vector3 v) {

		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;

		return *this;
	}

	Vector3& divideScalar(T scalar) {
		return multiplyScalar(1 / scalar);

	}

	Vector3& min(Vector3& v) {

		this->x = fmin(this->x, v.x);
		this->y = fmin(this->y, v.y);
		this->z = fmin(this->z, v.z);

		return *this;
	}

	Vector3& max(Vector3& v) {
		this->x = fmax(this->x, v.x);
		this->y = fmax(this->y, v.y);
		this->z = fmax(this->z, v.z);

		return *this;
	}

	Vector3& clamp(Vector3& min,Vector3& max){
		// assumes min < max, componentwise

		this->x = fmax(min.x, fmin(max.x, this->x));
		this->y = fmax(min.y, fmin(max.y, this->y));
		this->z = fmax(min.z, fmin(max.z, this->z));

		return *this;
	}

	Vector3& clampScalar(double minVal,double maxVal) {
		this->x = fmax(minVal, fmin(maxVal, this->x));
		this->y = fmax(minVal, fmin(maxVal, this->y));
		this->z = fmax(minVal, fmin(maxVal, this->z));

		return *this;
	}

	Vector3& clampLength(double min,double max) {
		//length()函数返回的是Vector的长度
		const T length = length();

		// (length||1)用来保证除数不为0
		divideScalar(length?length:1);
		multiplyScalar(fmax(min, fmin(max,length)));

		return *this;
	}

	Vector3& floor(){
		this->x = floor( this->x );
		this->y = floor( this->y );
		this->z = floor( this->z );

		return *this;
	}

	Vector3& ceil(){
		this->x = ceil( this->x );
		this->y = ceil( this->y );
		this->z = ceil( this->z );

		return *this;
	}

	Vector3& round(){
		this->x = round( this->x );
		this->y = round( this->y );
		this->z = round( this->z );

		return *this;
	}

	Vector3& roundToZero(){
		this->x = ( this->x < 0 ) ? ceil( this->x ) : floor( this->x );
		this->y = ( this->y < 0 ) ? ceil( this->y ) : floor( this->y );
		this->z = ( this->z < 0 ) ? ceil( this->z ) : floor( this->z );

		return *this;
	}

	Vector3& negate(){
		this->x = - this->x;
		this->y = - this->y;
		this->z = - this->z;

		return *this;
	}

	Vector3& cross(Vector3& v) {
		return crossVectors(*this, v);
	}

	Vector3& crossVectors(Vector3& a,Vector3& b){
		const T ax = a.x, ay = a.y, az = a.z;
		const T bx = b.x, by = b.y, bz = b.z;

		this->x = ay * bz - az * by;
		this->y = az * bx - ax * bz;
		this->z = ax * by - ay * bx;

		return *this;
	}

	Vector3& projectOnVector(Vector3& v) {
		const T denominator = v.lengthSq();

		if (denominator == 0) return set(0, 0, 0);

		const double scalar = v.dot(*this) / denominator;

		return this->copy(v).multiplyScalar(scalar);

	}

	Vector3& projectOnPlane(Vector3&  planeNormal) {
		_vector->copy(*this).projectOnVector(planeNormal);

		return sub(_vector);
	}

	Vector3& reflect(Vector3& normal) {
		// reflect incident vector off plane orthogonal to normal
		// normal is assumed to have unit length

		return sub(*_vector.copy(normal).multiplyScalar(2*dot(normal)));
	}

	double angleTo(Vector3& v){
		const double denominator = sqrt(this->lengthSq() * v.lengthSq());

		if (denominator == 0) return M_PI / 2;

		const double theta = this->dot(v) / denominator;

		// clamp, to handle numerical problems
		return acos(clamp(theta, - 1, 1));
	}

	double distanceTo(Vector3&  v) {
		return sqrt(this->distanceToSquared(v));
	}

	double distanceToSquared(Vector3& v){
		const double dx = this->x - v.x, dy = this->y - v.y, dz = this->z - v.z;

		return dx * dx + dy * dy + dz * dz;
	}

	double manhattanDistanceTo(Vector3& v){
		return abs(this->x - v.x) + abs(this->y - v.y) + abs(this->z - v.z);
	}

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

	Vector3& setFromMatrixPosition(Matrix4& m){
		this->x = m.elements[ 12 ];
		this->y = m.elements[ 13 ];
		this->z = m.elements[ 14 ];

		return *this;
	}

	Vector3& setFromMatrixScale(Matrix4& m){
		const double sx = setFromMatrixColumn(m, 0).length();
		const double sy = setFromMatrixColumn(m, 1).length();
		const double sz = setFromMatrixColumn(m, 2).length();

		this->x = sx;
		this->y = sy;
		this->z = sz;

		return *this;
	}

	Vector3& setFromMatrixColumn(Matrix4& m,int index){
		return fromArray(m.elements, index * 4);
	}

// 	setFromMatrix3Column( m, index ) {

// 		return *this-fromArray( m.elements, index * 3 );

// 	}

// 	setFromEuler( e ) {

// 		this->x = e._x;
// 		this->y = e._y;
// 		this->z = e._z;

// 		return *this;

// 	}

	Vector3& fromArray(T array[], int offset = 0 ) {
		x = array[ offset ];
		y = array[ offset + 1 ];
		z = array[ offset + 2 ];

		return *this;
	}

	double* toArray(double array[],int offset = 0){
		array[offset] = this->x;
		array[offset + 1] = this->y;
		array[offset + 2] = this->z;

		return array;
	}

// 	fromBufferAttribute( attribute, index, offset ) {

// 		if ( offset !== undefined ) {

// 			console.warn( 'THREE.Vector3: offset has been removed from .fromBufferAttribute().' );

// 		}

// 		this->x = attribute.getX( index );
// 		this->y = attribute.getY( index );
// 		this->z = attribute.getZ( index );

// 		return *this;

// 	}

	Vector3& random() {
		this->x = random_gen<double>();
		this->y = random_gen<double>();
		this->z = random_gen<double>();

		return *this;
	}

	Vector3& randomDirection() {
		// Derived from https://mathworld.wolfram.com/SpherePointPicking.html

		const double u = (random_gen<double>() - 0.5) * 2;
		const double t = random_gen<double>() * M_PI * 2;
		const double f = sqrt(1 - std::pow(u,2));

		this->x = f * cos(t);
		this->y = f * sin(t);
		this->z = u;

		return *this;
	}

	bool equals(Vector3& v){
		return ( (v.x == this->x) && (v.y == this->y) && (v.z == this->z) );
	}

    private:
		std::shared_ptr<Vector3> _vector;
		std::shared_ptr<Quaternion> _quaternion;		
        
};

//设置常用子类型的alias，以方便使用
// element of type double precision float
typedef Vector3<double> Vector3d;
typedef Vector3<float> Vector3f;
typedef Vector3<unsigned char> Vector3uc;

#endif //VECTOR3_H