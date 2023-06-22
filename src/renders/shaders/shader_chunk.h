//
// Created by SeanR on 2023/6/17.
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
#include "shader_chunk/bsdfs.glsl.h"
#include "shader_chunk/bumpmap_pars_fragment.glsl.h"
#include "shader_chunk/common.glsl.h"
#include "shader_chunk/clearcoat_normal_fragment_begin.glsl.h"
#include "shader_chunk/clearcoat_normal_fragment_maps.glsl.h"
#include "shader_chunk/clearcoat_pars_fragment.glsl.h"
#include "shader_chunk/clipping_planes_fragment.glsl.h"
#include "shader_chunk/clipping_planes_pars_fragment.glsl.h"
#include "shader_chunk/clipping_planes_pars_vertex.glsl.h"
#include "shader_chunk/clipping_planes_vertex.glsl.h"
#include "shader_chunk/color_fragment.glsl.h"
#include "shader_chunk/color_vertex.glsl.h"
#include "shader_chunk/color_pars_fragment.glsl.h"
#include "shader_chunk/color_pars_vertex.glsl.h"
#include "shader_chunk/cube_uv_reflection_fragment.glsl.h"
#include "shader_chunk/default_fragment.glsl.h"
#include "shader_chunk/default_vertex.glsl.h"
#include "shader_chunk/defaultnormal_vertex.glsl.h"
#include "shader_chunk/displacementmap_pars_vertex.glsl.h"
#include "shader_chunk/displacementmap_vertex.glsl.h"
#include "shader_chunk/dithering_fragment.glsl.h"



std::map<std::string,std::string> ShaderChunk = {
        {"alphamap_fragment",shader_chunk::alphamap_fragment},
        {"alphamap_pars_fragment",shader_chunk::alphamap_pars_fragment},
        {"alphatest_fragment",shader_chunk::alphatest_fragment},
        {"alphatest_pars_fragment",shader_chunk::alphatest_pars_fragment},
        {"aomap_fragment",shader_chunk::aomap_fragment},
        {"aomap_pars_fragment",shader_chunk::aomap_pars_fragment},
        {"begin_vertex",shader_chunk::begin_vertex},
        {"beginnormal_vertex",shader_chunk::beginnormal_vertex},
        {"bsdfs",shader_chunk::bsdfs},
        {"bumpmap_pars_fragment",shader_chunk::bumpmap_pars_fragment},
        {"common",shader_chunk::common},
        {"clearcoat_normal_fragment_begin",shader_chunk::clearcoat_normal_fragment_begin},
        {"clearcoat_normal_fragment_maps",shader_chunk::clearcoat_normal_fragment_maps},
        {"clearcoat_pars_fragment",shader_chunk::clearcoat_pars_fragment},
        {"clipping_planes_fragment",shader_chunk::clipping_planes_fragment},
        {"clipping_planes_pars_fragment",shader_chunk::clipping_planes_pars_fragment},
        {"clipping_planes_pars_vertex",shader_chunk::clipping_planes_pars_vertex},
        {"clipping_planes_vertex",shader_chunk::clipping_planes_vertex},
        {"color_fragment",shader_chunk::color_fragment},
        {"color_vertex",shader_chunk::color_vertex},
        {"color_pars_fragment",shader_chunk::color_pars_fragment},
        {"color_pars_vertex",shader_chunk::color_pars_vertex},
        {"cube_uv_reflection_fragment",shader_chunk::cube_uv_reflection_fragment},
        {"default_fragment",shader_chunk::default_fragment},
        {"default_vertex",shader_chunk::default_vertex},
        {"defaultnormal_vertex",shader_chunk::defaultnormal_vertex},
        {"displacementmap_pars_vertex",shader_chunk::displacementmap_pars_vertex},
        {"displacementmap_vertex",shader_chunk::displacementmap_vertex},
        {"dithering_fragment",shader_chunk::dithering_fragment},
        {"common",shader_chunk::common},
        {"common",shader_chunk::common},
        {"common",shader_chunk::common},
        {"common",shader_chunk::common},
};


#endif //THREE_CPP_SHADER_CHUNK_H
