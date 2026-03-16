//
// Created by SeanR on 05/03/2026.
//

#ifndef THREE_CPP_GL_ENV_MAPS_H
#define THREE_CPP_GL_ENV_MAPS_H

#include "texture.h"

class GLEnvMaps {

public:
    using sptr = std::shared_ptr<GLEnvMaps>;

    virtual Texture::sptr get(const Texture::sptr texture) = 0;

};

#endif //THREE_CPP_GL_ENV_MAPS_H
