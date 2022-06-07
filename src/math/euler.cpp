#include "euler.h"

Euler::Euler():_matrix(std::make_shared<Matrix4>(new Matrix4())),_quaternion(std::make_shared<Quaternion>(new Quaternion())){}