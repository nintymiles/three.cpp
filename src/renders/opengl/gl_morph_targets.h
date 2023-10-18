//
// Created by SeanR on 2023/10/18.
//

#ifndef THREE_CPP_GL_MORPH_TARGETS_H
#define THREE_CPP_GL_MORPH_TARGETS_H

#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdlib>
#include <cmath>

class Object3D;
class BufferGeometry;
class Material;
class GLProgram;

class GLMorphtargets {
public:
    using sptr = std::shared_ptr<GLMorphtargets>;

    std::unordered_map<unsigned, std::vector<std::vector<size_t>>> influencesList;
    std::vector<float> morphInfluences;

    std::vector<std::vector<size_t>> workInfluences;

    GLMorphtargets() {
        influencesList.clear();
        morphInfluences.resize(8);
        for (unsigned int i = 0; i < 8; i++) {
            workInfluences.push_back({i,0 });
        }
    }
    GLMorphtargets(const GLMorphtargets& source) {
        influencesList = source.influencesList;
        morphInfluences = source.morphInfluences;
    }

    GLMorphtargets* clone() {
        return new GLMorphtargets(*this);
    }

    GLMorphtargets& copy(const GLMorphtargets& source) {
        influencesList = source.influencesList;
        morphInfluences = source.morphInfluences;
        return *this;
    }
    virtual ~GLMorphtargets() = default;

    void update(Object3D& object, BufferGeometry& geometry, Material& material, GLProgram& program);

};

inline bool absNumericalSort(std::vector<size_t> a, std::vector<size_t> b) {

    return std::abs((long long)b[1]) - std::abs((long long)a[1]);

}
inline bool numericalSort(std::vector<size_t> a, std::vector<size_t> b) {
    return a[0] - b[0];
}

#endif //THREE_CPP_GL_MORPH_TARGETS_H
