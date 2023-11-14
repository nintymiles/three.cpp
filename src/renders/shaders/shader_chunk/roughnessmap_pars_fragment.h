//
// Created by SeanR on 2023/11/14.
//

#ifndef THREE_CPP_ROUGHNESSMAP_PARS_FRAGMENT_H
#define THREE_CPP_ROUGHNESSMAP_PARS_FRAGMENT_H

namespace shader_chunk {

const char* roughnessmap_pars_fragment =R"(
#ifdef USE_ROUGHNESSMAP

	uniform sampler2D roughnessMap;

#endif
)";

}

#endif //THREE_CPP_ROUGHNESSMAP_PARS_FRAGMENT_H
