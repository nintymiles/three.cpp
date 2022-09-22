#include "common_utils.h"


std::runtime_error threecpp::genOutOfRangeError(int index){
    std::stringstream ss;
    ss << "index is out of range: " << index << std::endl;
    return std::runtime_error(ss.str());
}

