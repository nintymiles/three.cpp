#include "box3.h"

std::shared_ptr<Box3> Box3::_box = std::make_shared<Box3>();

Vector3 Box3::_points[6]= {Vector3(),Vector3(),Vector3(),Vector3(),Vector3(),Vector3()}; 