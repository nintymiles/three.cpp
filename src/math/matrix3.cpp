#include "matrix3.h"
#include "vector3.h"

Matrix3& Matrix3::extractBasis( Vector3& xAxis,Vector3& yAxis,Vector3& zAxis ) {
	xAxis.setFromMatrix3Column( *this, 0 );
	yAxis.setFromMatrix3Column( *this, 1 );
	zAxis.setFromMatrix3Column( *this, 2 );

	return *this;
}