//
// Created by Willie on 2023/6/17.
//

#ifndef THREE_CPP_SHADER_CHUNK_H
#define THREE_CPP_SHADER_CHUNK_H

#include <map>
#include <string>
#include "shader_chunk/begin_vertex.glsl.h"

std::map<std::string,std::string> ShaderChunk = {
        {"alphamap_fragment",alphamap_fragment},
        {"alphamap_pars_fragment",alphamap_pars_fragment},
        {"alphatest_fragment",alphatest_fragment},
        {"begin_vertex",begin_vertex},
        {"begin_vertex",begin_vertex},
};


#endif //THREE_CPP_SHADER_CHUNK_H
