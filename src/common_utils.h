#ifndef SRC_COMMON_UTILS_H_
#define SRC_COMMON_UTILS_H_

#include <exception>
#include <sstream>

namespace threecpp{

//单独的函数声明和定义一定要分开，否则被多次include后，就会出现duplicate symbols
//function decalaration and defintion must be seperated，or else 
//will occur duplicate symbols after included more than once
std::runtime_error genOutOfRangeError(int index);


} // namespace threecpp


#endif /* SRC_COMMON_UTILS_H_ */
