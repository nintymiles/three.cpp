//
// Created by SeanR on 2023/10/15.
//

#ifndef THREE_CPP_DIRECT_GEOMETRY_H
#define THREE_CPP_DIRECT_GEOMETRY_H

#include "geometry.h"
#include "constants.h"
#include "common_types.h"

#include <memory>


class DirectGeometry : public Geometry {
public:
    using sptr = std::shared_ptr<DirectGeometry>;

    std::vector<unsigned> indices;

    std::vector<threecpp::DrawRange> groups;

    //ThreeDataTable morphPositionNormal;
    std::unordered_map<std::string, std::vector<MorphTarget>> morphPositionNormal;

    DirectGeometry() :Geometry() {}

    using ptr = std::shared_ptr<DirectGeometry>;

    void computeGroups(const Geometry& geometry);

    DirectGeometry& fromGeometry(Geometry& geometry);

};

#endif //THREE_CPP_DIRECT_GEOMETRY_H
