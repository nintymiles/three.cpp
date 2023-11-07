#include "obj_loader.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "buffer_geometry.h"
#include "material.h"
#include "mesh.h"
#include "line.h"
#include "points.h"
#include "mesh_phong_material.h"
#include "line_basic_material.h"
#include "points_material.h"
#include "string_utils.h"
#include "timer.h"
#include "common_utils.h"

#include <unordered_set>

namespace objloader {
    Object::ObjectMaterial::ptr emptyMaterial = nullptr;
    static const int OBJ_SIZE = 1000 * 1000 * 1000 * 6;
}
using namespace objloader;
using namespace std;
using namespace string_utils;

Object::ObjectMaterial::ptr Object::ObjectMaterial::clone(int _index){
    auto cloned = std::make_shared<Object::ObjectMaterial>();
    cloned->name = name;
    cloned->index = _index;
    cloned->smooth = smooth;

    cloned->objMtlLib = objMtlLib;

    return cloned;
}

Object::ObjectMaterial::ptr& Object::currentMaterial(){
    if (materials.size() > 0)
        return materials[materials.size() - 1];
    return emptyMaterial;

}

Object::ObjectMaterial::ptr Object::startMaterial(const string& name, vector<string>& libraries){
    auto previous = finalize(false);

    if (previous!=nullptr && (previous->inherited || previous->groupCount <= 0)){
        materials.erase(materials.begin() + previous->index);
    }

    Object::ObjectMaterial::ptr material = make_shared<Object::ObjectMaterial>();

    material->index = materials.size();
    material->name = name;
    //material->stringLib = libraries[libraries.size() - 1];
    material->smooth = previous!=nullptr ? previous->smooth : smooth;
    material->groupStart = previous!=nullptr ? previous->groupEnd : 0;
    material->groupEnd = -1;
    material->groupCount = -1;
    material->inherited = false;

    materials.push_back(material);

    return material;
}

Object::ObjectMaterial::ptr Object::startMaterial(const string& name, std::unordered_map<string,size_t>& materialCount){
    auto previous = finalize(false);

    if (previous!=nullptr && (previous->inherited || previous->groupCount <= 0)){
        materials.erase(materials.begin() + previous->index);
    }

    Object::ObjectMaterial::ptr material = make_shared<Object::ObjectMaterial>();

    material->index = materials.size();
    material->name = name;
    //material->stringLib = libraries[libraries.size() - 1];
    material->smooth = previous!=nullptr ? previous->smooth : smooth;
    material->groupStart = previous!=nullptr ? previous->groupEnd : 0;
    material->groupEnd = materialCount[name] + material->groupStart;
    material->groupCount = materialCount[name];
    material->inherited = false;

    materials.push_back(material);

    return material;
}



Object::ObjectMaterial::ptr& Object::finalize(bool end){
    ObjectMaterial::ptr& lastMultiMaterial = this->currentMaterial();
    if (lastMultiMaterial!=nullptr && lastMultiMaterial->groupEnd == -1) {
//        lastMultiMaterial->groupEnd = geometry.vertices.size() / 3;
//        lastMultiMaterial->groupCount = lastMultiMaterial->groupEnd - lastMultiMaterial->groupStart;
        lastMultiMaterial->inherited = false;
    }

//    // Ignore objects tail materials if no face declarations followed them before a new o/g started.
//    if (end && materials.size() > 1) {
//
//        for (int mi = (int)materials.size() - 1; mi >= 0; mi--) {
//
//            if (materials[mi]->groupCount <= 0) {
//
//                materials.erase(materials.begin() + mi);
//
//            }
//        }
//    }

    // Guarantee at least one empty material, this makes the creation later more straight forward.
    if (end && materials.size() == 0) {

        Object::ObjectMaterial::ptr material = make_shared<Object::ObjectMaterial>();
        material->name = "";
        material->smooth = smooth;
        materials.push_back(material);
    }

    return lastMultiMaterial;
}

int ObjectState::parseVertexIndex(int value, size_t len){
    int index = value;

    return (index >= 0 ? index - 1 : index + len / 3) * 3;
}

int ObjectState::parseNormalIndex(int value, size_t len){
    int index = value;
    return (index >= 0 ? index - 1 : index + len / 3) * 3;
}

int ObjectState::parseUVIndex(int value, size_t len){
    int index = value;

    return (index >= 0 ? index - 1 : index + len / 2) * 2;
}

void addVector3(vector<float>& target, float x, float y, float z) {
    target.push_back(x);
    target.push_back(y);
    target.push_back(z);
}

void addVector2(vector<float>& target, float x, float y) {
    target.push_back(x);
    target.push_back(y);
}

void ObjectState::addVertex(int a, int b, int c){
    auto& src = vertices;
    auto& dst = object->geometry.vertices;
    /*if (src.size() != dst.size())
        dst.resize(src.size());


    std::copy(src.begin() + a, src.begin() + a + 3, dst.begin() + a);
    std::copy(src.begin() + b, src.begin() + b + 3, dst.begin() + b);
    std::copy(src.begin() + c, src.begin() + c + 3, dst.begin() + c);*/


    addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
    addVector3(dst, src[b + 0], src[b + 1], src[b + 2]);
    addVector3(dst, src[c + 0], src[c + 1], src[c + 2]);

}

void ObjectState::addColor(int a, int b, int c){
    auto& src = colors;
    auto& dst = object->geometry.colors;

   /* if (src.size() != dst.size())
        dst.resize(src.size());

    std::copy(src.begin() + a, src.begin() + a + 3, dst.end());
    std::copy(src.begin() + b, src.begin() + b + 3, dst.end());
    std::copy(src.begin() + c, src.begin() + c + 3, dst.end());*/

    addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
    addVector3(dst, src[b + 0], src[b + 1], src[b + 2]);
    addVector3(dst, src[c + 0], src[c + 1], src[c + 2]);

}

void ObjectState::addUV(int a, int b, int c){
    auto& src = uvs;
    auto& dst = object->geometry.uvs;


   /* if (src.size() != dst.size())
        dst.resize(src.size());

    std::copy(src.begin() + a, src.begin() + a + 2, dst.end());
    std::copy(src.begin() + b, src.begin() + b + 2, dst.end());
    std::copy(src.begin() + c, src.begin() + c + 2, dst.end()); */

    addVector2(dst, src[a + 0], src[a + 1]);
    addVector2(dst, src[b + 0], src[b + 1]);
    addVector2(dst, src[c + 0], src[c + 1]);

}

void ObjectState::addNormal(int a, int b, int c){
    auto& src = normals;
    auto& dst = object->geometry.normals;

   /* if (src.size() != dst.size())
        dst.resize(src.size());

    std::copy(src.begin() + a, src.begin() + a + 3, dst.end());
    std::copy(src.begin() + b, src.begin() + b + 3, dst.end());
    std::copy(src.begin() + c, src.begin() + c + 3, dst.end());*/

    addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
    addVector3(dst, src[b + 0], src[b + 1], src[b + 2]);
    addVector3(dst, src[c + 0], src[c + 1], src[c + 2]);

}

ObjectState::ObjectState(){
    object = make_shared<Object>();
}

void ObjectState::startObject(const string& name, bool fromDeclaration){
    if (this->object!=nullptr && this->object->fromDeclaration == false){
        this->object->name = name;
        this->object->fromDeclaration = fromDeclaration != false;

        return;
    }

    auto previousMaterial = object->currentMaterial();

    if (this->object!=nullptr){
        this->object->finalize(true);
    }

    this->object = make_shared<Object>();
    this->object->name = name;
    this->object->fromDeclaration = fromDeclaration != false;


//    if (previousMaterial!=nullptr && previousMaterial->name != ""){
//        Object::ObjectMaterial::ptr declared = previousMaterial->clone(0);
//        declared->inherited = true;
//        object->materials.push_back(declared);
//    }

    this->objects.push_back(this->object);
}

void ObjectState::finalize(){
    if (object!=nullptr)
    {
        object->finalize(true);
    }
}

//adapting tinyobj
void ObjectState::addVertexPointFromArray(float src[3]){
    auto& dst = object->geometry.vertices;

    /* if (src.size() != dst.size())
         dst.resize(src.size());

     std::copy(src.begin() + a, src.begin() + a + 2, dst.end());*/

    addVector3(dst, src[0], src[1], src[2]);

}

void ObjectState::addNormalFromArray(float src[3]){
    auto& dst = object->geometry.normals;

    /* if (src.size() != dst.size())
         dst.resize(src.size());

     std::copy(src.begin() + a, src.begin() + a + 3, dst.end());
     std::copy(src.begin() + b, src.begin() + b + 3, dst.end());
     std::copy(src.begin() + c, src.begin() + c + 3, dst.end());*/

    addVector3(dst, src[0], src[1], src[2]);

}

void ObjectState::addUVFromArray(float src[2]){
    auto& dst = object->geometry.uvs;

    /* if (src.size() != dst.size())
         dst.resize(src.size());*/

    addVector2(dst, src[0], src[1]);
}

void ObjectState::addColorFromFloat(float a, float b, float c) {
    auto& dst = object->geometry.colors;

    /* if (src.size() != dst.size())
         dst.resize(src.size());

     std::copy(src.begin() + a, src.begin() + a + 3, dst.end());
     std::copy(src.begin() + b, src.begin() + b + 3, dst.end());
     std::copy(src.begin() + c, src.begin() + c + 3, dst.end());*/

    addVector3(dst, a , b, c);
}



void ObjectState::addVertexPoint(int a){
    auto& src = vertices;
    auto& dst = object->geometry.vertices;

   /* if (src.size() != dst.size())
        dst.resize(src.size());

    std::copy(src.begin() + a, src.begin() + a + 2, dst.end());*/

    addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);

}

void ObjectState::addVertexLine(int a){
    auto& src = vertices;
    auto& dst = object->geometry.vertices;

    /*
    if (src.size() != dst.size())
        dst.resize(src.size());

    std::copy(src.begin() + a, src.begin() + a + 2, dst.end());
    */

    addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
}

void ObjectState::addUVLine(int a){
    auto& src = uvs;
    auto& dst = object->geometry.uvs;

   /* if (src.size() != dst.size())
        dst.resize(src.size());

    std::copy(src.begin() + a, src.begin() + a + 1, dst.end());*/
    addVector2(dst, src[a + 0], src[a + 1]);
}

void ObjectState::addPointGeometry(const vector<string>& _vertices){
    object->geometry.type = "Points";

    auto vLen = _vertices.size();

    for (auto vi = 0; vi < _vertices.size(); vi++)
    {
        addVertexPoint(parseVertexIndex(atoi(_vertices[vi].c_str()), vLen));
    }
}

void ObjectState::addPointGeometry(const vector<float>& _vertices){
    object->geometry.type = "Points";

    auto vLen = _vertices.size();

    for (auto vi = 0; vi < _vertices.size(); vi++)
    {
        addVertexPoint(parseVertexIndex((int)_vertices[vi], vLen));
    }
}

void ObjectState::addLineGeometry(const vector<string>& _vertices, const vector<string>& _uvs){
    object->geometry.type = "Line";

    auto vLen = _vertices.size();
    auto uvLen = _uvs.size();

    for (auto vi = 0; vi < _vertices.size(); vi++){

        addVertexLine(parseVertexIndex(atoi(_vertices[vi].c_str()), vLen));

    }

    if (_uvs.size()> 0){
        for (auto uvi = 0; uvi < _uvs.size(); uvi++)
        {

            addUVLine(parseUVIndex(atoi(_uvs[uvi].c_str()), uvLen));

        }
    }
}

void ObjectState::addLineGeometry(const vector<float>& _vertices, vector<float> _uvs){
    object->geometry.type = "Line";

    auto vLen = _vertices.size();
    auto uvLen = _uvs.size();

    for (auto vi = 0; vi < _vertices.size(); vi++){

        addVertexLine(parseVertexIndex((int)_vertices[vi], vLen));

    }

    if (_uvs.size() > 0){
        for (auto uvi = 0; uvi < _uvs.size(); uvi++)
        {

            addUVLine(parseUVIndex((int)_uvs[uvi], uvLen));

        }
    }
}

void OBJLoader::parseState(ObjectState& state){
    state.startObject("", false);
}

float OBJLoader::parseFloat(string value){
    return atof(value.c_str());
}

int OBJLoader::parseInt(string value){
    return atoi(value.c_str());
}

void ObjectState::addFace(int a, int b, int c, int ua, int ub, int uc, int na, int nb, int nc){
    auto vLen = vertices.size();

    auto ia = parseVertexIndex(a, vLen);
    auto ib = parseVertexIndex(b, vLen);
    auto ic = parseVertexIndex(c, vLen);

    addVertex(ia, ib, ic);

    if (colors.size() > 0){
        addColor(ia, ib, ic);
    }

    if (ua != numeric_limits<int>::quiet_NaN()){

        auto uvLen = uvs.size();
        ia = parseUVIndex(ua, uvLen);
        ib = parseUVIndex(ub, uvLen);
        ic = parseUVIndex(uc, uvLen);

        addUV(ia, ib, ic);
    }

    if (na != numeric_limits<int>::quiet_NaN()){

        // Normals are many times the same. If so, skip function call and parseInt.
        auto nLen = normals.size();
        ia = parseNormalIndex(na, nLen);

        ib = na == nb ? ia : parseNormalIndex(nb, nLen);
        ic = na == nc ? ia : parseNormalIndex(nc, nLen);

        addNormal(ia, ib, ic);

    }
}

void ObjectState::addFace(int a, int b, int c, int d, int ua, int ub, int uc, int ud, int na, int nb, int nc, int nd){
    auto vLen = vertices.size();

    auto ia = parseVertexIndex(a, vLen);
    auto ib = parseVertexIndex(b, vLen);
    auto ic = parseVertexIndex(c, vLen);

    if (d == numeric_limits<int>::quiet_NaN()){
        addVertex(ia, ib, ic);
    }else{
        auto id = parseVertexIndex(d, vLen);

        addVertex(ia, ib, id);
        addVertex(ib, ic, id);
    }

    if (colors.size() > 0){

        addColor(ia, ib, ic);

    }

    if (ua != numeric_limits<int>::quiet_NaN()){

        auto uvLen = uvs.size();
        ia = parseUVIndex(ua, uvLen);
        ib = parseUVIndex(ub, uvLen);
        ic = parseUVIndex(uc, uvLen);

        if (d == numeric_limits<int>::quiet_NaN()){
            addUV(ia, ib, ic);
        }else{
            auto id = parseUVIndex(ud, uvLen);

            addUV(ia, ib, id);
            addUV(ib, ic, id);
        }

    }

    if (na != numeric_limits<int>::quiet_NaN()){

        // Normals are many times the same. If so, skip function call and parseInt.
        auto nLen = normals.size();
        ia = parseNormalIndex(na, nLen);

        ib = na == nb ? ia : parseNormalIndex(nb, nLen);
        ic = na == nc ? ia : parseNormalIndex(nc, nLen);

        if (d == numeric_limits<int>::quiet_NaN()){
            addNormal(ia, ib, ic);
        }else{
            auto id = parseNormalIndex(nd, nLen);

            addNormal(ia, ib, id);
            addNormal(ib, ic, id);
        }

    }
}

void OBJLoader::setMaterials(const MTLLoader::MaterialCreator& materials){
    this->materials = materials;
}

typedef struct {
    GLuint vb_id;  // vertex buffer id
    int numTriangles;
    size_t material_id;
} DrawObject;

struct vec3 {
    float v[3];
    vec3() {
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
    }
};

void normalizeVector(vec3 &v) {
    float len2 = v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
    if (len2 > 0.0f) {
        float len = sqrtf(len2);

        v.v[0] /= len;
        v.v[1] /= len;
        v.v[2] /= len;
    }
}

static std::string GetBaseDir(const std::string& filepath) {
    if (filepath.find_last_of("/\\") != std::string::npos)
        return filepath.substr(0, filepath.find_last_of("/\\"));
    return "";
}

bool hasSmoothingGroup(const tinyobj::shape_t& shape){
    for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i++) {
        if (shape.mesh.smoothing_group_ids[i] > 0) {
            return true;
        }
    }
    return false;
}

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
    float v10[3];
    v10[0] = v1[0] - v0[0];
    v10[1] = v1[1] - v0[1];
    v10[2] = v1[2] - v0[2];

    float v20[3];
    v20[0] = v2[0] - v0[0];
    v20[1] = v2[1] - v0[1];
    v20[2] = v2[2] - v0[2];

    N[0] = v10[1] * v20[2] - v10[2] * v20[1];
    N[1] = v10[2] * v20[0] - v10[0] * v20[2];
    N[2] = v10[0] * v20[1] - v10[1] * v20[0];

    float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
    if (len2 > 0.0f) {
        float len = sqrtf(len2);

        N[0] /= len;
        N[1] /= len;
        N[2] /= len;
    }
}

void computeSmoothingNormals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape,
                             std::map<int, vec3>& smoothVertexNormals) {
    smoothVertexNormals.clear();
    std::map<int, vec3>::iterator iter;

    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
        // Get the three indexes of the face (all faces are triangular)
        tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
        tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
        tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

        // Get the three vertex indexes and coordinates
        int vi[3];      // indexes
        float v[3][3];  // coordinates

        for (int k = 0; k < 3; k++) {
            vi[0] = idx0.vertex_index;
            vi[1] = idx1.vertex_index;
            vi[2] = idx2.vertex_index;
            assert(vi[0] >= 0);
            assert(vi[1] >= 0);
            assert(vi[2] >= 0);

            v[0][k] = attrib.vertices[3 * vi[0] + k];
            v[1][k] = attrib.vertices[3 * vi[1] + k];
            v[2][k] = attrib.vertices[3 * vi[2] + k];
        }

        // Compute the normal of the face
        float normal[3];
        CalcNormal(normal, v[0], v[1], v[2]);

        // Add the normal to the three vertexes
        for (size_t i = 0; i < 3; ++i) {
            iter = smoothVertexNormals.find(vi[i]);
            if (iter != smoothVertexNormals.end()) {
                // add
                iter->second.v[0] += normal[0];
                iter->second.v[1] += normal[1];
                iter->second.v[2] += normal[2];
            } else {
                smoothVertexNormals[vi[i]].v[0] = normal[0];
                smoothVertexNormals[vi[i]].v[1] = normal[1];
                smoothVertexNormals[vi[i]].v[2] = normal[2];
            }
        }

    }  // f

    // Normalize the normals, that is, make them unit vectors
    for (iter = smoothVertexNormals.begin(); iter != smoothVertexNormals.end();
         iter++) {
        normalizeVector(iter->second);
    }

}

static void computeSmoothingShape(tinyobj::attrib_t& inattrib, tinyobj::shape_t& inshape,
                                  std::vector<std::pair<unsigned int, unsigned int>>& sortedids,
                                  unsigned int idbegin, unsigned int idend,
                                  std::vector<tinyobj::shape_t>& outshapes,
                                  tinyobj::attrib_t& outattrib) {
    unsigned int sgroupid = sortedids[idbegin].first;
    bool hasmaterials = inshape.mesh.material_ids.size();
    // Make a new shape from the set of faces in the range [idbegin, idend).
    outshapes.emplace_back();
    tinyobj::shape_t& outshape = outshapes.back();
    outshape.name = inshape.name;
    // Skip lines and points.

    std::unordered_map<unsigned int, unsigned int> remap;
    for (unsigned int id = idbegin; id < idend; ++id) {
        unsigned int face = sortedids[id].second;

        outshape.mesh.num_face_vertices.push_back(3); // always triangles
        if (hasmaterials)
            outshape.mesh.material_ids.push_back(inshape.mesh.material_ids[face]);
        outshape.mesh.smoothing_group_ids.push_back(sgroupid);
        // Skip tags.

        for (unsigned int v = 0; v < 3; ++v) {
            tinyobj::index_t inidx = inshape.mesh.indices[3*face + v], outidx;
            assert(inidx.vertex_index != -1);
            auto iter = remap.find(inidx.vertex_index);
            // Smooth group 0 disables smoothing so no shared vertices in that case.
            if (sgroupid && iter != remap.end()) {
                outidx.vertex_index = (*iter).second;
                outidx.normal_index = outidx.vertex_index;
                outidx.texcoord_index = (inidx.texcoord_index == -1) ? -1 : outidx.vertex_index;
            }
            else {
                assert(outattrib.vertices.size() % 3 == 0);
                unsigned int offset = static_cast<unsigned int>(outattrib.vertices.size() / 3);
                outidx.vertex_index = outidx.normal_index = offset;
                outidx.texcoord_index = (inidx.texcoord_index == -1) ? -1 : offset;
                outattrib.vertices.push_back(inattrib.vertices[3*inidx.vertex_index  ]);
                outattrib.vertices.push_back(inattrib.vertices[3*inidx.vertex_index+1]);
                outattrib.vertices.push_back(inattrib.vertices[3*inidx.vertex_index+2]);
                outattrib.normals.push_back(0.0f);
                outattrib.normals.push_back(0.0f);
                outattrib.normals.push_back(0.0f);
                if (inidx.texcoord_index != -1) {
                    outattrib.texcoords.push_back(inattrib.texcoords[2*inidx.texcoord_index  ]);
                    outattrib.texcoords.push_back(inattrib.texcoords[2*inidx.texcoord_index+1]);
                }
                remap[inidx.vertex_index] = offset;
            }
            outshape.mesh.indices.push_back(outidx);
        }
    }
}

static void computeSmoothingShapes(tinyobj::attrib_t &inattrib,
                                   std::vector<tinyobj::shape_t>& inshapes,
                                   std::vector<tinyobj::shape_t>& outshapes,
                                   tinyobj::attrib_t& outattrib) {
    for (size_t s = 0, slen = inshapes.size() ; s < slen; ++s) {
        tinyobj::shape_t& inshape = inshapes[s];

        unsigned int numfaces = static_cast<unsigned int>(inshape.mesh.smoothing_group_ids.size());
        assert(numfaces);
        std::vector<std::pair<unsigned int,unsigned int>> sortedids(numfaces);
        for (unsigned int i = 0; i < numfaces; ++i)
            sortedids[i] = std::make_pair(inshape.mesh.smoothing_group_ids[i], i);
        sort(sortedids.begin(), sortedids.end());

        unsigned int activeid = sortedids[0].first;
        unsigned int id = activeid, idbegin = 0, idend = 0;
        // Faces are now bundled by smoothing group id, create shapes from these.
        while (idbegin < numfaces) {
            while (activeid == id && ++idend < numfaces)
                id = sortedids[idend].first;
            computeSmoothingShape(inattrib, inshape, sortedids, idbegin, idend,
                                  outshapes, outattrib);
            activeid = id;
            idbegin = idend;
        }
    }
}

static void computeAllSmoothingNormals(tinyobj::attrib_t& attrib,
                                       std::vector<tinyobj::shape_t>& shapes) {
    vec3 p[3];
    for (size_t s = 0, slen = shapes.size(); s < slen; ++s) {
        const tinyobj::shape_t& shape(shapes[s]);
        size_t facecount = shape.mesh.num_face_vertices.size();
        assert(shape.mesh.smoothing_group_ids.size());

        for (size_t f = 0, flen = facecount; f < flen; ++f) {
            for (unsigned int v = 0; v < 3; ++v) {
                tinyobj::index_t idx = shape.mesh.indices[3*f + v];
                assert(idx.vertex_index != -1);
                p[v].v[0] = attrib.vertices[3*idx.vertex_index  ];
                p[v].v[1] = attrib.vertices[3*idx.vertex_index+1];
                p[v].v[2] = attrib.vertices[3*idx.vertex_index+2];
            }

            // cross(p[1] - p[0], p[2] - p[0])
            float nx = (p[1].v[1] - p[0].v[1]) * (p[2].v[2] - p[0].v[2]) -
                       (p[1].v[2] - p[0].v[2]) * (p[2].v[1] - p[0].v[1]);
            float ny = (p[1].v[2] - p[0].v[2]) * (p[2].v[0] - p[0].v[0]) -
                       (p[1].v[0] - p[0].v[0]) * (p[2].v[2] - p[0].v[2]);
            float nz = (p[1].v[0] - p[0].v[0]) * (p[2].v[1] - p[0].v[1]) -
                       (p[1].v[1] - p[0].v[1]) * (p[2].v[0] - p[0].v[0]);

            // Don't normalize here.
            for (unsigned int v = 0; v < 3; ++v) {
                tinyobj::index_t idx = shape.mesh.indices[3*f + v];
                attrib.normals[3*idx.normal_index  ] += nx;
                attrib.normals[3*idx.normal_index+1] += ny;
                attrib.normals[3*idx.normal_index+2] += nz;
            }
        }
    }

    assert(attrib.normals.size() % 3 == 0);
    for (size_t i = 0, nlen = attrib.normals.size() / 3; i < nlen; ++i) {
        tinyobj::real_t& nx = attrib.normals[3*i  ];
        tinyobj::real_t& ny = attrib.normals[3*i+1];
        tinyobj::real_t& nz = attrib.normals[3*i+2];
        tinyobj::real_t len = sqrtf(nx*nx + ny*ny + nz*nz);
        tinyobj::real_t scale = len == 0 ? 0 : 1 / len;
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
}

static bool LoadObjAndConvert(float bmin[3], float bmax[3],
                              std::vector<DrawObject>* drawObjects,
                              std::vector<tinyobj::material_t>& materials,
                              std::map<std::string, GLuint>& textures,
                              ObjectState& objState,
                              const char* filename) {
    tinyobj::attrib_t inattrib;
    std::vector<tinyobj::shape_t> inshapes;

    Timer tm;

    //tm.reset();

    std::string base_dir = GetBaseDir(filename);
    if (base_dir.empty()) {
        base_dir = ".";
    }

    base_dir += threecpp::getFileSeparator();

    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&inattrib, &inshapes, &materials, &warn, &err, filename,
                                base_dir.c_str());
    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }
    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    //tm.end();

    if (!ret) {
        std::cerr << "Failed to load " << filename << std::endl;
        return false;
    }

    printf("Parsing time: %d [ms]\n", (int)tm.get_elapsed_time());

    printf("# of vertices  = %d\n", (int)(inattrib.vertices.size()) / 3);
    printf("# of normals   = %d\n", (int)(inattrib.normals.size()) / 3);
    printf("# of texcoords = %d\n", (int)(inattrib.texcoords.size()) / 2);
    printf("# of materials = %d\n", (int)materials.size());
    printf("# of shapes    = %d\n", (int)inshapes.size());

    // Append `default` material
    materials.push_back(tinyobj::material_t());

    for (size_t i = 0; i < materials.size(); i++) {
        printf("material[%d].diffuse_texname = %s\n", int(i),
               materials[i].diffuse_texname.c_str());
    }

    // Load diffuse textures
//    {
//        for (size_t m = 0; m < materials.size(); m++) {
//            tinyobj::material_t* mp = &materials[m];
//
//            if (mp->diffuse_texname.length() > 0) {
//                // Only load the texture if it is not already loaded
//                if (textures.find(mp->diffuse_texname) == textures.end()) {
//                    GLuint texture_id;
//                    int w, h;
//                    int comp;
//
//                    std::string texture_filename = mp->diffuse_texname;
//                    if (!threecpp::FileExists(texture_filename)) {
//                        // Append base dir.
//                        texture_filename = base_dir + mp->diffuse_texname;
//                        if (!threecpp::FileExists(texture_filename)) {
//                            std::cerr << "Unable to find file: " << mp->diffuse_texname
//                                      << std::endl;
//                            exit(1);
//                        }
//                    }
//
//                    unsigned char* image =
//                            stbi_load(texture_filename.c_str(), &w, &h, &comp, STBI_default);
//                    if (!image) {
//                        std::cerr << "Unable to load texture: " << texture_filename
//                                  << std::endl;
//                        exit(1);
//                    }
//                    std::cout << "Loaded texture: " << texture_filename << ", w = " << w
//                              << ", h = " << h << ", comp = " << comp << std::endl;
//
////                    glGenTextures(1, &texture_id);
////                    glBindTexture(GL_TEXTURE_2D, texture_id);
////                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////                    if (comp == 3) {
////                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
////                                     GL_UNSIGNED_BYTE, image);
////                    } else if (comp == 4) {
////                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
////                                     GL_UNSIGNED_BYTE, image);
////                    } else {
////                        assert(0);  // TODO
////                    }
////                    glBindTexture(GL_TEXTURE_2D, 0);
//                    stbi_image_free(image);
//                    textures.insert(std::make_pair(mp->diffuse_texname, texture_id));
//                }
//            }
//        }
//    }

    bmin[0] = bmin[1] = bmin[2] = std::numeric_limits<float>::max();
    bmax[0] = bmax[1] = bmax[2] = -std::numeric_limits<float>::max();

    bool regen_all_normals = inattrib.normals.size() == 0;
    tinyobj::attrib_t outattrib;
    std::vector<tinyobj::shape_t> outshapes;
    if (regen_all_normals) {
        computeSmoothingShapes(inattrib, inshapes, outshapes, outattrib);
        computeAllSmoothingNormals(outattrib, outshapes);
    }

    std::vector<tinyobj::shape_t>&shapes = regen_all_normals ? outshapes : inshapes;

    tinyobj::attrib_t& attrib = regen_all_normals ? outattrib : inattrib;
    MTLLoader mtlLoader;
    std::vector<std::string> emptyMaterialPathVec;

    {
        for (size_t s = 0; s < shapes.size(); s++) {
            DrawObject o;
            objState.startObject(shapes[s].name,true);

            //std::vector<float> buffer;  // pos(3float), normal(3float), color(3float)

            // Check for smoothing group and compute smoothing normals
            std::map<int, vec3> smoothVertexNormals;
            if (!regen_all_normals && (hasSmoothingGroup(shapes[s]) > 0)) {
                std::cout << "Compute smoothingNormal for shape [" << s << "]" << std::endl;
                computeSmoothingNormals(attrib, shapes[s], smoothVertexNormals);
            }

            std::unordered_set<size_t> objMaterialSet;
            for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
                tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
                tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
                tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

                int current_material_id = shapes[s].mesh.material_ids[f];
                objMaterialSet.insert(current_material_id);

                //tinyobj::material_t currentMaterial;
                if ((current_material_id < 0) ||
                    (current_material_id >= static_cast<int>(materials.size()))) {
                    // Invaid material ID. Use default material.
                    current_material_id = materials.size() - 1;  // Default material is added to the last item in `materials`.

                    //currentMaterial = materials[current_material_id];
                    //objState.object->startMaterial(currentMaterial.name);

                    //objState.object->material_ts.push_back(currentMaterial);
                    //objState.object->startMaterial(currentMaterial.name,emptyMaterialPathVec);
                }
                // if (current_material_id >= materials.size()) {
                //    std::cerr << "Invalid material index: " << current_material_id <<
                //    std::endl;
                //}


                float diffuse[3];
                for (size_t i = 0; i < 3; i++) {
                    diffuse[i] = materials[current_material_id].diffuse[i];
                }
                float tc[3][2];
                if (attrib.texcoords.size() > 0) {
                    if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) ||
                        (idx2.texcoord_index < 0)) {
                        // face does not contain valid uv index.
                        tc[0][0] = 0.0f;
                        tc[0][1] = 0.0f;
                        tc[1][0] = 0.0f;
                        tc[1][1] = 0.0f;
                        tc[2][0] = 0.0f;
                        tc[2][1] = 0.0f;
                    } else {
                        assert(attrib.texcoords.size() >
                               size_t(2 * idx0.texcoord_index + 1));
                        assert(attrib.texcoords.size() >
                               size_t(2 * idx1.texcoord_index + 1));
                        assert(attrib.texcoords.size() >
                               size_t(2 * idx2.texcoord_index + 1));

                        // Flip Y coord.
                        tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
                        tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
                        tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
                        tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
                        tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
                        tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
                    }
                } else {
                    tc[0][0] = 0.0f;
                    tc[0][1] = 0.0f;
                    tc[1][0] = 0.0f;
                    tc[1][1] = 0.0f;
                    tc[2][0] = 0.0f;
                    tc[2][1] = 0.0f;
                }

                float v[3][3];
                for (int k = 0; k < 3; k++) {
                    int f0 = idx0.vertex_index;
                    int f1 = idx1.vertex_index;
                    int f2 = idx2.vertex_index;
                    assert(f0 >= 0);
                    assert(f1 >= 0);
                    assert(f2 >= 0);

                    v[0][k] = attrib.vertices[3 * f0 + k];
                    v[1][k] = attrib.vertices[3 * f1 + k];
                    v[2][k] = attrib.vertices[3 * f2 + k];
                    bmin[k] = std::min(v[0][k], bmin[k]);
                    bmin[k] = std::min(v[1][k], bmin[k]);
                    bmin[k] = std::min(v[2][k], bmin[k]);
                    bmax[k] = std::max(v[0][k], bmax[k]);
                    bmax[k] = std::max(v[1][k], bmax[k]);
                    bmax[k] = std::max(v[2][k], bmax[k]);
                }

                float n[3][3];{
                    bool invalid_normal_index = false;
                    if (attrib.normals.size() > 0) {
                        int nf0 = idx0.normal_index;
                        int nf1 = idx1.normal_index;
                        int nf2 = idx2.normal_index;

                        if ((nf0 < 0) || (nf1 < 0) || (nf2 < 0)) {
                            // normal index is missing from this face.
                            invalid_normal_index = true;
                        } else {
                            for (int k = 0; k < 3; k++) {
                                assert(size_t(3 * nf0 + k) < attrib.normals.size());
                                assert(size_t(3 * nf1 + k) < attrib.normals.size());
                                assert(size_t(3 * nf2 + k) < attrib.normals.size());
                                n[0][k] = attrib.normals[3 * nf0 + k];
                                n[1][k] = attrib.normals[3 * nf1 + k];
                                n[2][k] = attrib.normals[3 * nf2 + k];
                            }
                        }
                    } else {
                        invalid_normal_index = true;
                    }

                    if (invalid_normal_index && !smoothVertexNormals.empty()) {
                        // Use smoothing normals
                        int f0 = idx0.vertex_index;
                        int f1 = idx1.vertex_index;
                        int f2 = idx2.vertex_index;

                        if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
                            n[0][0] = smoothVertexNormals[f0].v[0];
                            n[0][1] = smoothVertexNormals[f0].v[1];
                            n[0][2] = smoothVertexNormals[f0].v[2];

                            n[1][0] = smoothVertexNormals[f1].v[0];
                            n[1][1] = smoothVertexNormals[f1].v[1];
                            n[1][2] = smoothVertexNormals[f1].v[2];

                            n[2][0] = smoothVertexNormals[f2].v[0];
                            n[2][1] = smoothVertexNormals[f2].v[1];
                            n[2][2] = smoothVertexNormals[f2].v[2];

                            invalid_normal_index = false;
                        }
                    }

                    if (invalid_normal_index) {
                        // compute geometric normal
                        CalcNormal(n[0], v[0], v[1], v[2]);
                        n[1][0] = n[0][0];
                        n[1][1] = n[0][1];
                        n[1][2] = n[0][2];
                        n[2][0] = n[0][0];
                        n[2][1] = n[0][1];
                        n[2][2] = n[0][2];
                    }
                }

                for (int k = 0; k < 3; k++) {
//                    objState.vertices.push_back(v[k][0]);
//                    objState.vertices.push_back(v[k][1]);
//                    objState.vertices.push_back(v[k][2]);
                    //std::vector<float> point{v[k][0],v[k][1],v[k][2]};
                    objState.addVertexPointFromArray(v[k]);
//                    objState.normals.push_back(n[k][0]);
//                    objState.normals.push_back(n[k][1]);
//                    objState.normals.push_back(n[k][2]);
                    objState.addNormalFromArray(n[k]);

                    // Combine normal and diffuse to get color.
                    float normal_factor = 0.2;
                    float diffuse_factor = 1 - normal_factor;
                    float c[3] = {n[k][0] * normal_factor + diffuse[0] * diffuse_factor,
                                  n[k][1] * normal_factor + diffuse[1] * diffuse_factor,
                                  n[k][2] * normal_factor + diffuse[2] * diffuse_factor};
                    float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
                    if (len2 > 0.0f) {
                        float len = sqrtf(len2);

                        c[0] /= len;
                        c[1] /= len;
                        c[2] /= len;
                    }
//                    objState.colors.push_back(c[0] * 0.5 + 0.5);
//                    objState.colors.push_back(c[1] * 0.5 + 0.5);
//                    objState.colors.push_back(c[2] * 0.5 + 0.5);
                    objState.addColorFromFloat(c[0] * 0.5 + 0.5,c[1] * 0.5 + 0.5,c[2] * 0.5 + 0.5);

//                    objState.uvs.push_back(tc[k][0]);
//                    objState.uvs.push_back(tc[k][1]);
                    objState.addUVFromArray(tc[k]);

                }
            }

//            o.vb_id = 0;
//            o.numTriangles = 0;
//
//            // OpenGL viewer does not support texturing with per-face material.
//            if (shapes[s].mesh.material_ids.size() > 0 &&
//                shapes[s].mesh.material_ids.size() > s) {
//                o.material_id = shapes[s].mesh.material_ids[0];  // use the material ID
//                // of the first face.
//            } else {
//                o.material_id = materials.size() - 1;  // = ID for default material.
//            }
//            printf("shape[%d] material_id %d\n", int(s), int(o.material_id));
//
//            if (buffer.size() > 0) {
//                glGenBuffers(1, &o.vb_id);
//                glBindBuffer(GL_ARRAY_BUFFER, o.vb_id);
//                glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float),
//                             &buffer.at(0), GL_STATIC_DRAW);
//                o.numTriangles = buffer.size() / (3 + 3 + 3 + 2) /
//                                 3;  // 3:vtx, 3:normal, 3:col, 2:texcoord
//
//                printf("shape[%d] # of triangles = %d\n", static_cast<int>(s),
//                       o.numTriangles);
//            }
            std::unordered_map<std::string,size_t> materialCount;
            for(auto iter=objMaterialSet.begin();iter!=objMaterialSet.end();iter++){
                auto materialId = *iter;
                auto materialName = materials[materialId].name;
                materialCount[materialName] = 0;
                for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
                    if(materialId == shapes[s].mesh.material_ids[f]) {
                        materialCount[materialName] ++;
                    }
                }
                objState.object->material_ts.push_back(materials[materialId]);
                objState.object->startMaterial(materialName,materialCount);
            }
            objState.finalize();
            drawObjects->push_back(o);
        }
    }



    printf("bmin = %f, %f, %f\n", bmin[0], bmin[1], bmin[2]);
    printf("bmax = %f, %f, %f\n", bmax[0], bmax[1], bmax[2]);

    return true;
}

Group::sptr OBJLoader::parse(const string& path){
    ObjectState state;

    parseState(state);

//    state.vertices.reserve(OBJ_SIZE);
//    state.normals.reserve(OBJ_SIZE);
//    state.uvs.reserve(OBJ_SIZE);

    std::vector<DrawObject> gDrawObjects;
    float bmin[3], bmax[3];
    std::vector<tinyobj::material_t> tmaterials;
    std::map<std::string, GLuint> textures;
    std::vector<tinyobj::shape_t> shapes;
    tinyobj::attrib_t attrib;
    if (false == LoadObjAndConvert(bmin, bmax, &gDrawObjects, tmaterials, textures, state,
                                   path.c_str())) {
        //return -1;
        std::cerr << "Loading Obj:" << path << " failed!!!" << std::endl;
    }

    Timer sw;

//    filesystem::path absoluteFilePath = filesystem::absolute(path);
//
//    string filePath = absoluteFilePath.parent_path().string();
//
//    ifstream infile;
//
//    infile.open(path);
//
//    stringstream ss;
//    ss << infile.rdbuf();
//    string textAll = ss.str();
//
//    infile.close();
//
//    if (textAll.find(R"(\r\n)") > -1) {
//        textAll = replace_all(textAll, R"(\r\n)", R"(\n)");
//    }
//    if (textAll.find(R"(\\\n)") > -1) {
//        textAll = replace_all(textAll, R"(\\\n)", "");
//    }
//
//    vector<string> lines = split(textAll, '\n');
//
//    char lineFirstChar;
//
//    auto intNan = numeric_limits<int>::quiet_NaN();
//
//    vector<vector<string>> faceVertices;
//    //faceVertices.reserve(OBJ_SIZE);
//    vector<string> lineVertices;
//    //lineVertices.reserve(OBJ_SIZE);
//    vector<string> lineUVs;
//    //lineUVs.reserve(OBJ_SIZE);
//
//    //对每一行数据进行分析
//    for (int i = 0; i < lines.size(); i++) {
//        //string line = trim(lines[i]);
//        string& line = lines[i];
//        float x, y, z;
//
//        if (line.size() == 0 || line.empty()) continue;
//
//        lineFirstChar = line.c_str()[0];
//
//        if (lineFirstChar == '#') continue;
//
//        if (lineFirstChar == 'v') {
//            //vector<string>& data = split(line, ' ');
//
//            switch (line.c_str()[0]) {
//            case 'v':
//                sscanf_s(line.c_str(), "v %f %f %f", &x, &y, &z);
//                state.vertices.push_back(x);
//                state.vertices.push_back(y);
//                state.vertices.push_back(z);
//                break;
//            default:
//                switch (line.c_str()[1]) {
//                case 'n':
//                    sscanf_s(line.c_str(), "vn %f %f %f", &x, &y, &z);
//                    state.normals.push_back(x);
//                    state.normals.push_back(y);
//                    state.normals.push_back(z);
//                    break;
//                case 't':
//                    sscanf_s(line.c_str(), "vt %f %f", &x, &y);
//                    state.uvs.push_back(x);
//                    state.uvs.push_back(y);
//                    break;
//                }
//            }
//        }
//        else if (lineFirstChar == 'f') {
//            vector<string> vertexData = split(line, ' ');
//            vertexData.erase(vertexData.begin());
//
//            faceVertices.clear();
//            if (vertexData.size() == 1) faceVertices[0] = vertexData;
//            else {
//                for (int j = 0; j < vertexData.size(); j++) {
//                    auto& vertex = vertexData[j];
//                    if (vertex.size() > 0) {
//                        auto vertexParts = split(vertex, '/');
//                        faceVertices.push_back(vertexParts);
//                    }
//                }
//            }
//            auto& v1 = faceVertices[0];
//
//            for (int j = 1; j < faceVertices.size() - 1; j++){
//                auto& v2 = faceVertices[j];
//                auto& v3 = faceVertices[j + 1];
//
//                state.addFace(
//                    parseInt(v1[0]), parseInt(v2[0]), parseInt(v3[0]),
//                    v1.size() > 1 ? parseInt(v1[1]) : numeric_limits<int>::quiet_NaN(), v2.size() > 1 ? parseInt(v2[1]) : numeric_limits<int>::quiet_NaN(), v3.size() > 1 ? parseInt(v3[1]) : numeric_limits<int>::quiet_NaN(),
//                    v1.size() > 2 ? parseInt(v1[2]) : numeric_limits<int>::quiet_NaN(), v2.size() > 2 ? parseInt(v2[2]) : numeric_limits<int>::quiet_NaN(), v3.size() > 2 ? parseInt(v3[2]) : numeric_limits<int>::quiet_NaN()
//                );
//            }
//        }else if (lineFirstChar == 'l'){
//            auto lineParts = split(line, ' ');
//            lineParts.erase(lineParts.begin());
//            lineVertices.clear();
//            lineUVs.clear();
//
//            if (line.find("/") == -1){
//                lineVertices = lineParts;
//            }else{
//                for (int li = 0; li < lineParts.size(); li++)
//                {
//                    auto parts = split(lineParts[li], '/');
//
//                    if (parts[0] != "") lineVertices.push_back(parts[0]);
//                    if (parts[1] != "") lineUVs.push_back(parts[1]);
//                }
//            }
//            state.addLineGeometry(lineVertices, lineUVs);
//        }else if (lineFirstChar == 'p'){
//            auto lineData = line.substr(1);
//            auto pointData = split(lineData, ' ');
//
//            state.addPointGeometry(pointData);
//        }else if(lineFirstChar == 's'){
//            auto result = split(line, ' ');
//            if (result.size() > 1){
//                toLowerCase(result[1]);
//                state.object->smooth = (result[1] != "0" && result[1] != "off");
//            }else{
//                state.object->smooth = true;
//            }
//            auto& material = state.object->currentMaterial();
//            if (material->index != -1) material->smooth = state.object->smooth;
//            continue;
//        }
//
//        else if (lineFirstChar == 'o' || lineFirstChar == 'g'){
//            auto name = line.substr(2);
//            state.startObject(name, true);
//            continue;
//        }else if(lineFirstChar == 'u') {// usemtl
//            string mtlName = line.substr(7);
//            state.object->startMaterial(mtlName, state.materialLibraries);
//            continue;
//        }else if (lineFirstChar == 'm') {// mtllib
//            string matFileName = line.substr(7);
//            string directory = filePath;
//            string matPath = directory + "/" + matFileName;
//            //state.Object.StartMaterial(matPath, state.MaterialLibraries);
//            state.materialLibraries.push_back(matPath);
//            continue;
//        }else{
//            if (line == "\0")
//                continue;
//            else {
//                throw std::runtime_error("THREE.OBJLoader: Unexpected line:" + line);
//            }
//        }
//    }

//    state.finalize();

    Group::sptr container = Group::create();

//    container->materialLibraries = state.materialLibraries;
//
//    if (container->materialLibraries.size() > 0){
//        MTLLoader mtlLoader;
//        for (int i = 0; i < container->materialLibraries.size(); i++){
//            string mtlPath = container->materialLibraries[i];
//            if (threecpp::FileExists(mtlPath))
//                mtlLoader.load(mtlPath);
//        }
//        setMaterials(mtlLoader.multiMaterialCreator);
//    }

    for (size_t i = 0; i < state.objects.size(); i++){
        auto& object = state.objects[i];
        auto& geometry = object->geometry;
        auto& materials = object->materials;
        auto& materialTs = object->material_ts;
        bool isLine = geometry.type == "Line" ? true : false;
        bool isPoints = geometry.type == "Points" ? true : false;
        bool hasVertexColors = false;

        if (geometry.vertices.size() == 0) continue;

        auto bufferGeometry = BufferGeometry::create();

        bufferGeometry->setAttribute(AttributeName::position, BufferAttribute<float>::create(geometry.vertices, 3));

        if (geometry.normals.size() > 0){
            bufferGeometry->setAttribute(AttributeName::normal, BufferAttribute<float>::create(geometry.normals, 3));
        }else{
            bufferGeometry->computeVertexNormals();
        }

        if (geometry.colors.size() > 0){
            hasVertexColors = true;
            bufferGeometry->setAttribute(AttributeName::color, BufferAttribute<float>::create(geometry.colors, 3));
        }

        if (geometry.uvs.size() > 0){
            bufferGeometry->setAttribute(AttributeName::uv, BufferAttribute<float>::create(geometry.uvs, 2));
        }

        // Create materials
        vector<Material::sptr> createdMaterials;

//        for (int mi = 0; mi < materials.size(); mi++){
//            auto sourceMaterial = materials[mi];
//            Material::sptr material = nullptr;
//
//            if (materials.size() > 0 && !sourceMaterial->name.empty()){
//
//                material = this->materials.create(sourceMaterial->name);
//
//                // mtl etc. loaders probably can't create line materials correctly, copy properties to a line material.
//                if (isLine && material != nullptr && !instanceOf<LineBasicMaterial>(material.get())){
//                    LineBasicMaterial::sptr materialLine = LineBasicMaterial::create();
//                    materialLine->copy(*material);
//                    material = materialLine;
//
//                }else if(isPoints && material != nullptr && !instanceOf<PointsMaterial>(material.get())){
//                    PointsMaterial::sptr materialPoints = PointsMaterial::create();
//                    materialPoints->copy(*std::dynamic_pointer_cast<PointsMaterial>(material));
//                    materialPoints->size = 10;
//                    materialPoints->sizeAttenuation = false;
//                    materialPoints->map = material->map;
//                    material = materialPoints;
//                }
//
//            }
//
//            if (material == nullptr){
//                if (isLine){
//                    material = LineBasicMaterial::create();
//                }else if(isPoints){
//                    material = PointsMaterial::create();
//                    material->size = 1;
//                    material->sizeAttenuation = false;
//                }else{
//                    material = MeshPhongMaterial::create();
//                }
//
//                material->name = sourceMaterial->name;
//            }
//
//            material->flatShading = sourceMaterial->smooth ? false : true;
//            material->vertexColors = hasVertexColors ? true : false;
//
//            createdMaterials.push_back(material);
//        }
        for (int mi = 0; mi < materialTs.size(); mi++){
            auto sourceMaterialT = materialTs[mi];
            Material::sptr material = nullptr;

            if (materialTs.size() > 0 && !sourceMaterialT.name.empty()){

                material = this->materials.createMaterialFromMaterialT(sourceMaterialT,GetBaseDir(path));

                // mtl etc. loaders probably can't create line materials correctly, copy properties to a line material.
                if (isLine && material != nullptr && !instanceOf<LineBasicMaterial>(material.get())){
                    LineBasicMaterial::sptr materialLine = LineBasicMaterial::create();
                    materialLine->copy(*material);
                    material = materialLine;

                }else if(isPoints && material != nullptr && !instanceOf<PointsMaterial>(material.get())){
                    PointsMaterial::sptr materialPoints = PointsMaterial::create();
                    materialPoints->copy(*std::dynamic_pointer_cast<PointsMaterial>(material));
                    materialPoints->size = 10;
                    materialPoints->sizeAttenuation = false;
                    materialPoints->map = material->map;
                    material = materialPoints;
                }

            }

            if (material == nullptr){
                if (isLine){
                    material = LineBasicMaterial::create();
                }else if(isPoints){
                    material = PointsMaterial::create();
                    material->size = 1;
                    material->sizeAttenuation = false;
                }else{
                    material = MeshPhongMaterial::create();
                }

                material->name = sourceMaterialT.name;
            }

            //todo:fix this
            material->flatShading = false;//sourceMaterial->smooth ? false : true;
            //material->vertexColors = hasVertexColors ? true : false;

            createdMaterials.push_back(material);
        }

        // Create Mesh
        Mesh::sptr mesh;

        if (createdMaterials.size() > 1){
            for (int mi = 0; mi < materials.size(); mi++){

                auto sourceMaterial = materials[mi];
                //bufferGeometry->addGroup(sourceMaterial., sourceMaterial->groupCount, mi);
                bufferGeometry->addGroup(sourceMaterial->groupStart, sourceMaterial->groupCount, mi);

            }

            if (isLine){

                auto lineSegment = LineSegments::create(bufferGeometry, createdMaterials);
                lineSegment->name = object->name;
                container->add(lineSegment);
            }else if(isPoints){

                auto points = Points::create(bufferGeometry, createdMaterials);
                points->name = object->name;
                container->add(points);

            }else{

                mesh = Mesh::create(bufferGeometry, createdMaterials);
                mesh->name = object->name;
                container->add(mesh);

            }

        }else{

            if (isLine){
                auto lineSegment = LineSegments::create(bufferGeometry, createdMaterials[0]);
                lineSegment->name = object->name;
                container->add(lineSegment);
            }else if(isPoints){
                auto points = Points::create(bufferGeometry, createdMaterials[0]);
                points->name = object->name;
                container->add(points);
            }else{
                mesh = Mesh::create(bufferGeometry, createdMaterials[0]);
                mesh->name = object->name;
                container->add(mesh);
            }
        }
    }

    return container;
}

Group::sptr OBJLoader::load(const string& path){
    return parse(path);
}

