#include "matrix3.h"
#include "vector3.h"
#include "matrix4.h"

Matrix3& Matrix3::extractBasis( Vector3& xAxis,Vector3& yAxis,Vector3& zAxis ) {
	xAxis.setFromMatrix3Column( *this, 0 );
	yAxis.setFromMatrix3Column( *this, 1 );
	zAxis.setFromMatrix3Column( *this, 2 );

	return *this;
}

Matrix3& Matrix3::setFromMatrix4( Matrix4& m ) {
	set(
		m.elements[ 0 ], m.elements[ 4 ], m.elements[ 8 ],
		m.elements[ 1 ], m.elements[ 5 ], m.elements[ 9 ],
		m.elements[ 2 ], m.elements[ 6 ], m.elements[ 10 ]
	);

	return *this;
}