//
// Created by SeanR on 2023/10/22.
//
#include "shader_chunk.h"

#include <unordered_map>
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
#include "shader_chunk/dithering_pars_fragment.glsl.h"
#include "shader_chunk/emissivemap_fragment.glsl.h"
#include "shader_chunk/emissivemap_pars_fragment.glsl.h"
#include "shader_chunk/encodings_fragment.glsl.h"
#include "shader_chunk/encodings_pars_fragment.glsl.h"
#include "shader_chunk/envmap_fragment.glsl.h"
#include "shader_chunk/envmap_common_pars_fragment.glsl.h"
#include "shader_chunk/envmap_pars_fragment.glsl.h"
#include "shader_chunk/envmap_pars_vertex.glsl.h"
#include "shader_chunk/envmap_physical_pars_fragment.glsl.h"
#include "shader_chunk/envmap_vertex.glsl.h"
#include "shader_chunk/fog_fragment.glsl.h"
#include "shader_chunk/fog_vertex.glsl.h"
#include "shader_chunk/fog_pars_fragment.glsl.h"
#include "shader_chunk/fog_pars_vertex.glsl.h"
#include "shader_chunk/gradientmap_pars_fragment.glsl.h"
#include "shader_chunk/iridescence_fragment.glsl.h"
#include "shader_chunk/iridescence_pars_fragment.glsl.h"
#include "shader_chunk/lightmap_fragment.glsl.h"
#include "shader_chunk/lightmap_pars_fragment.glsl.h"
#include "shader_chunk/lights_fragment_begin.glsl.h"
#include "shader_chunk/lights_fragment_end.glsl.h"
#include "shader_chunk/lights_fragment_maps.glsl.h"
#include "shader_chunk/lights_toon_fragment.glsl.h"
#include "shader_chunk/lights_toon_pars_fragment.glsl.h"
#include "shader_chunk/lights_lambert_vertex.glsl.h"
#include "shader_chunk/lights_pars_begin.glsl.h"
#include "shader_chunk/lights_phong_fragment.glsl.h"
#include "shader_chunk/lights_phong_pars_fragment.glsl.h"
#include "shader_chunk/lights_physical_fragment.glsl.h"
#include "shader_chunk/lights_physical_pars_fragment.glsl.h"
#include "shader_chunk/logdepthbuf_fragment.glsl.h"
#include "shader_chunk/logdepthbuf_pars_fragment.glsl.h"
#include "shader_chunk/logdepthbuf_pars_vertex.glsl.h"
#include "shader_chunk/logdepthbuf_vertex.glsl.h"
#include "shader_chunk/uv_pars_vertex.glsl.h"
#include "shader_chunk/uv_pars_fragment.glsl.h"
#include "shader_chunk/uv_vertex.glsl.h"
#include "shader_chunk/uv2_vertex.glsl.h"
#include "shader_chunk/uv2_pars_fragment.glsl.h"
#include "shader_chunk/uv2_pars_vertex.glsl.h"
#include "shader_chunk/morphnormal_vertex.glsl.h"
#include "shader_chunk/morphtarget_vertex.glsl.h"
#include "shader_chunk/morphtarget_pars_vertex.glsl.h"
#include "shader_chunk/map_fragment.glsl.h"
#include "shader_chunk/map_pars_fragment.glsl.h"
#include "shader_chunk/map_particle_fragment.glsl.h"
#include "shader_chunk/map_particle_pars_fragment.glsl.h"
#include "shader_chunk/metalnessmap_fragment.glsl.h"
#include "shader_chunk/metalnessmap_pars_fragment.glsl.h"
#include "shader_chunk/normal_fragment_begin.glsl.h"
#include "shader_chunk/normal_fragment_maps.glsl.h"
#include "shader_chunk/normalmap_pars_fragment.h"
#include "shader_chunk/packing.glsl.h"
#include "shader_chunk/project_vertex.glsl.h"
#include "shader_chunk/premultiplied_alpha_fragment.glsl.h"
#include "shader_chunk/skinbase_vertex.glsl.h"
#include "shader_chunk/skinning_pars_vertex.glsl.h"
#include "shader_chunk/skinning_vertex.glsl.h"
#include "shader_chunk/skinnormal_vertex.glsl.h"
#include "shader_chunk/shadowmap_pars_fragment.glsl.h"
#include "shader_chunk/shadowmap_pars_vertex.glsl.h"
#include "shader_chunk/shadowmap_vertex.glsl.h"
#include "shader_chunk/shadowmask_pars_fragment.glsl.h"
#include "shader_chunk/specularmap_fragment.glsl.h"
#include "shader_chunk/specularmap_pars_fragment.glsl.h"
#include "shader_chunk/roughnessmap_fragment.h"
#include "shader_chunk/roughnessmap_pars_fragment.h"
#include "shader_chunk/tonemapping_fragment.glsl.h"
#include "shader_chunk/tonemapping_pars_fragment.glsl.h"
#include "shader_chunk/transmissionmap_fragment.glsl.h"
#include "shader_chunk/transmissionmap_pars_fragment.glsl.h"
#include "shader_chunk/worldpos_vertex.glsl.h"

#include "shader_lib/meshbasic_vert.h"
#include "shader_lib/meshbasic_frag.h"
#include "shader_lib/meshlambert_vert.h"
#include "shader_lib/meshlambert_frag.h"
#include "shader_lib/meshphong_vert.h"
#include "shader_lib/meshphong_frag.h"
#include "shader_lib/meshphysical_vert.h"
#include "shader_lib/meshphysical_frag.h"
#include "shader_lib/meshtoon_vert.h"
#include "shader_lib/meshtoon_frag.h"
#include "shader_lib/meshmatcap_vert.h"
#include "shader_lib/meshmatcap_frag.h"
#include "shader_lib/points_vert.h"
#include "shader_lib/points_frag.h"
#include "shader_lib/linedashed_vert.h"
#include "shader_lib/linedashed_frag.h"
#include "shader_lib/depth_vert.h"
#include "shader_lib/depth_frag.h"
#include "shader_lib/normal_vert.h"
#include "shader_lib/normal_frag.h"
#include "shader_lib/sprite_vert.h"
#include "shader_lib/sprite_frag.h"
#include "shader_lib/background_vert.h"
#include "shader_lib/background_frag.h"
#include "shader_lib/cube_vert.h"
#include "shader_lib/cube_frag.h"
#include "shader_lib/equirect_vert.h"
#include "shader_lib/equirect_frag.h"
#include "shader_lib/distanceRGBA_vert.h"
#include "shader_lib/distanceRGBA_frag.h"
#include "shader_lib/shadow_vert.h"
#include "shader_lib/shadow_frag.h"
#include "shader_lib/vsm_vert.h"
#include "shader_lib/vsm_frag.h"


static const std::unordered_map<std::string,ShaderLibID> ShaderLibMap = {
        {"alphamap_fragment",ShaderLibID::alphamap_fragment},
        {"alphamap_pars_fragment",ShaderLibID::alphamap_pars_fragment},
        {"alphatest_fragment",ShaderLibID::alphatest_fragment},
        {"alphatest_pars_fragment",ShaderLibID::alphatest_pars_fragment},
        {"aomap_fragment",ShaderLibID::aomap_fragment},
        {"aomap_pars_fragment",ShaderLibID::aomap_pars_fragment},
        {"begin_vertex",ShaderLibID::begin_vertex},
        {"beginnormal_vertex",ShaderLibID::beginnormal_vertex},
        {"bsdfs",ShaderLibID::bsdfs},
        {"bumpmap_pars_fragment",ShaderLibID::bumpmap_pars_fragment},
        {"common",ShaderLibID::common},
        {"clearcoat_normal_fragment_begin",ShaderLibID::clearcoat_normal_fragment_begin},
        {"clearcoat_normal_fragment_maps",ShaderLibID::clearcoat_normal_fragment_maps},
        {"clearcoat_pars_fragment",ShaderLibID::clearcoat_pars_fragment},
        {"clipping_planes_fragment",ShaderLibID::clipping_planes_fragment},
        {"clipping_planes_pars_fragment",ShaderLibID::clipping_planes_pars_fragment},
        {"clipping_planes_pars_vertex",ShaderLibID::clipping_planes_pars_vertex},
        {"clipping_planes_vertex",ShaderLibID::clipping_planes_vertex},
        {"color_fragment",ShaderLibID::color_fragment},
        {"color_vertex",ShaderLibID::color_vertex},
        {"color_pars_fragment",ShaderLibID::color_pars_fragment},
        {"color_pars_vertex",ShaderLibID::color_pars_vertex},
        {"cube_uv_reflection_fragment",ShaderLibID::cube_uv_reflection_fragment},
        {"default_fragment",ShaderLibID::default_fragment},
        {"default_vertex",ShaderLibID::default_vertex},
        {"defaultnormal_vertex",ShaderLibID::defaultnormal_vertex},
        {"displacementmap_pars_vertex",ShaderLibID::displacementmap_pars_vertex},
        {"displacementmap_vertex",ShaderLibID::displacementmap_vertex},
        {"dithering_fragment",ShaderLibID::dithering_fragment},
        {"dithering_pars_fragment",ShaderLibID::dithering_pars_fragment},
        {"emissivemap_fragment",ShaderLibID::emissivemap_fragment},
        {"emissivemap_pars_fragment",ShaderLibID::emissivemap_pars_fragment},
        {"encodings_fragment",ShaderLibID::encodings_fragment},
        {"encodings_pars_fragment",ShaderLibID::encodings_pars_fragment},
        {"envmap_common_pars_fragment",ShaderLibID::envmap_common_pars_fragment},
        {"envmap_fragment",ShaderLibID::envmap_fragment},
        {"envmap_pars_fragment",ShaderLibID::envmap_pars_fragment},
        {"envmap_pars_vertex",ShaderLibID::envmap_pars_vertex},
        {"envmap_physical_pars_fragment",ShaderLibID::envmap_physical_pars_fragment},
        {"envmap_vertex",ShaderLibID::envmap_vertex},
        {"fog_fragment",ShaderLibID::fog_fragment},
        {"fog_vertex",ShaderLibID::fog_vertex},
        {"fog_pars_fragment",ShaderLibID::fog_pars_fragment},
        {"fog_pars_vertex",ShaderLibID::fog_pars_vertex},
        {"gradientmap_pars_fragment",ShaderLibID::gradientmap_pars_fragment},
        {"iridescence_fragment",ShaderLibID::iridescence_fragment},
        {"iridescence_pars_fragment",ShaderLibID::iridescence_pars_fragment},
        {"lightmap_fragment",ShaderLibID::lightmap_fragment},
        {"lightmap_pars_fragment",ShaderLibID::lightmap_pars_fragment},
        {"lights_fragment_begin",ShaderLibID::lights_fragment_begin},
        {"lights_fragment_end",ShaderLibID::lights_fragment_end},
        {"lights_fragment_maps",ShaderLibID::lights_fragment_maps},
        {"lights_toon_fragment",ShaderLibID::lights_toon_fragment},
        {"lights_toon_pars_fragment",ShaderLibID::lights_toon_pars_fragment},
        {"lights_lambert_vertex",ShaderLibID::lights_lambert_vertex},
        {"lights_pars_begin",ShaderLibID::lights_pars_begin},
        {"lights_phong_fragment",ShaderLibID::lights_phong_fragment},
        {"lights_phong_pars_fragment",ShaderLibID::lights_phong_pars_fragment},
        {"lights_physical_fragment",ShaderLibID::lights_physical_fragment},
        {"lights_physical_pars_fragment",ShaderLibID::lights_physical_pars_fragment},
        {"normal_fragment_begin",ShaderLibID::normal_fragment_begin},
        {"normal_fragment_maps",ShaderLibID::normal_fragment_maps},
        {"normalmap_pars_fragment",ShaderLibID::normalmap_pars_fragment},
        {"logdepthbuf_fragment",ShaderLibID::logdepthbuf_fragment},
        {"logdepthbuf_pars_fragment",ShaderLibID::logdepthbuf_pars_fragment},
        {"logdepthbuf_pars_vertex",ShaderLibID::logdepthbuf_pars_vertex},
        {"logdepthbuf_vertex",ShaderLibID::logdepthbuf_vertex},
        {"fog_fragment",ShaderLibID::fog_fragment},
        {"fog_pars_fragment",ShaderLibID::fog_pars_fragment},
        {"uv_pars_vertex",ShaderLibID::uv_pars_vertex},
        {"uv_pars_fragment",ShaderLibID::uv_pars_fragment},
        {"uv_vertex",ShaderLibID::uv_vertex},
        {"uv2_pars_fragment",ShaderLibID::uv2_pars_fragment},
        {"uv2_pars_vertex",ShaderLibID::uv2_pars_vertex},
        {"uv2_vertex",ShaderLibID::uv2_vertex},
        {"morphnormal_vertex",ShaderLibID::morphnormal_vertex},
        {"morphtarget_vertex",ShaderLibID::morphtarget_vertex},
        {"morphtarget_pars_vertex",ShaderLibID::morphtarget_pars_vertex},
        {"map_fragment",ShaderLibID::map_fragment},
        {"map_pars_fragment",ShaderLibID::map_pars_fragment},
        {"map_particle_fragment",ShaderLibID::map_particle_fragment},
        {"map_particle_pars_fragment",ShaderLibID::map_particle_pars_fragment},
        {"metalnessmap_fragment",ShaderLibID::metalnessmap_fragment},
        {"metalnessmap_pars_fragment",ShaderLibID::metalnessmap_pars_fragment},
        {"project_vertex",ShaderLibID::project_vertex},
        {"packing",ShaderLibID::packing},
        {"premultiplied_alpha_fragment",ShaderLibID::premultiplied_alpha_fragment},
        {"skinbase_vertex",ShaderLibID::skinbase_vertex},
        {"skinning_pars_vertex",ShaderLibID::skinning_pars_vertex},
        {"skinning_vertex",ShaderLibID::skinning_vertex},
        {"skinnormal_vertex",ShaderLibID::skinnormal_vertex},
        {"specularmap_fragment",ShaderLibID::specularmap_fragment},
        {"specularmap_pars_fragment",ShaderLibID::specularmap_pars_fragment},
        {"shadowmap_pars_fragment",ShaderLibID::shadowmap_pars_fragment},
        {"shadowmap_pars_vertex",ShaderLibID::shadowmap_pars_vertex},
        {"shadowmap_vertex",ShaderLibID::shadowmap_vertex},
        {"shadowmask_pars_fragment",ShaderLibID::shadowmask_pars_fragment},
        {"roughnessmap_fragment",ShaderLibID::roughnessmap_fragment},
        {"roughnessmap_pars_fragment",ShaderLibID::roughnessmap_pars_fragment},
        {"tonemapping_fragment",ShaderLibID::tonemapping_fragment},
        {"tonemapping_pars_fragment",ShaderLibID::tonemapping_pars_fragment},
        {"transmissionmap_fragment",ShaderLibID::transmissionmap_fragment},
        {"transmissionmap_pars_fragment",ShaderLibID::transmissionmap_pars_fragment},
        {"worldpos_vertex",ShaderLibID::worldpos_vertex},

        {"meshbasic_vert",ShaderLibID::meshbasic_vert},
        {"meshbasic_frag",ShaderLibID::meshbasic_frag},
        {"meshlambert_vert",ShaderLibID::meshlambert_vert},
        {"meshlambert_frag",ShaderLibID::meshlambert_frag},
        {"meshphong_vert",ShaderLibID::meshphong_vert},
        {"meshphong_frag",ShaderLibID::meshphong_frag},
        {"meshphysical_vert",ShaderLibID::meshphysical_vert},
        {"meshphysical_frag",ShaderLibID::meshphysical_frag},
        {"meshtoon_vert",ShaderLibID::meshtoon_vert},
        {"meshtoon_frag",ShaderLibID::meshtoon_frag},
        {"meshmatcap_vert",ShaderLibID::meshmatcap_vert},
        {"meshmatcap_frag",ShaderLibID::meshmatcap_frag},
        {"points_vert",ShaderLibID::points_vert},
        {"points_vert",ShaderLibID::points_frag},
        {"linedashed_vert",ShaderLibID::linedashed_vert},
        {"linedashed_frag",ShaderLibID::linedashed_frag},
        {"depth_vert",ShaderLibID::depth_vert},
        {"depth_frag",ShaderLibID::depth_frag},
        {"normal_vert",ShaderLibID::normal_vert},
        {"normal_frag",ShaderLibID::normal_frag},
        {"sprite_vert",ShaderLibID::sprite_vert},
        {"sprite_frag",ShaderLibID::sprite_frag},
        {"background_vert",ShaderLibID::background_vert},
        {"background_frag",ShaderLibID::background_frag},
        {"cube_vert",ShaderLibID::cube_vert},
        {"cube_frag",ShaderLibID::cube_frag},
        {"equirect_vert",ShaderLibID::equirect_vert},
        {"equirect_frag",ShaderLibID::equirect_frag},
        {"distanceRGBA_vert",ShaderLibID::distanceRGBA_vert},
        {"distanceRGBA_frag",ShaderLibID::distanceRGBA_frag},
        {"shadow_vert",ShaderLibID::shadow_vert},
        {"shadow_frag",ShaderLibID::shadow_frag},
        {"vsm_vert",ShaderLibID::vsm_vert},
        {"vsm_frag",ShaderLibID::vsm_frag},
};

static const std::unordered_map<ShaderLibID,const char*> ShaderChunkMap = {
        {ShaderLibID::alphamap_fragment,shader_chunk::alphamap_fragment},
        {ShaderLibID::alphamap_pars_fragment,shader_chunk::alphamap_pars_fragment},
        {ShaderLibID::alphatest_fragment,shader_chunk::alphatest_fragment},
        {ShaderLibID::alphatest_pars_fragment,shader_chunk::alphatest_pars_fragment},
        {ShaderLibID::aomap_fragment,shader_chunk::aomap_fragment},
        {ShaderLibID::aomap_pars_fragment,shader_chunk::aomap_pars_fragment},
        {ShaderLibID::begin_vertex,shader_chunk::begin_vertex},
        {ShaderLibID::beginnormal_vertex,shader_chunk::beginnormal_vertex},
        {ShaderLibID::bsdfs,shader_chunk::bsdfs},
        {ShaderLibID::bumpmap_pars_fragment,shader_chunk::bumpmap_pars_fragment},
        {ShaderLibID::common,shader_chunk::common},
        {ShaderLibID::clearcoat_normal_fragment_begin,shader_chunk::clearcoat_normal_fragment_begin},
        {ShaderLibID::clearcoat_normal_fragment_maps,shader_chunk::clearcoat_normal_fragment_maps},
        {ShaderLibID::clearcoat_pars_fragment,shader_chunk::clearcoat_pars_fragment},
        {ShaderLibID::clipping_planes_fragment,shader_chunk::clipping_planes_fragment},
        {ShaderLibID::clipping_planes_pars_fragment,shader_chunk::clipping_planes_pars_fragment},
        {ShaderLibID::clipping_planes_pars_vertex,shader_chunk::clipping_planes_pars_vertex},
        {ShaderLibID::clipping_planes_vertex,shader_chunk::clipping_planes_vertex},
        {ShaderLibID::color_fragment,shader_chunk::color_fragment},
        {ShaderLibID::color_vertex,shader_chunk::color_vertex},
        {ShaderLibID::color_pars_fragment,shader_chunk::color_pars_fragment},
        {ShaderLibID::color_pars_vertex,shader_chunk::color_pars_vertex},
        {ShaderLibID::cube_uv_reflection_fragment,shader_chunk::cube_uv_reflection_fragment},
        {ShaderLibID::default_fragment,shader_chunk::default_fragment},
        {ShaderLibID::default_vertex,shader_chunk::default_vertex},
        {ShaderLibID::defaultnormal_vertex,shader_chunk::defaultnormal_vertex},
        {ShaderLibID::displacementmap_pars_vertex,shader_chunk::displacementmap_pars_vertex},
        {ShaderLibID::displacementmap_vertex,shader_chunk::displacementmap_vertex},
        {ShaderLibID::dithering_fragment,shader_chunk::dithering_fragment},
        {ShaderLibID::dithering_pars_fragment,shader_chunk::dithering_pars_fragment},
        {ShaderLibID::emissivemap_fragment,shader_chunk::emissivemap_fragment},
        {ShaderLibID::emissivemap_pars_fragment,shader_chunk::emissivemap_pars_fragment},
        {ShaderLibID::encodings_fragment,shader_chunk::encodings_fragment},
        {ShaderLibID::encodings_pars_fragment,shader_chunk::encodings_pars_fragment},
        {ShaderLibID::envmap_common_pars_fragment,shader_chunk::envmap_common_pars_fragment},
        {ShaderLibID::envmap_fragment,shader_chunk::envmap_fragment},
        {ShaderLibID::envmap_pars_fragment,shader_chunk::envmap_pars_fragment},
        {ShaderLibID::envmap_pars_vertex,shader_chunk::envmap_pars_vertex},
        {ShaderLibID::envmap_physical_pars_fragment,shader_chunk::envmap_physical_pars_fragment},
        {ShaderLibID::envmap_vertex,shader_chunk::envmap_vertex},
        {ShaderLibID::fog_fragment,shader_chunk::fog_fragment},
        {ShaderLibID::fog_vertex,shader_chunk::fog_vertex},
        {ShaderLibID::fog_pars_fragment,shader_chunk::fog_pars_fragment},
        {ShaderLibID::fog_pars_vertex,shader_chunk::fog_pars_vertex},
        {ShaderLibID::gradientmap_pars_fragment,shader_chunk::gradientmap_pars_fragment},
        {ShaderLibID::iridescence_fragment,shader_chunk::iridescence_fragment},
        {ShaderLibID::iridescence_pars_fragment,shader_chunk::iridescence_pars_fragment},
        {ShaderLibID::lightmap_fragment,shader_chunk::lightmap_fragment},
        {ShaderLibID::lightmap_pars_fragment,shader_chunk::lightmap_pars_fragment},
        {ShaderLibID::lights_fragment_begin,shader_chunk::lights_fragment_begin},
        {ShaderLibID::lights_fragment_end,shader_chunk::lights_fragment_end},
        {ShaderLibID::lights_fragment_maps,shader_chunk::lights_fragment_maps},
        {ShaderLibID::lights_toon_fragment,shader_chunk::lights_toon_fragment},
        {ShaderLibID::lights_toon_pars_fragment,shader_chunk::lights_toon_pars_fragment},
        {ShaderLibID::lights_lambert_vertex,shader_chunk::lights_lambert_vertex},
        {ShaderLibID::lights_pars_begin,shader_chunk::lights_pars_begin},
        {ShaderLibID::lights_phong_fragment,shader_chunk::lights_phong_fragment},
        {ShaderLibID::lights_phong_pars_fragment,shader_chunk::lights_phong_pars_fragment},
        {ShaderLibID::lights_physical_fragment,shader_chunk::lights_physical_fragment},
        {ShaderLibID::lights_physical_pars_fragment,shader_chunk::lights_physical_pars_fragment},
        {ShaderLibID::logdepthbuf_vertex,shader_chunk::logdepthbuf_vertex},
        {ShaderLibID::logdepthbuf_pars_vertex,shader_chunk::logdepthbuf_pars_vertex},
        {ShaderLibID::logdepthbuf_pars_fragment,shader_chunk::logdepthbuf_pars_fragment},
        {ShaderLibID::logdepthbuf_fragment,shader_chunk::logdepthbuf_fragment},
        {ShaderLibID::fog_fragment,shader_chunk::fog_fragment},
        {ShaderLibID::fog_pars_fragment,shader_chunk::fog_pars_fragment},
        {ShaderLibID::uv_pars_vertex,shader_chunk::uv_pars_vertex},
        {ShaderLibID::uv_pars_fragment,shader_chunk::uv_pars_fragment},
        {ShaderLibID::uv_vertex,shader_chunk::uv_vertex},
        {ShaderLibID::uv2_vertex,shader_chunk::uv2_vertex},
        {ShaderLibID::uv2_pars_vertex,shader_chunk::uv2_pars_vertex},
        {ShaderLibID::uv2_pars_fragment,shader_chunk::uv2_pars_fragment},
        {ShaderLibID::morphnormal_vertex,shader_chunk::morphnormal_vertex},
        {ShaderLibID::morphtarget_vertex,shader_chunk::morphtarget_vertex},
        {ShaderLibID::morphtarget_pars_vertex,shader_chunk::morphtarget_pars_vertex},
        {ShaderLibID::map_particle_pars_fragment,shader_chunk::map_particle_pars_fragment},
        {ShaderLibID::map_particle_fragment,shader_chunk::map_particle_fragment},
        {ShaderLibID::map_pars_fragment,shader_chunk::map_pars_fragment},
        {ShaderLibID::map_fragment,shader_chunk::map_fragment},
        {ShaderLibID::metalnessmap_pars_fragment,shader_chunk::metalnessmap_pars_fragment},
        {ShaderLibID::metalnessmap_fragment,shader_chunk::metalnessmap_fragment},
        {ShaderLibID::normal_fragment_begin,shader_chunk::normal_fragment_begin},
        {ShaderLibID::normal_fragment_maps,shader_chunk::normal_fragment_maps},
        {ShaderLibID::normalmap_pars_fragment,shader_chunk::normalmap_pars_fragment},
        {ShaderLibID::packing,shader_chunk::packing},
        {ShaderLibID::project_vertex,shader_chunk::project_vertex},
        {ShaderLibID::premultiplied_alpha_fragment,shader_chunk::premultiplied_alpha_fragment},
        {ShaderLibID::skinnormal_vertex,shader_chunk::skinnormal_vertex},
        {ShaderLibID::skinning_vertex,shader_chunk::skinning_vertex},
        {ShaderLibID::skinning_pars_vertex,shader_chunk::skinning_pars_vertex},
        {ShaderLibID::skinbase_vertex,shader_chunk::skinbase_vertex},
        {ShaderLibID::specularmap_pars_fragment,shader_chunk::specularmap_pars_fragment},
        {ShaderLibID::specularmap_fragment,shader_chunk::specularmap_fragment},
        {ShaderLibID::shadowmask_pars_fragment,shader_chunk::shadowmask_pars_fragment},
        {ShaderLibID::shadowmap_vertex,shader_chunk::shadowmap_vertex},
        {ShaderLibID::shadowmap_pars_vertex,shader_chunk::shadowmap_pars_vertex},
        {ShaderLibID::shadowmap_pars_fragment,shader_chunk::shadowmap_pars_fragment},
        {ShaderLibID::roughnessmap_fragment,shader_chunk::roughnessmap_fragment},
        {ShaderLibID::roughnessmap_pars_fragment,shader_chunk::roughnessmap_pars_fragment},
        {ShaderLibID::tonemapping_fragment,shader_chunk::tonemapping_fragment},
        {ShaderLibID::tonemapping_pars_fragment,shader_chunk::tonemapping_pars_fragment},
        {ShaderLibID::transmissionmap_fragment,shader_chunk::transmissionmap_fragment},
        {ShaderLibID::transmissionmap_pars_fragment,shader_chunk::transmissionmap_pars_fragment},
        {ShaderLibID::worldpos_vertex,shader_chunk::worldpos_vertex},

        {ShaderLibID::meshbasic_vert,meshbasic_vert},
        {ShaderLibID::meshbasic_frag,meshbasic_frag},
        {ShaderLibID::meshlambert_vert,meshlambert_vert},
        {ShaderLibID::meshlambert_frag,meshlambert_frag},
        {ShaderLibID::meshphong_vert,meshphong_vert},
        {ShaderLibID::meshphong_frag,meshphong_frag},
        {ShaderLibID::meshphysical_vert,meshphysical_vert},
        {ShaderLibID::meshphysical_frag,meshphysical_frag},
        {ShaderLibID::meshtoon_vert,meshtoon_vert},
        {ShaderLibID::meshtoon_frag,meshtoon_frag},
        {ShaderLibID::meshmatcap_vert,meshmatcap_vert},
        {ShaderLibID::meshmatcap_frag, meshmatcap_frag},
        {ShaderLibID::points_vert,points_vert},
        {ShaderLibID::points_frag, points_frag},
        {ShaderLibID::linedashed_vert,linedashed_vert},
        {ShaderLibID::linedashed_frag, linedashed_frag},
        {ShaderLibID::depth_vert,depth_vert},
        {ShaderLibID::depth_frag, depth_frag},
        {ShaderLibID::normal_vert,normal_vert},
        {ShaderLibID::normal_frag, normal_frag},
        {ShaderLibID::sprite_vert,sprite_vert},
        {ShaderLibID::sprite_frag, sprite_frag},
        {ShaderLibID::background_vert,background_vert},
        {ShaderLibID::background_frag, background_frag},
        {ShaderLibID::cube_vert,cube_vert},
        {ShaderLibID::cube_frag, cube_frag},
        {ShaderLibID::equirect_vert,equirect_vert},
        {ShaderLibID::equirect_frag, equirect_frag},
        {ShaderLibID::distanceRGBA_vert,distanceRGBA_vert},
        {ShaderLibID::distanceRGBA_frag, distanceRGBA_frag},
        {ShaderLibID::shadow_vert,shadow_vert},
        {ShaderLibID::shadow_frag, shadow_frag},
        {ShaderLibID::vsm_vert,vsm_vert},
        {ShaderLibID::vsm_frag, vsm_frag},

};

namespace shaderlib_name {
    ShaderLibID get(std::string name){
        auto found = ShaderLibMap.find(name);
        if (found != ShaderLibMap.end()) return found->second;
        else return ShaderLibID::unknown;
    }
}

const char* getShaderChunk(ShaderLibID shaderLibId){
    return ShaderChunkMap.at(shaderLibId);
};
