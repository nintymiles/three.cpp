#include "common_utils.h"

#include <filesystem>


std::runtime_error threecpp::genOutOfRangeError(int index){
    std::stringstream ss;
    ss << "index is out of range: " << index << std::endl;
    return std::runtime_error(ss.str());
}

bool threecpp::FileExists(const std::string& fileName){
    return access(fileName.c_str(), 0) == 0;
}

std::string threecpp::getProgramPath(){
    return std::filesystem::current_path().string();
}





