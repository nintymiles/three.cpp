//
// Created by Willie on 2023/6/17.
//

#ifndef THREE_CPP_SHADER_CHUNK_H
#define THREE_CPP_SHADER_CHUNK_H

#include <map>
#include <string>

#include "shader_chunk/alphamap_fragment.glsl.h"
#include "shader_chunk/alphamap_pars_fragment.glsl.h"
#include "shader_chunk/alphatest_fragment.glsl.h"
#include "shader_chunk/alphatest_pars_fragment.glsl.h"
#include "shader_chunk/aomap_fragment.glsl.h"
#include "shader_chunk/aomap_pars_fragment.glsl.h"
#include "shader_chunk/begin_vertex.glsl.h"
#include "shader_chunk/beginnormal_vertex.glsl.h"

std::map<std::string,std::string> ShaderChunk = {
        {"alphamap_fragment",shader_chunk::alphamap_fragment},
        {"alphamap_pars_fragment",shader_chunk::alphamap_pars_fragment},
        {"alphatest_fragment",shader_chunk::alphatest_fragment},
        {"alphatest_pars_fragment",shader_chunk::alphatest_pars_fragment},
        {"aomap_fragment",shader_chunk::aomap_fragment},
        {"aomap_pars_fragment",shader_chunk::aomap_pars_fragment},
        {"begin_vertex",shader_chunk::begin_vertex},
        {"beginnormal_vertex",shader_chunk::beginnormal_vertex},
};


#endif //THREE_CPP_SHADER_CHUNK_H
