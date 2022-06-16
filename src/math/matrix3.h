#ifndef MATRIX3_H
#define MATRIX3_H

#include "vector3.h"
#include<memory>
#include<cmath>

class Quaternion;
class Vector3;

class Matrix3{
    public:
    
    double elements[9] ={
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
        };
    
    Matrix3();

	Matrix3& set(double n11,double n12,double n13,
                double n21,double n22,double n23,
                double n31,double n32,double n33) {

		elements[ 0 ] = n11; elements[ 3 ] = n12; elements[ 6 ] = n13;
		elements[ 1 ] = n21; elements[ 4 ] = n22; elements[ 7 ] = n23;
		elements[ 2 ] = n31; elements[ 5 ] = n32; elements[ 8 ] = n33;
		

		return *this;
	}
	Matrix3& identity() {
		set(
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		);
        return *this;
    }

	Matrix3 clone(){
		return Matrix3().fromArray(elements);
	}

	Matrix3 copy(Matrix3& m) {
        for(int i=0;i<sizeof(elements)/sizeof(elements[0]);i++){
            elements[i] = m.elements[i];
        }

		return *this;
	}

    // Matrix3& extractBasis( Vector3& xAxis,Vector3& yAxis,Vector3& zAxis ) {
	// 	xAxis.setFromMatrix3Column( *this, 0 );
	// 	yAxis.setFromMatrix3Column( *this, 1 );
	// 	zAxis.setFromMatrix3Column( *this, 2 );

	// 	return *this;
	// }

	Matrix3& setFromMatrix4( Matrix4& m ) {
		set(
			m.elements[ 0 ], m.elements[ 4 ], m.elements[ 8 ],
			m.elements[ 1 ], m.elements[ 5 ], m.elements[ 9 ],
			m.elements[ 2 ], m.elements[ 6 ], m.elements[ 10 ]
		);

		return *this;
	}

	Matrix3& multiply( Matrix3& m ) {
		return multiplyMatrices( *this, m );
	}

	Matrix3& premultiply( Matrix3& m ) {
		return multiplyMatrices( m, *this );
	}

	Matrix3& multiplyMatrices( Matrix3& a, Matrix3& b ) {
		double (*ae) = a.elements;
		double (*be) = b.elements;
		double (*te) = elements;

		// const ae = a.elements;
		// const be = b.elements;
		// const te = this.elements;

		// const double a11 = ae[ 0 ], a12 = ae[ 3 ], a13 = ae[ 6 ];
		// const double a21 = ae[ 1 ], a22 = ae[ 4 ], a23 = ae[ 7 ];
		// const double a31 = ae[ 2 ], a32 = ae[ 5 ], a33 = ae[ 8 ];
		const double a11 = *(ae + 0), a12 = *(ae + 3), a13 = *(ae + 6);
		const double a21 = *(ae + 1), a22 = *(ae + 4), a23 = *(ae + 7);
		const double a31 = *(ae + 2), a32 = *(ae + 5), a33 = *(ae + 8);

		// const double b11 = be[ 0 ], b12 = be[ 3 ], b13 = be[ 6 ];
		// const double b21 = be[ 1 ], b22 = be[ 4 ], b23 = be[ 7 ];
		// const double b31 = be[ 2 ], b32 = be[ 5 ], b33 = be[ 8 ];
		const double b11 = *(be + 0), b12 = *(be + 3), b13 = *(be + 6);
		const double b21 = *(be + 1), b22 = *(be + 4), b23 = *(be + 7);
		const double b31 = *(be + 2), b32 = *(be + 5), b33 = *(be + 8);

		te[ 0 ] = a11 * b11 + a12 * b21 + a13 * b31;
		te[ 3 ] = a11 * b12 + a12 * b22 + a13 * b32;
		te[ 6 ] = a11 * b13 + a12 * b23 + a13 * b33;

		te[ 1 ] = a21 * b11 + a22 * b21 + a23 * b31;
		te[ 4 ] = a21 * b12 + a22 * b22 + a23 * b32;
		te[ 7 ] = a21 * b13 + a22 * b23 + a23 * b33;

		te[ 2 ] = a31 * b11 + a32 * b21 + a33 * b31;
		te[ 5 ] = a31 * b12 + a32 * b22 + a33 * b32;
		te[ 8 ] = a31 * b13 + a32 * b23 + a33 * b33;

		return *this;
	}

	Matrix3& multiplyScalar( double s ) {
		const te = this.elements;

		te[ 0 ] *= s; te[ 3 ] *= s; te[ 6 ] *= s;
		te[ 1 ] *= s; te[ 4 ] *= s; te[ 7 ] *= s;
		te[ 2 ] *= s; te[ 5 ] *= s; te[ 8 ] *= s;

		return *this;
	}

	double determinant() {
		const te = this.elements;

		const a = te[ 0 ], b = te[ 1 ], c = te[ 2 ],
			d = te[ 3 ], e = te[ 4 ], f = te[ 5 ],
			g = te[ 6 ], h = te[ 7 ], i = te[ 8 ];

		return a * e * i - a * f * h - b * d * i + b * f * g + c * d * h - c * e * g;
	}

	Matrix3& invert() {

		const te = this.elements,

			n11 = te[ 0 ], n21 = te[ 1 ], n31 = te[ 2 ],
			n12 = te[ 3 ], n22 = te[ 4 ], n32 = te[ 5 ],
			n13 = te[ 6 ], n23 = te[ 7 ], n33 = te[ 8 ],

			t11 = n33 * n22 - n32 * n23,
			t12 = n32 * n13 - n33 * n12,
			t13 = n23 * n12 - n22 * n13,

			det = n11 * t11 + n21 * t12 + n31 * t13;

		if ( det === 0 ) return this.set( 0, 0, 0, 0, 0, 0, 0, 0, 0 );

		const detInv = 1 / det;

		te[ 0 ] = t11 * detInv;
		te[ 1 ] = ( n31 * n23 - n33 * n21 ) * detInv;
		te[ 2 ] = ( n32 * n21 - n31 * n22 ) * detInv;

		te[ 3 ] = t12 * detInv;
		te[ 4 ] = ( n33 * n11 - n31 * n13 ) * detInv;
		te[ 5 ] = ( n31 * n12 - n32 * n11 ) * detInv;

		te[ 6 ] = t13 * detInv;
		te[ 7 ] = ( n21 * n13 - n23 * n11 ) * detInv;
		te[ 8 ] = ( n22 * n11 - n21 * n12 ) * detInv;

		return this;

	}

	Matrix3& transpose() {

		let tmp;
		const m = this.elements;

		tmp = m[ 1 ]; m[ 1 ] = m[ 3 ]; m[ 3 ] = tmp;
		tmp = m[ 2 ]; m[ 2 ] = m[ 6 ]; m[ 6 ] = tmp;
		tmp = m[ 5 ]; m[ 5 ] = m[ 7 ]; m[ 7 ] = tmp;

		return this;

	}

	Matrix3& getNormalMatrix( matrix4 ) {

		return this.setFromMatrix4( matrix4 ).invert().transpose();

	}

	double* transposeIntoArray( (double*) r ) {

		const m = this.elements;

		r[ 0 ] = m[ 0 ];
		r[ 1 ] = m[ 3 ];
		r[ 2 ] = m[ 6 ];
		r[ 3 ] = m[ 1 ];
		r[ 4 ] = m[ 4 ];
		r[ 5 ] = m[ 7 ];
		r[ 6 ] = m[ 2 ];
		r[ 7 ] = m[ 5 ];
		r[ 8 ] = m[ 8 ];

		return this;

	}

	Matrix3& setUvTransform( tx, ty, sx, sy, rotation, cx, cy ) {

		const c = Math.cos( rotation );
		const s = Math.sin( rotation );

		this.set(
			sx * c, sx * s, - sx * ( c * cx + s * cy ) + cx + tx,
			- sy * s, sy * c, - sy * ( - s * cx + c * cy ) + cy + ty,
			0, 0, 1
		);

		return this;

	}

	Matrix3& scale( sx, sy ) {

		const te = this.elements;

		te[ 0 ] *= sx; te[ 3 ] *= sx; te[ 6 ] *= sx;
		te[ 1 ] *= sy; te[ 4 ] *= sy; te[ 7 ] *= sy;

		return this;

	}

	Matrix3& rotate( theta ) {

		const c = Math.cos( theta );
		const s = Math.sin( theta );

		const te = this.elements;

		const a11 = te[ 0 ], a12 = te[ 3 ], a13 = te[ 6 ];
		const a21 = te[ 1 ], a22 = te[ 4 ], a23 = te[ 7 ];

		te[ 0 ] = c * a11 + s * a21;
		te[ 3 ] = c * a12 + s * a22;
		te[ 6 ] = c * a13 + s * a23;

		te[ 1 ] = - s * a11 + c * a21;
		te[ 4 ] = - s * a12 + c * a22;
		te[ 7 ] = - s * a13 + c * a23;

		return this;

	}

	Matrix3& translate( tx, ty ) {

		const te = this.elements;

		te[ 0 ] += tx * te[ 2 ]; te[ 3 ] += tx * te[ 5 ]; te[ 6 ] += tx * te[ 8 ];
		te[ 1 ] += ty * te[ 2 ]; te[ 4 ] += ty * te[ 5 ]; te[ 7 ] += ty * te[ 8 ];

		return this;

	}

	bool equals( Matrix3& matrix ) {

		const te = this.elements;
		const me = matrix.elements;

		for ( let i = 0; i < 9; i ++ ) {

			if ( te[ i ] !== me[ i ] ) return false;

		}

		return true;

	}

	Matrix3& fromArray( array, int offset = 0 ) {

		for ( let i = 0; i < 9; i ++ ) {

			this.elements[ i ] = array[ i + offset ];

		}

		return this;

	}

	toArray( array = [], offset = 0 ) {

		const te = this.elements;

		array[ offset ] = te[ 0 ];
		array[ offset + 1 ] = te[ 1 ];
		array[ offset + 2 ] = te[ 2 ];

		array[ offset + 3 ] = te[ 3 ];
		array[ offset + 4 ] = te[ 4 ];
		array[ offset + 5 ] = te[ 5 ];

		array[ offset + 6 ] = te[ 6 ];
		array[ offset + 7 ] = te[ 7 ];
		array[ offset + 8 ] = te[ 8 ];

		return array;

	}


    private:


};


#endif //MATRIX4_H
