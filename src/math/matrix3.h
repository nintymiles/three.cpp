#ifndef MATRIX3_H
#define MATRIX3_H

#include<memory>
#include<cmath>

class Quaternion;
class Vector3;
class Matrix4;

class Matrix3{
    public:
    
    double elements[9] ={
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
        };
    
    Matrix3() = default;

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

	Matrix3 copy(const Matrix3& m) {
        for(int i=0;i<sizeof(elements)/sizeof(elements[0]);i++){
            elements[i] = m.elements[i];
        }

		return *this;
	}

    Matrix3& extractBasis( Vector3& xAxis,Vector3& yAxis,Vector3& zAxis );

	Matrix3& setFromMatrix4( Matrix4& m );

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

		// te[ 0 ] = a11 * b11 + a12 * b21 + a13 * b31;
		// te[ 3 ] = a11 * b12 + a12 * b22 + a13 * b32;
		// te[ 6 ] = a11 * b13 + a12 * b23 + a13 * b33;
		*(te + 0) = a11 * b11 + a12 * b21 + a13 * b31;
		*(te + 3) = a11 * b12 + a12 * b22 + a13 * b32;
		*(te + 6) = a11 * b13 + a12 * b23 + a13 * b33;

		*(te + 1) = a21 * b11 + a22 * b21 + a23 * b31;
		*(te + 4) = a21 * b12 + a22 * b22 + a23 * b32;
		*(te + 7) = a21 * b13 + a22 * b23 + a23 * b33;

		*(te + 2) = a31 * b11 + a32 * b21 + a33 * b31;
		*(te + 5) = a31 * b12 + a32 * b22 + a33 * b32;
		*(te + 8) = a31 * b13 + a32 * b23 + a33 * b33;

		return *this;
	}

	Matrix3& multiplyScalar( double s ) {
		// double te[9];
		// memcpy(te,elements,sizeof(te));
		//const te = this.elements;
		elements[ 0 ] *= s; elements[ 3 ] *= s; elements[ 6 ] *= s;
		elements[ 1 ] *= s; elements[ 4 ] *= s; elements[ 7 ] *= s;
		elements[ 2 ] *= s; elements[ 5 ] *= s; elements[ 8 ] *= s;

		return *this;
	}

	double determinant() {
		//const te = this.elements;
		double te[9];
		memcpy(te,elements,sizeof(te));

		const double
			a = te[ 0 ], b = te[ 1 ], c = te[ 2 ],
			d = te[ 3 ], e = te[ 4 ], f = te[ 5 ],
			g = te[ 6 ], h = te[ 7 ], i = te[ 8 ];

		return a * e * i - a * f * h - b * d * i + b * f * g + c * d * h - c * e * g;
	}

	Matrix3& invert() {
		//const te = this.elements,
		//double te[9];
		//memcpy(te,elements,sizeof(te));

		const double
			n11 = elements[ 0 ], n21 = elements[ 1 ], n31 = elements[ 2 ],
			n12 = elements[ 3 ], n22 = elements[ 4 ], n32 = elements[ 5 ],
			n13 = elements[ 6 ], n23 = elements[ 7 ], n33 = elements[ 8 ],

			t11 = n33 * n22 - n32 * n23,
			t12 = n32 * n13 - n33 * n12,
			t13 = n23 * n12 - n22 * n13,

			det = n11 * t11 + n21 * t12 + n31 * t13;

		if ( det == 0 ) return set( 0, 0, 0, 0, 0, 0, 0, 0, 0 );

		const double detInv = 1 / det;

		elements[ 0 ] = t11 * detInv;
		elements[ 1 ] = ( n31 * n23 - n33 * n21 ) * detInv;
		elements[ 2 ] = ( n32 * n21 - n31 * n22 ) * detInv;

		elements[ 3 ] = t12 * detInv;
		elements[ 4 ] = ( n33 * n11 - n31 * n13 ) * detInv;
		elements[ 5 ] = ( n31 * n12 - n32 * n11 ) * detInv;

		elements[ 6 ] = t13 * detInv;
		elements[ 7 ] = ( n21 * n13 - n23 * n11 ) * detInv;
		elements[ 8 ] = ( n22 * n11 - n21 * n12 ) * detInv;

		return *this;
	}

	Matrix3& transpose() {
		double tmp;

		tmp = elements[ 1 ]; elements[ 1 ] = elements[ 3 ]; elements[ 3 ] = tmp;
		tmp = elements[ 2 ]; elements[ 2 ] = elements[ 6 ]; elements[ 6 ] = tmp;
		tmp = elements[ 5 ]; elements[ 5 ] = elements[ 7 ]; elements[ 7 ] = tmp;

		return *this;
	}

	Matrix3& getNormalMatrix(Matrix4& matrix4 ) {
		return setFromMatrix4( matrix4 ).invert().transpose();
	}

	double* transposeIntoArray( double r[] ) {
		r[ 0 ] = elements[ 0 ];
		r[ 1 ] = elements[ 3 ];
		r[ 2 ] = elements[ 6 ];
		r[ 3 ] = elements[ 1 ];
		r[ 4 ] = elements[ 4 ];
		r[ 5 ] = elements[ 7 ];
		r[ 6 ] = elements[ 2 ];
		r[ 7 ] = elements[ 5 ];
		r[ 8 ] = elements[ 8 ];

		return r;
	}

	Matrix3& setUvTransform( double tx,double ty,double sx,double sy,double rotation,double cx,double cy ) {
		const double c = cos( rotation );
		const double s = sin( rotation );

		set(
			sx * c, sx * s, - sx * ( c * cx + s * cy ) + cx + tx,
			- sy * s, sy * c, - sy * ( - s * cx + c * cy ) + cy + ty,
			0, 0, 1
		);

		return *this;
	}

	Matrix3& scale(double sx,double sy) {
		elements[ 0 ] *= sx; elements[ 3 ] *= sx; elements[ 6 ] *= sx;
		elements[ 1 ] *= sy; elements[ 4 ] *= sy; elements[ 7 ] *= sy;

		return *this;
	}

	Matrix3& rotate( double theta ) {
		const double c = cos( theta );
		const double s = sin( theta );

		const double a11 = elements[ 0 ], a12 = elements[ 3 ], a13 = elements[ 6 ];
		const double a21 = elements[ 1 ], a22 = elements[ 4 ], a23 = elements[ 7 ];

		elements[ 0 ] = c * a11 + s * a21;
		elements[ 3 ] = c * a12 + s * a22;
		elements[ 6 ] = c * a13 + s * a23;

		elements[ 1 ] = - s * a11 + c * a21;
		elements[ 4 ] = - s * a12 + c * a22;
		elements[ 7 ] = - s * a13 + c * a23;

		return *this;
	}

	Matrix3& translate( double tx,double ty ) {
		elements[ 0 ] += tx * elements[ 2 ]; elements[ 3 ] += tx * elements[ 5 ]; elements[ 6 ] += tx * elements[ 8 ];
		elements[ 1 ] += ty * elements[ 2 ]; elements[ 4 ] += ty * elements[ 5 ]; elements[ 7 ] += ty * elements[ 8 ];

		return *this;
	}

	bool equals( Matrix3& matrix ) {
		for ( int i = 0; i < 9; i ++ ) {
			if ( elements[ i ] != matrix.elements[ i ] ) 
				return false;
		}

		return true;
	}

	Matrix3& fromArray( double array[], int offset = 0 ) {
		for ( int i = 0; i < 9; i ++ ) {

			elements[ i ] = array[ i + offset ];

		}

		return *this;
	}

	double* toArray( double array[], int offset = 0 ) {
		array[ offset ] = elements[ 0 ];
		array[ offset + 1 ] = elements[ 1 ];
		array[ offset + 2 ] = elements[ 2 ];

		array[ offset + 3 ] = elements[ 3 ];
		array[ offset + 4 ] = elements[ 4 ];
		array[ offset + 5 ] = elements[ 5 ];

		array[ offset + 6 ] = elements[ 6 ];
		array[ offset + 7 ] = elements[ 7 ];
		array[ offset + 8 ] = elements[ 8 ];

		return array;
	}


    private:


};


#endif //MATRIX3_H
