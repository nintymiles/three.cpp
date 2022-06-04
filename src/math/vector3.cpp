#include "vector3.h"

Vector3::Vector3():_vector(std::make_shared(new Vector3())),
                                _quaternion(std::make_shared(new Quaternion())){}
