#include "box3.h"

std::shared_ptr<Box3> Box3::_box = std::make_shared<Box3>();

Vector3 Box3::_points[6]= {Vector3(),Vector3(),Vector3(),Vector3(),Vector3(),Vector3()}; 

template <typename T>
Box3& Box3::setFromBufferAttribute( BufferAttribute<T> attribute ){
	double minX = DBL_MAX;
	double minY = DBL_MAX
	double minZ = DBL_MAX;

	double maxX = DBL_MIN;
	double maxY = DBL_MIN;
	double maxZ = DBL_MIN;

	for ( int i = 0, l = attribute.count; i < l; i ++ ) {

		const double x = attribute.getX( i );
		const double y = attribute.getY( i );
		const double z = attribute.getZ( i );

		if ( x < minX ) minX = x;
		if ( y < minY ) minY = y;
		if ( z < minZ ) minZ = z;

		if ( x > maxX ) maxX = x;
		if ( y > maxY ) maxY = y;
		if ( z > maxZ ) maxZ = z;
	}

	min.set( minX, minY, minZ );
	max.set( maxX, maxY, maxZ );

	return *this;
}