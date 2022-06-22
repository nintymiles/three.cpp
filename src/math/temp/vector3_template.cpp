#include "vector3.h"

template<typename T> 
Vector3<T>::Vector3():_vector(std::make_shared(new Vector3())),
                                _quaternion(std::make_shared(new Quaternion())){}
