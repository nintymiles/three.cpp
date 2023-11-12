//
// Created by SeanR on 2022/12/13.
//

#ifndef THREE_CPP_SRC_TEXTURES_SOURCE_H
#define THREE_CPP_SRC_TEXTURES_SOURCE_H

#include "math_utils.h"

#include <string>


struct Source {
    std::string uuid = math_utils::generate_uuid();
    int version = 0;
    uint8_t *data;

    Source( uint8_t* data = nullptr ):data(data) {}

    Source &needsUpdate(bool value) {
        if (value == true)
            this->version++;

        return *this;
    }
};

#endif //THREE_CPP_SRC_TEXTURES_SOURCE_H
