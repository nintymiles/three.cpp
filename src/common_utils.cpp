#include "common_utils.h"

#include <filesystem>
#include <chrono>


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

size_t threecpp::getSystemTimeInMillis() {
    std::chrono::time_point<std::chrono::steady_clock> time = std::chrono::steady_clock::now();
    auto timeInMillis = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();
    return timeInMillis;
}





