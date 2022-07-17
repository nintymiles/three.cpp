#ifndef MATRIX4_H
#define MATRIX4_H

//#include "vector3.h"
#include<memory>
#include<cmath>

class Quaternion;
class Vector3;

class Matrix4{
    public:
    
    double elements[16] ={
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
        };
    
    Matrix4();

	Matrix4& set(double n11,double n12,double n13,double n14, 
                double n21,double n22,double n23,double n24,
                double n31,double n32,double n33,double n34,
                double n41,double n42,double n43,double n44 ) {

		elements[ 0 ] = n11; elements[ 4 ] = n12; elements[ 8 ] = n13; elements[ 12 ] = n14;
		elements[ 1 ] = n21; elements[ 5 ] = n22; elements[ 9 ] = n23; elements[ 13 ] = n24;
		elements[ 2 ] = n31; elements[ 6 ] = n32; elements[ 10 ] = n33; elements[ 14 ] = n34;
		elements[ 3 ] = n41; elements[ 7 ] = n42; elements[ 11 ] = n43; elements[ 15 ] = n44;

		return *this;

	}
	Matrix4& identity() {
		set(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
        return *this;
    }

	Matrix4 clone(){
		return Matrix4().fromArray(elements);
	}

	Matrix4& copy(Matrix4& m) {
        for(int i=0;i<sizeof(elements)/sizeof(elements[0]);i++){
            elements[i] = m.elements[i];
        }

		return *this;
	}

    Matrix4& copyPosition(const Matrix4& m) {
		elements[ 12 ] = m.elements[ 12 ];
		elements[ 13 ] = m.elements[ 13 ];
		elements[ 14 ] = m.elements[ 14 ];

		return *this;
	}
// 	setFromMatrix3( m ) {

// 		const me = m.elements;

// 		this.set(

// 			me[ 0 ], me[ 3 ], me[ 6 ], 0,
// 			me[ 1 ], me[ 4 ], me[ 7 ], 0,
// 			me[ 2 ], me[ 5 ], me[ 8 ], 0,
// 			0, 0, 0, 1

// 		);

// 		return this;

// 	}

	Matrix4& extractBasis(Vector3& xAxis,Vector3& yAxis,Vector3& zAxis);

	Matrix4& makeBasis(Vector3& xAxis,Vector3& yAxis,Vector3& zAxis);

	Matrix4& extractRotation(Matrix4& m);

// 	makeRotationFromEuler( euler ) {

// 		if ( ! ( euler && euler.isEuler ) ) {

// 			console.error( 'THREE.Matrix4: .makeRotationFromEuler() now expects a Euler rotation rather than a Vector3 and order.' );

// 		}

// 		const te = this.elements;

// 		const x = euler.x, y = euler.y, z = euler.z;
// 		const a = Math.cos( x ), b = Math.sin( x );
// 		const c = Math.cos( y ), d = Math.sin( y );
// 		const e = Math.cos( z ), f = Math.sin( z );

// 		if ( euler.order === 'XYZ' ) {

// 			const ae = a * e, af = a * f, be = b * e, bf = b * f;

// 			te[ 0 ] = c * e;
// 			te[ 4 ] = - c * f;
// 			te[ 8 ] = d;

// 			te[ 1 ] = af + be * d;
// 			te[ 5 ] = ae - bf * d;
// 			te[ 9 ] = - b * c;

// 			te[ 2 ] = bf - ae * d;
// 			te[ 6 ] = be + af * d;
// 			te[ 10 ] = a * c;

// 		} else if ( euler.order === 'YXZ' ) {

// 			const ce = c * e, cf = c * f, de = d * e, df = d * f;

// 			te[ 0 ] = ce + df * b;
// 			te[ 4 ] = de * b - cf;
// 			te[ 8 ] = a * d;

// 			te[ 1 ] = a * f;
// 			te[ 5 ] = a * e;
// 			te[ 9 ] = - b;

// 			te[ 2 ] = cf * b - de;
// 			te[ 6 ] = df + ce * b;
// 			te[ 10 ] = a * c;

// 		} else if ( euler.order === 'ZXY' ) {

// 			const ce = c * e, cf = c * f, de = d * e, df = d * f;

// 			te[ 0 ] = ce - df * b;
// 			te[ 4 ] = - a * f;
// 			te[ 8 ] = de + cf * b;

// 			te[ 1 ] = cf + de * b;
// 			te[ 5 ] = a * e;
// 			te[ 9 ] = df - ce * b;

// 			te[ 2 ] = - a * d;
// 			te[ 6 ] = b;
// 			te[ 10 ] = a * c;

// 		} else if ( euler.order === 'ZYX' ) {

// 			const ae = a * e, af = a * f, be = b * e, bf = b * f;

// 			te[ 0 ] = c * e;
// 			te[ 4 ] = be * d - af;
// 			te[ 8 ] = ae * d + bf;

// 			te[ 1 ] = c * f;
// 			te[ 5 ] = bf * d + ae;
// 			te[ 9 ] = af * d - be;

// 			te[ 2 ] = - d;
// 			te[ 6 ] = b * c;
// 			te[ 10 ] = a * c;

// 		} else if ( euler.order === 'YZX' ) {

// 			const ac = a * c, ad = a * d, bc = b * c, bd = b * d;

// 			te[ 0 ] = c * e;
// 			te[ 4 ] = bd - ac * f;
// 			te[ 8 ] = bc * f + ad;

// 			te[ 1 ] = f;
// 			te[ 5 ] = a * e;
// 			te[ 9 ] = - b * e;

// 			te[ 2 ] = - d * e;
// 			te[ 6 ] = ad * f + bc;
// 			te[ 10 ] = ac - bd * f;

// 		} else if ( euler.order === 'XZY' ) {

// 			const ac = a * c, ad = a * d, bc = b * c, bd = b * d;

// 			te[ 0 ] = c * e;
// 			te[ 4 ] = - f;
// 			te[ 8 ] = d * e;

// 			te[ 1 ] = ac * f + bd;
// 			te[ 5 ] = a * e;
// 			te[ 9 ] = ad * f - bc;

// 			te[ 2 ] = bc * f - ad;
// 			te[ 6 ] = b * e;
// 			te[ 10 ] = bd * f + ac;

// 		}

// 		// bottom row
// 		te[ 3 ] = 0;
// 		te[ 7 ] = 0;
// 		te[ 11 ] = 0;

// 		// last column
// 		te[ 12 ] = 0;
// 		te[ 13 ] = 0;
// 		te[ 14 ] = 0;
// 		te[ 15 ] = 1;

// 		return this;

// 	}

	Matrix4& makeRotationFromQuaternion(Quaternion& q);

	Matrix4& lookAt(Vector3& eye,Vector3& target,Vector3& up); 

	Matrix4& multiply(Matrix4& m) {
		return multiplyMatrices(*this, m);
	}

	Matrix4& premultiply(Matrix4& m){
		return multiplyMatrices(m, *this);
	}

    //实现一个operator*更好
	Matrix4& multiplyMatrices(Matrix4& a,Matrix4& b){
		const double a11 = a.elements[ 0 ], a12 = a.elements[ 4 ], a13 = a.elements[ 8 ], a14 = a.elements[ 12 ];
		const double a21 = a.elements[ 1 ], a22 = a.elements[ 5 ], a23 = a.elements[ 9 ], a24 = a.elements[ 13 ];
		const double a31 = a.elements[ 2 ], a32 = a.elements[ 6 ], a33 = a.elements[ 10 ], a34 = a.elements[ 14 ];
		const double a41 = a.elements[ 3 ], a42 = a.elements[ 7 ], a43 = a.elements[ 11 ], a44 = a.elements[ 15 ];

		const double b11 = b.elements[ 0 ], b12 = b.elements[ 4 ], b13 = b.elements[ 8 ], b14 = b.elements[ 12 ];
		const double b21 = b.elements[ 1 ], b22 = b.elements[ 5 ], b23 = b.elements[ 9 ], b24 = b.elements[ 13 ];
		const double b31 = b.elements[ 2 ], b32 = b.elements[ 6 ], b33 = b.elements[ 10 ], b34 = b.elements[ 14 ];
		const double b41 = b.elements[ 3 ], b42 = b.elements[ 7 ], b43 = b.elements[ 11 ], b44 = b.elements[ 15 ];

		elements[ 0 ] = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
		elements[ 4 ] = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
		elements[ 8 ] = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
		elements[ 12 ] = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;

		elements[ 1 ] = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
		elements[ 5 ] = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
		elements[ 9 ] = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
		elements[ 13 ] = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;

		elements[ 2 ] = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
		elements[ 6 ] = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
		elements[ 10 ] = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
		elements[ 14 ] = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;

		elements[ 3 ] = a41 * b11 + a42 * b21 + a43 * b31 + a44 * b41;
		elements[ 7 ] = a41 * b12 + a42 * b22 + a43 * b32 + a44 * b42;
		elements[ 11 ] = a41 * b13 + a42 * b23 + a43 * b33 + a44 * b43;
		elements[ 15 ] = a41 * b14 + a42 * b24 + a43 * b34 + a44 * b44;

		return *this;

	}

	Matrix4& multiplyScalar(double s) {
		elements[ 0 ] *= s; elements[ 4 ] *= s; elements[ 8 ] *= s; elements[ 12 ] *= s;
		elements[ 1 ] *= s; elements[ 5 ] *= s; elements[ 9 ] *= s; elements[ 13 ] *= s;
		elements[ 2 ] *= s; elements[ 6 ] *= s; elements[ 10 ] *= s; elements[ 14 ] *= s;
		elements[ 3 ] *= s; elements[ 7 ] *= s; elements[ 11 ] *= s; elements[ 15 ] *= s;

		return *this;
	}

	double determinant(){
        const double
		     n11 = elements[ 0 ], n12 = elements[ 4 ], n13 = elements[ 8 ], n14 = elements[ 12 ],
		     n21 = elements[ 1 ], n22 = elements[ 5 ], n23 = elements[ 9 ], n24 = elements[ 13 ],
		     n31 = elements[ 2 ], n32 = elements[ 6 ], n33 = elements[ 10 ], n34 = elements[ 14 ],
	         n41 = elements[ 3 ], n42 = elements[ 7 ], n43 = elements[ 11 ], n44 = elements[ 15 ];

		//TODO: make this more efficient
		//( based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm )

		return (
			n41 * (
				+ n14 * n23 * n32
				 - n13 * n24 * n32
				 - n14 * n22 * n33
				 + n12 * n24 * n33
				 + n13 * n22 * n34
				 - n12 * n23 * n34
			) +
			n42 * (
				+ n11 * n23 * n34
				 - n11 * n24 * n33
				 + n14 * n21 * n33
				 - n13 * n21 * n34
				 + n13 * n24 * n31
				 - n14 * n23 * n31
			) +
			n43 * (
				+ n11 * n24 * n32
				 - n11 * n22 * n34
				 - n14 * n21 * n32
				 + n12 * n21 * n34
				 + n14 * n22 * n31
				 - n12 * n24 * n31
			) +
			n44 * (
				- n13 * n22 * n31
				 - n11 * n23 * n32
				 + n11 * n22 * n33
				 + n13 * n21 * n32
				 - n12 * n21 * n33
				 + n12 * n23 * n31
			)

		);
	}

	Matrix4& transpose(){
		double tmp;

		tmp = elements[ 1 ]; elements[ 1 ] = elements[ 4 ]; elements[ 4 ] = tmp;
		tmp = elements[ 2 ]; elements[ 2 ] = elements[ 8 ]; elements[ 8 ] = tmp;
		tmp = elements[ 6 ]; elements[ 6 ] = elements[ 9 ]; elements[ 9 ] = tmp;

		tmp = elements[ 3 ]; elements[ 3 ] = elements[ 12 ]; elements[ 12 ] = tmp;
		tmp = elements[ 7 ]; elements[ 7 ] = elements[ 13 ]; elements[ 13 ] = tmp;
		tmp = elements[ 11 ]; elements[ 11 ] = elements[ 14 ]; elements[ 14 ] = tmp;

		return *this;
	}

	Matrix4& setPosition(Vector3& v);

    Matrix4& setPosition(double x,double y,double z){
        elements[ 12 ] = x;
        elements[ 13 ] = y;
        elements[ 14 ] = z;

        return *this;
    }

    Matrix4& invert() {
		// based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
		const double 
            n11 = elements[ 0 ], n21 = elements[ 1 ], n31 = elements[ 2 ], n41 = elements[ 3 ],
			n12 = elements[ 4 ], n22 = elements[ 5 ], n32 = elements[ 6 ], n42 = elements[ 7 ],
			n13 = elements[ 8 ], n23 = elements[ 9 ], n33 = elements[ 10 ], n43 = elements[ 11 ],
			n14 = elements[ 12 ], n24 = elements[ 13 ], n34 = elements[ 14 ], n44 = elements[ 15 ],

			t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44,
			t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44,
			t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44,
			t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

		const double det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;

		if (det == 0) return set( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

		const double detInv = 1 / det;

		elements[ 0 ] = t11 * detInv;
		elements[ 1 ] = ( n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44 ) * detInv;
		elements[ 2 ] = ( n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44 ) * detInv;
		elements[ 3 ] = ( n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43 ) * detInv;

		elements[ 4 ] = t12 * detInv;
		elements[ 5 ] = ( n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44 ) * detInv;
		elements[ 6 ] = ( n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44 ) * detInv;
		elements[ 7 ] = ( n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43 ) * detInv;

		elements[ 8 ] = t13 * detInv;
		elements[ 9 ] = ( n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44 ) * detInv;
		elements[ 10 ] = ( n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44 ) * detInv;
		elements[ 11 ] = ( n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43 ) * detInv;

		elements[ 12 ] = t14 * detInv;
		elements[ 13 ] = ( n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34 ) * detInv;
		elements[ 14 ] = ( n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34 ) * detInv;
		elements[ 15 ] = ( n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33 ) * detInv;

		return *this;
	}

	Matrix4& scale(Vector3& v);

	double getMaxScaleOnAxis() {
		const double scaleXSq = elements[ 0 ] * elements[ 0 ] + elements[ 1 ] * elements[ 1 ] + elements[ 2 ] * elements[ 2 ];
		const double scaleYSq = elements[ 4 ] * elements[ 4 ] + elements[ 5 ] * elements[ 5 ] + elements[ 6 ] * elements[ 6 ];
		const double scaleZSq = elements[ 8 ] * elements[ 8 ] + elements[ 9 ] * elements[ 9 ] + elements[ 10 ] * elements[ 10 ];

		return sqrt(fmax(fmax(scaleXSq, scaleYSq), scaleZSq));
	}

	Matrix4& makeTranslation(double x,double y,double z){
		set(
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1
		);
		return *this;
	}

    //rotate theta degress around x axis
	Matrix4& makeRotationX(double theta){

		const double c = cos(theta), s = sin(theta);

		set(
			1, 0, 0, 0,
			0, c, - s, 0,
			0, s, c, 0,
			0, 0, 0, 1
		);

		return *this;
	}

	Matrix4& makeRotationY(double theta){
		const double c = cos(theta), s = sin(theta);

        set(
			 c, 0, s, 0,
			 0, 1, 0, 0,
			- s, 0, c, 0,
			 0, 0, 0, 1
		);

		return *this;
	}

	Matrix4& makeRotationZ(double theta){
        const double c = cos(theta), s = sin(theta);

		set(
			c, - s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);

		return *this;
	}

    //rotate angle degrees around any axis 
	Matrix4& makeRotationAxis(Vector3& axis, double angle);

	Matrix4& makeScale(double x,double y,double z){
		set(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		);
		return *this;
	}

	Matrix4& makeShear(double xy,double xz,double yx,double yz,double zx,double zy){
		set(
			1, yx, zx, 0,
			xy, 1, zy, 0,
			xz, yz, 1, 0,
			0, 0, 0, 1
		);

		return *this;
	}

	Matrix4& compose(const Vector3& position,const Quaternion& quaternion,const Vector3& scale);

	Matrix4& decompose(Vector3& position,Quaternion& quaternion,Vector3& scale);

	Matrix4& makePerspective(double left,double right,double top,double bottom,double near,double far){
		const double x = 2 * near / ( right - left );
		const double y = 2 * near / ( top - bottom );

		const double a = ( right + left ) / ( right - left );
		const double b = ( top + bottom ) / ( top - bottom );
		const double c = - ( far + near ) / ( far - near );
		const double d = - 2 * far * near / ( far - near );

		elements[ 0 ] = x;	elements[ 4 ] = 0;	elements[ 8 ] = a;	elements[ 12 ] = 0;
		elements[ 1 ] = 0;	elements[ 5 ] = y;	elements[ 9 ] = b;	elements[ 13 ] = 0;
		elements[ 2 ] = 0;	elements[ 6 ] = 0;	elements[ 10 ] = c;	elements[ 14 ] = d;
		elements[ 3 ] = 0;	elements[ 7 ] = 0;	elements[ 11 ] = - 1;	elements[ 15 ] = 0;

		return *this;
	}

	Matrix4& makeOrthographic(double left,double right,double top,double bottom,double near,double far){
		const double w = 1.0 / ( right - left );
		const double h = 1.0 / ( top - bottom );
		const double p = 1.0 / ( far - near );

		const double x = ( right + left ) * w;
		const double y = ( top + bottom ) * h;
		const double z = ( far + near ) * p;

		elements[ 0 ] = 2 * w;	elements[ 4 ] = 0;	elements[ 8 ] = 0;	elements[ 12 ] = - x;
		elements[ 1 ] = 0;	elements[ 5 ] = 2 * h;	elements[ 9 ] = 0;	elements[ 13 ] = - y;
		elements[ 2 ] = 0;	elements[ 6 ] = 0;	elements[ 10 ] = - 2 * p;	elements[ 14 ] = - z;
		elements[ 3 ] = 0;	elements[ 7 ] = 0;	elements[ 11 ] = 0;	elements[ 15 ] = 1;

		return *this;
	}

	bool equals(const Matrix4& m) {
		for (int i = 0; i < 16; i ++) {
			if (elements[ i ] != m.elements[ i ]) return false;
		}

		return true;
	}

	Matrix4& fromArray(double array[], int offset = 0 ) {
		for (int i = 0; i < 16; i ++) {
			elements[ i ] = array[ i + offset ];
		}
		return *this;
	}

	double* toArray(double array[],int offset = 0){
		array[ offset ] = elements[ 0 ];
		array[ offset + 1 ] = elements[ 1 ];
		array[ offset + 2 ] = elements[ 2 ];
		array[ offset + 3 ] = elements[ 3 ];

		array[ offset + 4 ] = elements[ 4 ];
		array[ offset + 5 ] = elements[ 5 ];
		array[ offset + 6 ] = elements[ 6 ];
		array[ offset + 7 ] = elements[ 7 ];

		array[ offset + 8 ] = elements[ 8 ];
		array[ offset + 9 ] = elements[ 9 ];
		array[ offset + 10 ] = elements[ 10 ];
		array[ offset + 11 ] = elements[ 11 ];

		array[ offset + 12 ] = elements[ 12 ];
		array[ offset + 13 ] = elements[ 13 ];
		array[ offset + 14 ] = elements[ 14 ];
		array[ offset + 15 ] = elements[ 15 ];

		return array;
	}



    private:
        std::shared_ptr<Vector3> _v1;
        std::shared_ptr<Matrix4>  _m1;
        std::shared_ptr<Vector3> _zero;
        std::shared_ptr<Vector3> _one;
        std::shared_ptr<Vector3> _x;
        std::shared_ptr<Vector3> _y;
        std::shared_ptr<Vector3> _z;

		//      //std::shared_ptr<Vector3> _v1;
		// Vector3 _v1;
        // //std::shared_ptr<Matrix4>  _m1;
		// Matrix4 _m1;
        // //std::shared_ptr<Vector3> _zero;
		// Vector3 _zero;
        // //std::shared_ptr<Vector3> _one;
		// Vector3 _one;
        // //std::shared_ptr<Vector3> _x;
        // //std::shared_ptr<Vector3> _y;
        // //std::shared_ptr<Vector3> _z;
		// Vector3 _x,_y,_z;

};


#endif //MATRIX4_H
